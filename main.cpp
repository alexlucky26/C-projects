#include "BulkParser.h"

#include <iostream>

int main(int argc, char* argv[]) 
{
    if (argc != 2) {
        cerr << "An argumets' amount is wrong. Should be an argument N provided!" << endl;
        return 1;

    }

    size_t N = stoi(argv[1]);
    BulkParser parser(N);
    string command;

    while (getline(cin, command)) 
    {
        if (command == "") // the empty string as an exit from an input mode
            break;
        parser.ProcessCommand(command);
    }

    parser.Finalize();
    return 0;
}
