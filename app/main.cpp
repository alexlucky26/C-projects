#include "BulkServer.h"
#include <boost/asio.hpp>
#include <iostream>

// # bulk_server <port> <bulk_size>
int main(int argc, char *argv[])
{
    locale::global(locale(""));
    if (argc != 3)
    {
        cerr << "Usage: bulk_server <port> <bulk_size>\n";
        return 1;
    }
    const int port = atoi(argv[1]);
    if (port <= 0 || port > 65535)
    {
        cerr << "Invalid port number: " << port << '\n';
        return 1;
    }
    boost::asio::io_context io_context;
    server server(io_context, atoi(argv[1]), atoi(argv[2]));
    cout << "Server started on port " << port << " with bulk size " << argv[2] << '\n';
    io_context.run();

    return 0;
}
