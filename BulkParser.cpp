#include "BulkParser.h"

/// @brief processing 'static' and 'dynamic' commands
/// @param command 
void BulkParser::ProcessCommand(const string& command) 
{
    if (command == "{") // start of a dynamic block "{"
    {
        if (!dynamicBlock.empty()) 
        {
            braceCounter++;
        } 
        else 
        {
            if (!staticBlock.empty()) 
            {
                FlushBlock(staticBlock);
                staticBlock.clear();
            }
            braceCounter = 1;
        }
    } 
    else if (command == "}") // end of a dynamic block "}"

    {
        if (braceCounter > 0) 
        {
            braceCounter--;
            if (braceCounter == 0) 
            {
                FlushBlock(dynamicBlock);
                dynamicBlock.clear();
            }
        }
    } 
    else 
    {
        if (braceCounter > 0)
        {
            dynamicBlock.push_back(command);
        } 
        else // finally a static block case
        {
            staticBlock.push_back(command);
            if (staticBlock.size() == N) 
            {
                FlushBlock(staticBlock);
                staticBlock.clear();
            }
        }
    }
}

/// @brief finish an input and flush all that remains in a static block
void BulkParser::Finalize()
{
    if (braceCounter == 0 && !staticBlock.empty()) 
    {
        FlushBlock(staticBlock);
        staticBlock.clear();
    }
}

/// @brief output of a block into a file (and into cout)
/// @param block commands
void BulkParser::FlushBlock(const vector<string>& block) 
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
}