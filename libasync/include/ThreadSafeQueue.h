// Кастомная асинхронная очередь
#pragma once
#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "BlockTask.h"
#include "export.hpp"

class LIBASYNC_EXPORT ThreadSafeQueue {
public:
    ThreadSafeQueue(){};
    void push(BlockTask value);
    // блокирующее извлечение; возвращает false, если очередь закрыли
    bool pop(BlockTask& out);
    // вызываем, когда больше не будет push
    void close();
private:
    std::queue<BlockTask>        queue_;
    std::mutex                     mutex_;
    std::condition_variable        cv_;
    bool                           closed_{false};
};