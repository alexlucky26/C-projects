#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <algorithm>
#include <memory>
#include "ThreadSafeQueue.h"

using namespace std;

/// @brief a base class with common functions for Static and Dynamic processors
using Block = vector<string>;
struct BlockTask {
    Block commands;
    std::time_t timestamp;
};
class BlockProcessor {
public:
    BlockTask block;
    void FlushBlock();

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
    BlockProcessor& staticProcessor;
    BlockProcessor& dynamicProcessor;
    size_t braceCounter = 0; // a counter for amount of braces '{}' to recognize braces that should be ignored
public:
    ThreadSafeQueue FileBlockTasks;
    ThreadSafeQueue LoggerBlockTasks;
    BulkParser(BlockProcessor& staticProc, BlockProcessor& dynamicProc) 
        : staticProcessor(staticProc), dynamicProcessor(dynamicProc) {}

    void ParseCommand(const string& command);
    void Finalize();
    void FlushBlockToTasks(BlockProcessor& processor);
};

