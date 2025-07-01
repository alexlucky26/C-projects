#pragma once
#include <vector>
#include <string>

// alias, чтобы не подтягивать <vector>/<string> в каждый другой заголовок
using Block = std::vector<std::string>;

struct BlockTask {
    Block        commands;
    long long    timestamp{};
};