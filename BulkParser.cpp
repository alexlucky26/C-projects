#include "BulkParser.h"

/// @brief processing 'static' and 'dynamic' commands
/// @param command 
void BulkParser::ParseCommand(const string& command) 
{
    if (command == "{") 
    {
        if (braceCounter == 0) {
            FlushBlockToTasks(staticProcessor);
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
                FlushBlockToTasks(dynamicProcessor);
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
            StaticBlockProcessor* staticBlockProcessor = dynamic_cast<StaticBlockProcessor*>(&staticProcessor);
            if (staticBlockProcessor && staticBlockProcessor->block.commands.size() == staticBlockProcessor->N) {
                FlushBlockToTasks(staticProcessor);
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
        FlushBlockToTasks(staticProcessor);
    }
    LoggerBlockTasks.close();
    FileBlockTasks.close();
}

/// @brief output of a block into a file (and into cout)
/// @param block commands
void BlockProcessor::FlushBlock() 
{
    //blockTasks.push(block);
    // // записывает в лог файл
    // // при этом, записывает в файл1 если команда нечетная
    // // и если четное, то в поток файл2
    // //
    // // Следует обратить внимание на недостаточную точность часов для формирования уникального имени. 
    // // Необходимо, сохранив timestamp в имени, добавить дополнительный постфикс, который будет гарантированно отличаться у файловых потоков.
    // if (block.empty()) return;
    //    time_t timestamp = time(nullptr);
    // stringstream logFilename;
    // logFilename << "bulk" << timestamp << ".log";

    // ofstream logFile(logFilename.str());
    // cout << "bulk: ";

    // for (size_t i = 0; i < block.size(); i++) 
    // {
    //     cout << block[i];
    //     logFile << block[i];
    //     if (i < block.size() - 1) 
    //     {
    //         cout << ", ";
    //         logFile << "\n";
    //     }
    // }
    // cout << endl;
    // logFile.close();
    // block.clear();
}

void StaticBlockProcessor::ProcessCommand(const string& command) {
    block.commands.push_back(command);
    // if (block.commands.size() == N) {
    //     FlushBlock();
    // }
}

void DynamicBlockProcessor::ProcessCommand(const string& command) {
    block.commands.push_back(command);
}