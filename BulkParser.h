#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <algorithm>

using namespace std;

class BulkParser {
    size_t N;
    vector<string> staticBlock; // just a usual block
    vector<string> dynamicBlock; // a block that is located between '{' and '}'
    size_t braceCounter = 0; // a counter of braces '{'. It needed to understand an amount of '{' to ignore some of '{}' elements
    void FlushBlock(const vector<string>& block);

public:
    BulkParser(size_t n) : N(n) {}
    void ProcessCommand(const string& command);
    void Finalize();
};