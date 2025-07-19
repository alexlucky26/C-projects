#include "../include/async.h"
#include "../include/BulkParser.h"
#include "../include/ThreadSafeQueue.h"
#include <memory>

template<typename T>
static void print_task_commands(T& stream, const std::vector<std::string>& commands)
{
    for (size_t idx = 0; idx < commands.size(); ++idx) 
    {
        stream << commands[idx];
        if (idx < commands.size() - 1)
            stream << ',';
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

void* libasync::connect(size_t bulk_size, bool writeToFile) {
    auto ctx = new Context;
    ctx->parser = make_unique<BulkParser>(make_unique<StaticBlockProcessor>(bulk_size), make_unique<DynamicBlockProcessor>()); 
    // запускаем потоки пусть ждут формирования блока в receive. Когда сработает cv_.notify_one в момент когда пушим таск в очередь 
    // Сработает в классе BulkParser в функции FlushBlockToTasks, там пушим в очередь ThreadSafeQueue push.
    if (!writeToFile)
        ctx->workers.emplace_back(write_to_log, std::ref(ctx->parser->BlockTasks));
    else
    {
        ctx->workers.emplace_back(write_to_file, std::ref(ctx->parser->BlockTasks), 1);
        ctx->workers.emplace_back(write_to_file, std::ref(ctx->parser->BlockTasks), 2);
    }
    return ctx;
}

void libasync::receive(void* h, std::string data)
{
    Context* context = static_cast<Context*>(h);
    if (!context->parser || data.empty()) 
        return;
    context->parser->ParseCommand(data);
}

void libasync::disconnect(void* h) {
    // Завершаем потоки, освобождаем память
    if (!h) 
        return;

    auto* ctx = static_cast<Context*>(h);
    // 1. Сбросить всё, что осталось в parser
    if (ctx->parser)
        ctx->parser->Finalize(); // вот тут push в LoggerBlockTasks и в FileBlockTasks, под капотом cv notify!
    // 2. Закрыть очереди и дождаться воркеров
    for (auto& th : ctx->workers)
        if (th.joinable())
            th.join();
    delete h;
}