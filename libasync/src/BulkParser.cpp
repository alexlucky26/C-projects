#include "../include/BulkParser.h"
#include <chrono>
/// @brief processing 'static' and 'dynamic' commands
/// @param command 
void BulkParser::ParseCommand(const string& command) 
{
    if (command.at(0) == '{') 
    {
        if (braceCounter == 0) {
            FlushBlockToTasks(*staticProcessor);
        }
        braceCounter++;
    } 
    else if (command.at(0) == '}') 
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
    // более точное время, чем просто time(nullptr), что необходимо для формирования разных имен файлов
    long long now_us = duration_cast<chrono::microseconds>(chrono::system_clock::now().time_since_epoch()).count();
    processor.block.timestamp = now_us;
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