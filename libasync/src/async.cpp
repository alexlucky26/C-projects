#include "../include/async.h"
#include "../include/BulkParser.h"

template<typename T>
static void print_task_commands(T& stream, const std::vector<std::string>& commands)
{
    for (size_t idx = 0; idx < commands.size(); ++idx) 
    {
        stream << commands[idx];
        if (idx < commands.size() - 1)
            stream << ", ";
    }
}

static void write_to_log(ThreadSafeQueue& queue){
    BlockTask task;
    if (queue.pop(task)) // ЖДЕТ с помощью conditional variable когда будет хотя бы один блок добавлен в очередь с помощью push
    {
        cout << "bulk: ";
        print_task_commands(cout, task.commands);
        cout << endl;
    }
}

static void write_to_file(ThreadSafeQueue& queue, int thread_idx){
    BlockTask task;
    if (queue.pop(task)) // ЖДЕТ с помощью conditional variable когда будет хотя бы один блок добавлен в очередь с помощью push
    {
        stringstream logFilename;
        logFilename << "bulk" << task.timestamp << ' ' << thread_idx << ".log";

        ofstream logFile(logFilename.str());
        print_task_commands(logFile, task.commands);

        logFile << endl;
        logFile.close();
    }
}

void* libasync::connect(size_t bulk_size) {
    auto ctx = new Context;
    ctx->parser = make_unique<BulkParser>(make_unique<StaticBlockProcessor>(bulk_size), make_unique<DynamicBlockProcessor>()); 
    // запускаем потоки пусть ждут формирования блока в receive. Когда сработает cv_.notify_one в момент когда пушим таск в очередь 
    // Сработает в классе BulkParser в функции FlushBlockToTasks, там пушим в очередь ThreadSafeQueue push.
    ctx->workers.emplace_back(write_to_log, std::ref(ctx->parser->LoggerBlockTasks));
    ctx->workers.emplace_back(write_to_file, std::ref(ctx->parser->FileBlockTasks), 1);
    ctx->workers.emplace_back(write_to_file, std::ref(ctx->parser->FileBlockTasks), 2);
    return ctx;
}

void libasync::receive(void* h, const char* data, size_t size) 
{
    istringstream iss(string(data, size));
    string line;
    Context* context = static_cast<Context*>(h);
    //формируем целый блок
    if (context && context->parser)
    {
        while (getline(iss, line)) {
            context->parser->ParseCommand(line);
        }
        context->parser->Finalize(); // вот тут будет push в LoggerBlockTasks и в FileBlockTasks, под капотом cv notify!
    }

    for (auto& thread_ : context->workers) // все 3 потока параллельны - лог и два файловых, у файловых общая очередь
        thread_.join();
}

void libasync::disconnect(void* h) {
    // Завершаем потоки, освобождаем память
    delete h;
}