#include "../include/BulkParser.h"

/// @brief processing 'static' and 'dynamic' commands
/// @param command 
void BulkParser::ParseCommand(const string& command) 
{
    if (command == "{") 
    {
        if (braceCounter == 0) {
            FlushBlockToTasks(*staticProcessor);
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
                FlushBlockToTasks(*dynamicProcessor);
            }
        }
    } 
    else
    {
        if (braceCounter > 0) 
        {
            dynamicProcessor->ProcessCommand(command);
        } 
        else 
        {
            staticProcessor->ProcessCommand(command);
            StaticBlockProcessor* staticBlockProcessor = static_cast<StaticBlockProcessor*>(staticProcessor.get());
            if (staticBlockProcessor && staticBlockProcessor->block.commands.size() == staticBlockProcessor->N) {
                FlushBlockToTasks(*staticProcessor);
            }
        }
    }
}

void BulkParser::FlushBlockToTasks(BlockProcessor& processor)
{
    processor.block.timestamp = time(nullptr);
    LoggerBlockTasks.push(processor.block);
    FileBlockTasks.push(move(processor.block));
}

/// @brief finish an input and flush all that remains in a static block
void BulkParser::Finalize()
{
    if (braceCounter == 0) {
        FlushBlockToTasks(*staticProcessor);
    }
    LoggerBlockTasks.close();
    FileBlockTasks.close();
}

void StaticBlockProcessor::ProcessCommand(const string& command) {
    block.commands.push_back(command);
}

void DynamicBlockProcessor::ProcessCommand(const string& command) {
    block.commands.push_back(command);
}