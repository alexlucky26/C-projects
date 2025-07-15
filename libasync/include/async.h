#pragma once
#include <string>
#include <memory>
#include <queue>
#include <mutex>
#include <thread>
#include "export.hpp"
#include "BulkParser.h"
#include "ThreadSafeQueue.h"

using namespace std;

//class BulkParser;
//class BlockTask;
//class BlockProcessor;

struct Context {
    shared_ptr<BlockProcessor> staticProcessor;
    shared_ptr<BulkParser> parser;
    std::vector<std::thread> workers;
    shared_ptr<ThreadSafeQueue> FileBlockTasks;
    shared_ptr<ThreadSafeQueue> LoggerBlockTasks;
    int bulk_size;
};

class LIBASYNC_EXPORT libasync {
public:
   static void* connect(size_t N);
   static void receive(void* h, const char* data, size_t size);
   static void disconnect(void* h);
};