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
    session(void* ctx, tcp::socket socket): socket_(std::move(socket)), ctx_(ctx) {}
    void start(int bulkSize){ 
        //ctx_ = libasync::connect(bulkSize);
        do_read(); 
    }
    ~session() {
        // if (ctx_) {
        //     libasync::disconnect(ctx_);
        // }
    }
private:
    void do_read();
    tcp::socket socket_;
    enum
    {
        max_length = 1024
    };
    char data_[max_length];
    void* ctx_ = nullptr; //не владеющая ссылка на глобальный контекст
};

class server
{
public:
    server(boost::asio::io_context &io_context, short port, int bulkSize)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), bulkSize_(bulkSize) {
        ctx_ = libasync::connect(bulkSize_);
        do_accept();
    }
    ~server() {
        libasync::disconnect(ctx_);
    }
private:
    void do_accept();
    tcp::acceptor acceptor_;
    int bulkSize_;
    void* ctx_ = nullptr; // контекст для асинхронной работы с библиотекой libasync
};