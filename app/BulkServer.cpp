#include "BulkServer.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void Session::start()
{
    do_read();
}

void Session::do_read()
{
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            [this, self](boost::system::error_code ec, size_t length)
                            {
                                if (!ec)
                                {
                                    //cout << "receive " << length << "=" << string{data_, length} << endl;
                                    vector<string> responses = processor_.execute(std::string(data_, length));
                                    do_write(responses);
                                }
                            });
}

void Session::do_write(vector<string>& responses)
{
    auto self(shared_from_this());

    // Сериализуем вектор строк в одну строку с разделителем \n
    std::string response_buffer;
    for (const auto& line : responses) {
        response_buffer += line + "\n";
    }

    boost::asio::async_write(socket_, boost::asio::buffer(response_buffer.c_str(), response_buffer.size()),
                             [this, self](boost::system::error_code ec, size_t /*length*/)
                             {
                                 if (!ec)
                                 {
                                     do_read();
                                 }
                             });
}

Server::Server(boost::asio::io_context &io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
{
    do_accept();
}

void Server::do_accept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                make_shared<Session>(move(socket))->start();
            }

            do_accept();
        });
}