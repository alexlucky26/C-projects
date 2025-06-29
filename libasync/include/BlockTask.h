#pragma once
#include <vector>
#include <string>
#include <ctime>

// alias, чтобы не подтягивать <vector>/<string> в каждый другой заголовок
using Block = std::vector<std::string>;

struct BlockTask {
    Block        commands;
    std::time_t  timestamp{};
};