#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <memory>
#include "ThreadSafeQueue.h"
#include "BlockTask.h"
#include "export.hpp"

using namespace std;

LIBASYNC_EXPORT class BlockProcessor {
public:
    BlockTask block;
    virtual void ProcessCommand(const string& command) = 0;
    virtual ~BlockProcessor() = default;
};

/// @brief processing of static blocks
LIBASYNC_EXPORT class StaticBlockProcessor : public BlockProcessor {
public:
    size_t N;
    StaticBlockProcessor(size_t n) : N(n) {}
    LIBASYNC_EXPORT void ProcessCommand(const string& command) override;
};

/// @brief processing of dynamic blocks in braces '{' and '}'
LIBASYNC_EXPORT class DynamicBlockProcessor : public BlockProcessor {
public:
    LIBASYNC_EXPORT void ProcessCommand(const string& command) override;
};

LIBASYNC_EXPORT class BulkParser {
    shared_ptr<BlockProcessor> staticProcessor;
    unique_ptr<BlockProcessor> dynamicProcessor;
    size_t braceCounter = 0; // a counter for amount of braces '{}' to recognize braces that should be ignored
public:
    shared_ptr<ThreadSafeQueue> FileBlockTasks;
    shared_ptr<ThreadSafeQueue> LoggerBlockTasks;
    //BulkParser(std::unique_ptr<BlockProcessor> staticProc, std::unique_ptr<BlockProcessor> dynamicProc)
    //    : staticProcessor(std::move(staticProc)), dynamicProcessor(std::move(dynamicProc)) {}


    // BulkParser additional constructor
    BulkParser(shared_ptr<BlockProcessor> sharedStatic, unique_ptr<BlockProcessor> dynamicProc)
        : staticProcessor(sharedStatic), dynamicProcessor(std::move(dynamicProc)) {}


    LIBASYNC_EXPORT void ParseCommand(const string& command);
    void Finalize();
    void FlushBlockToTasks(BlockProcessor& processor);
};

