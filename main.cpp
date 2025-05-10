#include "BulkParser.h"

#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "An arguments' amount is wrong. Should be an argument N provided!" << endl;
        return 1;
    }

    size_t N = stoi(argv[1]);

    StaticBlockProcessor staticProcessor(N);
    DynamicBlockProcessor dynamicProcessor;
    BulkParser parser(staticProcessor, dynamicProcessor);

    string command;
    while (getline(cin, command)) 
    {
        if (command == "") break;
            parser.ParseCommand(command);
    }

    parser.Finalize();
    return 0;
}
