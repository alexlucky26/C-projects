#include "BulkServer.h"
#include "ThreadSafeQueue.h"
using namespace std;

void SessionHandler::run() 
{
    try {
        boost::asio::streambuf buffer;
        while (true) {
            boost::system::error_code ec;
            size_t len = boost::asio::read_until(socket_, buffer, '\n', ec);

            if (ec == boost::asio::error::eof) {
                break;
            } 
            else if (ec) {
                throw boost::system::system_error(ec);
            }

            std::string line{
                boost::asio::buffers_begin(buffer.data()),
                boost::asio::buffers_begin(buffer.data()) + len - 1};
            buffer.consume(len);
            libasync::receive(global_ctx, line);
            libasync::receive(local_ctx, line);
            // искусственная задержка для демонстрации перемешивания
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    catch (std::exception& e) {
        std::cerr << "Client error: " << e.what() << std::endl;
    }
}

void Server::do_accept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                auto handler = std::make_unique<SessionHandler>(std::move(socket), ctx_, bulkSize_);
                handler->start();
                handlers_.push_back(std::move(handler));
            }
            do_accept();
        });
}
