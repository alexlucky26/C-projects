#include "async.h"

#include <filesystem> 
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    cout << "Enter file path:\n";
    string path;
    while (getline(cin, path))
    {
        if (!fs::exists(path) || !fs::is_regular_file(path))
        {
            cerr << "File not found: " << path << '\n';
            continue;
        }

        const uintmax_t sz = fs::file_size(path);
        vector<char> buffer(sz);
        ifstream in(path, ios::binary);
        if (!in.read(buffer.data(), static_cast<streamsize>(sz)))
        {
            cerr << "Error reading file: " << path << '\n';
            continue;
        }

        void* ctx = libasync::connect(/*bulk size*/ 5);
        libasync::receive(ctx, buffer.data(), buffer.size());
        libasync::disconnect(ctx);
        cout << "Processed: " << path << '\n';
    }
    return 0;
}
