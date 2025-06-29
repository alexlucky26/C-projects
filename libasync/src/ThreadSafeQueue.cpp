#include "../include/ThreadSafeQueue.h"

void ThreadSafeQueue::push(BlockTask value) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(value));
    }
    cv_.notify_one(); // разбудить один ожид. поток
}

// блокирующее извлечение; возвращает false, если очередь закрыли
bool ThreadSafeQueue::pop(BlockTask& out) {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return closed_ || !queue_.empty(); });
    if (closed_ && queue_.empty()) return false;

    out = std::move(queue_.front());
    queue_.pop();
    return true;
}

// вызываем, когда больше не будет push
void ThreadSafeQueue::close() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        closed_ = true;
    }
    cv_.notify_all();
}