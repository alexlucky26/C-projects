#include "BulkServer.h"
#include <boost/asio.hpp>
#include <iostream>

// # join_server <port>
int main(int argc, char *argv[])
{
    locale::global(locale(""));
    if (argc != 2)
    {
        cerr << "Usage: join_server <port>\n";
        return 1;
    }
    const int port = atoi(argv[1]);
    if (port <= 0 || port > 65535)
    {
        cerr << "Invalid port number: " << port << '\n';
        return 1;
    }
    boost::asio::io_context io_context;
    Server server(io_context, atoi(argv[1]));
    cout << "The Join Server started on port " << port << '\n';
    io_context.run();

    return 0;
}
