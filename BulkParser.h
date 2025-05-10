#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <algorithm>
#include <memory>

using namespace std;

/// @brief a base class with common functions for Static and Dynamic processors
class BlockProcessor {
public:
    vector<string> block;
    void FlushBlock();

    virtual void ProcessCommand(const string& command) = 0;
    virtual ~BlockProcessor() = default;
};

/// @brief processing of static blocks
class StaticBlockProcessor : public BlockProcessor {
    size_t N;
public:
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
    BulkParser(BlockProcessor& staticProc, BlockProcessor& dynamicProc) 
        : staticProcessor(staticProc), dynamicProcessor(dynamicProc) {}

    void ParseCommand(const string& command);
    void Finalize();
};
