// BulkServer.cpp
#include "BulkServer.h"
#include <iostream>
#include "BulkParser.h"

using boost::asio::ip::tcp;
using namespace std;

session::session(void* ctx, tcp::socket socket) : socket_(std::move(socket)), ctx_(ctx)
{
    auto* context = static_cast<Context*>(ctx_);
    parser_ = make_unique<BulkParser>(
        context->staticProcessor,
        make_unique<DynamicBlockProcessor>()
    );
    parser_->FileBlockTasks = context->FileBlockTasks;
    parser_->LoggerBlockTasks = context->LoggerBlockTasks;
}

void session::start() {
    do_read();
}

void session::do_read()
{
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, size_t length)
        {
            if (!ec)
            {
                std::istringstream iss(std::string(data_, length));
                std::string line;
                while (std::getline(iss, line)) {
                    if (!line.empty()) {
                        parser_->ParseCommand(line);
                    }
                }
                do_read();
            }
        });
}

server::server(boost::asio::io_context& io_context, short port, int bulkSize) : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), bulkSize_(bulkSize)
{
    ctx_ = libasync::connect(bulkSize_);
    do_accept();
}

void server::do_accept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                std::make_shared<session>(ctx_, std::move(socket))->start();
            }
            do_accept();
        });
}