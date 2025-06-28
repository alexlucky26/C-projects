#include "async.h"
#include "BulkParser.h"
// void* libasync::connect(short N) {
//     // StaticBlockProcessor staticProcessor(N);
//     // DynamicBlockProcessor dynamicProcessor;
//     // BulkParser parser(staticProcessor, dynamicProcessor);

//     return new Context{ N };
// };

// //c передачей указателя на начало буфера, его размера, а также контекста. 
// bool libasync::receive(int pos, short size, void* context) {
//     // string command;
//     // while (getline(cin, command)) 
//     // {
//     //     if (command == "") break;
//     //         parser.ParseCommand(command);
//     // }

//     // parser.Finalize();

//     return false;
// };

// bool libasync::disconnect() {
//     return false;
// };

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
        // for (size_t idx = 0; idx < task.commands.size(); ++idx) 
        // {
        //     cout << task.commands[idx];
        //     if (idx < task.commands.size() - 1)
        //         cout << ", ";
        // }
        cout << endl;
    }
}

static void write_to_file(ThreadSafeQueue& queue, int thread_idx){
    BlockTask task;
    if (queue.pop(task)) // ЖДЕТ с помощью conditional variable когда будет хотя бы один блок добавлен в очередь с помощью push
    {
        // записывает в лог файл
        // при этом, записывает в файл1 если команда нечетная
        // и если четное, то в поток файл2
        //
        // Следует обратить внимание на недостаточную точность часов для формирования уникального имени. 
        // Необходимо, сохранив timestamp в имени, добавить дополнительный постфикс, который будет гарантированно отличаться у файловых потоков.
        stringstream logFilename;
        logFilename << "bulk" << task.timestamp << ' ' << thread_idx << ".log";

        ofstream logFile(logFilename.str());
        print_task_commands(logFile, task.commands);
        // for (size_t idx = 0; idx < task.commands.size(); ++idx) 
        // {
        //     logFile << task.commands[idx];
        //     if (idx < task.commands.size() - 1)
        //         logFile << ", ";
        // }

        logFile << endl;
        logFile.close();
    }
}

void* libasync::connect(size_t bulk_size) {
    StaticBlockProcessor staticProcessor(bulk_size);
    DynamicBlockProcessor dynamicProcessor;
    auto ctx = new Context;
    ctx->parser = make_unique<BulkParser>(move(staticProcessor), move(dynamicProcessor));

    // запускаем потоки пусть ждут формирования блока в receive. Когда сработает cv_.notify_one в момент когда пушим таск в очередь 
    // Сработает в классе BulkParser в функции FlushBlockToTasks, там пушим в очередь ThreadSafeQueue push.
    
    ctx->workers.emplace_back(write_to_log, std::ref(ctx->parser->LoggerBlockTasks));
    ctx->workers.emplace_back(write_to_file, std::ref(ctx->parser->FileBlockTasks), 1);
    ctx->workers.emplace_back(write_to_file, std::ref(ctx->parser->FileBlockTasks), 2);

    // std::thread log_thread(write_to_log, std::ref(ctx->parser->LoggerBlockTasks));
    // std::thread file_thread_1(write_to_file, std::ref(ctx->parser->FileBlockTasks), 1);
    // std::thread file_thread_2(write_to_file, std::ref(ctx->parser->FileBlockTasks), 2);
    return ctx;
}

void libasync::receive(void* h, const char* data, size_t size) 
{
    istringstream iss(string(data, size));
    string line;
    Context* context = ((Context*)h);
    //формируем целый блок
    {
        while (getline(iss, line)) {
            ((Context*)h)->parser->ParseCommand(line);
        }
        context->parser->Finalize(); // вот тут будет push в LoggerBlockTasks и в FileBlockTasks, под капотом cv notify!
    }
    // просто сразу выводим последний добавленный целый блок в лог
    
    //log_thread.join(); // просто ждем когда лог поток выпишет блок
    // workers - 3 thread'a, log, 2 files
    // идёт в log и в один из файлов одновременно

    // и вот тут ассинхронная обработка blockTasks для вывода в файл
    //context->parser->blockTasks;

    for (auto& thread_ : context->workers) // все 3 потока параллельны - лог и два файловых, у файловых общая очередь
        thread_.join();
}

void libasync::disconnect(void* h) {
    // Завершаем потоки, освобождаем память
    delete h;
}