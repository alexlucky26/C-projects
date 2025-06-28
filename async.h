#pragma once
#include <string>
#include <memory>
#include <queue>
#include <mutex>
#include <thread>

using namespace std;

class BulkParser;
class BlockTask;

struct Context {
    shared_ptr<BulkParser> parser;
    std::vector<std::thread> workers;
    int bulk_size;
};

class libasync {
public:
   static void* connect(size_t N);
   static void receive(void* h, const char* data, size_t size);
   static void disconnect(void* h);
};