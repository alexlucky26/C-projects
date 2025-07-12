#include "async.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket): socket_(std::move(socket)) {}
    void start(int bulkSize){ 
        do_read(bulkSize); 
    }
private:
    void do_read(int bulkSize);
    tcp::socket socket_;
    enum
    {
        max_length = 1024
    };
    char data_[max_length];
};

class server
{
public:
    server(boost::asio::io_context &io_context, short port, int bulkSize)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), bulkSize_(bulkSize) {
        do_accept();
    }

private:
    void do_accept();
    tcp::acceptor acceptor_;
    int bulkSize_;
};