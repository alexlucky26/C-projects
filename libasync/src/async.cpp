#include "../include/async.h"
#include "../include/BulkParser.h"

template<typename T>
static void print_task_commands(T& stream, const std::vector<std::string>& commands)
{
    for (const auto& command : commands) 
    {
        stream << command;
        if constexpr (is_same_v<remove_cvref_t<T>, std::ostream>) {
                stream << ' '; // только если это std::ostream (например, std::cout или std::cerr)
        }
    }
    stream << endl;
}

static void write_to_log(ThreadSafeQueue& queue){
    BlockTask task;
    while (queue.pop(task)) // ЖДЕТ с помощью conditional variable когда будет хотя бы один блок добавлен в очередь с помощью push
    {
        if (task.commands.empty()) continue; // если пустой блок, то пропускаем
        cout << "bulk: ";
        print_task_commands(cout, task.commands);
    }
}

static void write_to_file(ThreadSafeQueue& queue, int thread_idx){
    BlockTask task;
    while (queue.pop(task)) // ЖДЕТ с помощью conditional variable когда будет хотя бы один блок добавлен в очередь с помощью push
    {
        stringstream logFilename;
        logFilename << "bulk" << task.timestamp << ' ' << thread_idx << ".log";

        ofstream logFile(logFilename.str());
        print_task_commands(logFile, task.commands);
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
            if (line.empty()) continue; // пропускаем пустые строки
            context->parser->ParseCommand(line);
        }
        
    }
}

void libasync::disconnect(void* h) {
    // Завершаем потоки, освобождаем память
    if (!h) 
        return;

    auto* ctx = static_cast<Context*>(h);
    // 1. Сбросить всё, что осталось в parser
    if (ctx->parser)
        ctx->parser->Finalize(); // вот тут будет push в LoggerBlockTasks и в FileBlockTasks, под капотом cv notify!
    // 2. Ждем завершения потоков
    for (auto& worker : ctx->workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    delete h;
}