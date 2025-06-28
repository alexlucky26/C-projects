#include "async.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    // if (argc != 2) {
    //     cerr << "An arguments' amount is wrong. Should be an argument N provided!" << endl;
    //     return 1;
    // }

    //size_t N = stoi(argv[1]);

    // StaticBlockProcessor staticProcessor(N);
    // DynamicBlockProcessor dynamicProcessor;
    // BulkParser parser(staticProcessor, dynamicProcessor);

    cout << "Enter the file path:" << '\n';
    string filepath;
    while (getline(cin, filepath)) 
    {
        std::ifstream file("D:/example.txt"); // открываем файл для чтения
        if (!file) 
        {
            std::cerr << "Error opening file.\n";
            return 1;
        }
        std::string file_str;
        file.open(file_str);
        void* context = libasync::connect(5);
        libasync::receive(context, file_str.c_str(), file_str.size());
        libasync::disconnect(context);
        file.close();

    }
    return 0;
}
