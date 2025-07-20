#include <boost/asio.hpp>
#include "CommandProcessor.h"
#include <iostream>

using boost::asio::ip::tcp;
using namespace std;

class Session : public enable_shared_from_this<Session>
{
public:
    Session(tcp::socket socket)
        : socket_(move(socket))
    {
    }
    void start();

private:
    void do_read();
    void do_write(vector<string>& responses);

    tcp::socket socket_;
    enum
    {
        max_length = 512
    };
    char data_[max_length];
    CommandProcessor processor_;
};

class Server
{
public:
    Server(boost::asio::io_context &io_context, short port);

private:
    void do_accept();

    tcp::acceptor acceptor_;
};