#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "ThreadSafeQueue.h"
#include <memory>
#include "BlockTask.h"

using namespace std;

class BlockProcessor {
public:
    BlockTask block;
    virtual void ProcessCommand(const string& command) = 0;
    virtual ~BlockProcessor() = default;
};

/// @brief processing of static blocks
class StaticBlockProcessor : public BlockProcessor {
public:
    size_t N;
    StaticBlockProcessor(size_t n) : N(n) {}
    void ProcessCommand(const string& command) override;
};

/// @brief processing of dynamic blocks in braces '{' and '}'
class DynamicBlockProcessor : public BlockProcessor {
public:
    void ProcessCommand(const string& command) override;
};

class BulkParser {
    unique_ptr<BlockProcessor> staticProcessor;
    unique_ptr<BlockProcessor> dynamicProcessor;
    size_t braceCounter = 0; // a counter for amount of braces '{}' to recognize braces that should be ignored
public:
    ThreadSafeQueue BlockTasks;
    std::mutex mutex; // для синхронизации доступа к parser
    BulkParser(std::unique_ptr<BlockProcessor> staticProc, std::unique_ptr<BlockProcessor> dynamicProc)
        : staticProcessor(std::move(staticProc)), dynamicProcessor(std::move(dynamicProc)) {}

    void ParseCommand(const string& command);
    void Finalize();
    void FlushBlockToTasks(BlockProcessor& processor);
};

