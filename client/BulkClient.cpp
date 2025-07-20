#include "BulkClient.h"
#include <iostream>

BulkClient::BulkClient(const std::string& ip, int port)
    : host_(ip), port_(port), socket_(std::make_unique<boost::asio::ip::tcp::socket>(io_context_)) {
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::make_address(host_), port_);
    socket_->connect(ep);
    std::cout << "Connected to " << host_ << ":" << port_ << "\n";
}

void BulkClient::sendCommand(const std::string& command) {
    boost::asio::write(*socket_, boost::asio::buffer(command + "\n"));
}

std::string BulkClient::receiveResponse() {
    boost::asio::streambuf buffer;
    std::string response;
    
    while (true) {
        boost::asio::read_until(*socket_, buffer, '\n');
        std::istream is(&buffer);
        std::string line;
        std::getline(is, line);
        response += line + "\n";
        if (line == "OK" || line.find("ERR") == 0) {
            break;
        }
    }
    
    return response;
}

void BulkClient::run() {
    try {
        while (true) {
            std::cout << "Enter command (or 'exit' to quit): ";
            std::string command;
            std::getline(std::cin, command);

            if (command == "exit") {
                break;
            }

            sendCommand(command);
            std::string response = receiveResponse();
            std::cout << "Server response: \n" << response << "\n";
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
}