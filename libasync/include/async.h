#pragma once
#include <string>
#include <memory>
#include <queue>
#include <mutex>
#include <thread>
#include <vector>
#include "export.hpp"

using namespace std;

class BulkParser;
class BlockTask;
class ThreadSafeQueue;

struct Context {
    shared_ptr<BulkParser> parser;
    vector<shared_ptr<ThreadSafeQueue>> fileBlocks;
    std::vector<std::thread> workers;
};

class LIBASYNC_EXPORT libasync {
public:
   static void* connect(size_t N, bool writeToFile = false);
   static void receive(void* h, std::string data);
   static void disconnect(void* h);
};