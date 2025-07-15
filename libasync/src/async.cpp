// libasync.cpp
#include "async.h"
//#include "BulkParser.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

template<typename T>
static void print_task_commands(T& stream, const vector<string>& commands)
{
    for (const auto& command : commands)
    {
        stream << command;
        if constexpr (is_same_v<remove_cvref_t<T>, ostream>) {
            stream << ' ';
        }
    }
    stream << endl;
}

static void write_to_log(ThreadSafeQueue& queue) {
    BlockTask task;
    while (queue.pop(task)) {
        if (task.commands.empty()) continue;
        cout << "bulk: ";
        print_task_commands(cout, task.commands);
    }
}

static void write_to_file(ThreadSafeQueue& queue, int thread_idx) {
    BlockTask task;
    while (queue.pop(task)) {
        stringstream logFilename;
        logFilename << "bulk" << task.timestamp << ' ' << thread_idx << ".log";
        ofstream logFile(logFilename.str());
        print_task_commands(logFile, task.commands);
        logFile.close();
    }
}

void* libasync::connect(size_t bulk_size) {
    auto ctx = new Context;
    ctx->LoggerBlockTasks = make_shared<ThreadSafeQueue>();
    ctx->FileBlockTasks = make_shared<ThreadSafeQueue>();
    ctx->staticProcessor = make_shared<StaticBlockProcessor>(bulk_size);
    ctx->workers.emplace_back(write_to_log, std::ref(*ctx->LoggerBlockTasks));
    ctx->workers.emplace_back(write_to_file, std::ref(*ctx->FileBlockTasks), 1);
    ctx->workers.emplace_back(write_to_file, std::ref(*ctx->FileBlockTasks), 2);
    return ctx;
}

void libasync::disconnect(void* h) {
    if (!h)
        return;
    auto* ctx = static_cast<Context*>(h);
    ctx->LoggerBlockTasks->close();
    ctx->FileBlockTasks->close();
    for (auto& worker : ctx->workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    delete ctx;
}
