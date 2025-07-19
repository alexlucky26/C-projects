#include "async.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <thread>
#include <boost/asio.hpp>
#include "ThreadSafeQueue.h"

using boost::asio::ip::tcp;
using namespace std;

class ThreadSafeQueue;

class SessionHandler {
public:
    SessionHandler(tcp::socket socket, void* ctx/*, ThreadSafeQueue& queue*/, int bulkSize)
        : socket_(std::move(socket)), global_ctx(ctx), local_ctx(libasync::connect(bulkSize, true)) {}
    ~SessionHandler() { if (local_ctx) libasync::disconnect(local_ctx); };
    void start() {
        thread_ = std::thread(&SessionHandler::run, this);
    }

    void join() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

private:
    void run();
    tcp::socket socket_;
    std::thread thread_;
    void* global_ctx; // глобальный контекст для работы с логом
    void* local_ctx; // контекст для асинхронной работы с файлами
};

class Server {
public:
    Server(boost::asio::io_context& io_context, short port, int bulkSize)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), 
          bulkSize_(bulkSize),
          ctx_(libasync::connect(bulkSize)) {
            do_accept();
        }

    ~Server() {
        libasync::disconnect(ctx_);
        for (auto& handler : handlers_) {
            handler->join();
        }
    }

private:
    void do_accept();

    tcp::acceptor acceptor_;
    std::vector<std::unique_ptr<SessionHandler>> handlers_;
    int bulkSize_;
    void* ctx_; // глобальный контекст для работы с логом
};