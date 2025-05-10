#include "BulkParser.h"

/// @brief processing 'static' and 'dynamic' commands
/// @param command 
void BulkParser::ParseCommand(const string& command) 
{
    if (command == "{") 
    {
        if (braceCounter == 0) {
            staticProcessor.FlushBlock();
        }
        braceCounter++;
    } 
    else if (command == "}") 
    {
        if (braceCounter > 0) 
        {
            braceCounter--;
            if (braceCounter == 0) 
            {
                dynamicProcessor.FlushBlock();
            }
        }
    } 
    else
    {
        if (braceCounter > 0) 
        {
            dynamicProcessor.ProcessCommand(command);
        } 
        else 
        {
            staticProcessor.ProcessCommand(command);
        }
    }
}

/// @brief finish an input and flush all that remains in a static block
void BulkParser::Finalize()
{
    if (braceCounter == 0) {
        staticProcessor.FlushBlock();
    }
}

/// @brief output of a block into a file (and into cout)
/// @param block commands
void BlockProcessor::FlushBlock() 
{
    if (block.empty()) return;
       time_t timestamp = time(nullptr);
    stringstream logFilename;
    logFilename << "bulk" << timestamp << ".log";

    ofstream logFile(logFilename.str());
    cout << "bulk: ";

    for (size_t i = 0; i < block.size(); i++) 
    {
        cout << block[i];
        logFile << block[i];
        if (i < block.size() - 1) 
        {
            cout << ", ";
            logFile << "\n";
        }
    }
    cout << endl;
    logFile.close();
    block.clear();
}

void StaticBlockProcessor::ProcessCommand(const string& command) {
    block.push_back(command);
    if (block.size() == N) {
        FlushBlock();
    }
}

void DynamicBlockProcessor::ProcessCommand(const string& command) {
    block.push_back(command);
}