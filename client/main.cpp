#include "BulkClient.h"
#include <iostream>
#include <locale>

int main(int argc, char* argv[]) {
    std::locale::global(std::locale(""));
    if (argc != 3) {
        std::cerr << "Usage: bulk_client <host> <port>\n";
        return 1;
    }

    const std::string host = argv[1];
    const int port = std::atoi(argv[2]);

    if (port <= 0 || port > 65535) {
        std::cerr << "Invalid port number: " << port << '\n';
        return 1;
    }

    try {
        BulkClient client(host, port);
        client.run();
    } catch (const std::exception& ex) {
        std::cerr << "Client error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}