#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <string>

class BulkClient {
public:
    BulkClient(const std::string& ip, int port);
    void run();  // Основной цикл отправки и получения данных

private:
    void sendCommand(const std::string& command);
    std::string receiveResponse();

    std::string host_;
    int port_;
    boost::asio::io_context io_context_;
    std::unique_ptr<boost::asio::ip::tcp::socket> socket_;
};