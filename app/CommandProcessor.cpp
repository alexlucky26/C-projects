// File: src/command_processor.cpp
#include "CommandProcessor.h"
#include <sstream>
#include <algorithm>

std::vector<std::string> CommandProcessor::execute(const std::string& command) {
    auto tokens = split(command);
    std::vector<std::string> result;

    if (tokens.empty()) {
        result.emplace_back("ERR empty command");
        return result;
    }

    const std::string& cmd = tokens[0];

    if (cmd == "INSERT") {
        if (tokens.size() != 4) {
            result.emplace_back("ERR invalid INSERT format");
            return result;
        }
        auto table = parse_table(tokens[1]);
        if (!table) {
            result.emplace_back("ERR unknown table " + tokens[1]);
            return result;
        }
        int id = std::stoi(tokens[2]);
        std::string error;
        if (!db.insert(*table, id, tokens[3], error)) {
            result.emplace_back("ERR " + error);
        } else {
            result.emplace_back("OK");
        }
    } else if (cmd == "TRUNCATE") {
        if (tokens.size() != 2) {
            result.emplace_back("ERR invalid TRUNCATE format");
            return result;
        }
        auto table = parse_table(tokens[1]);
        if (!table) {
            result.emplace_back("ERR unknown table " + tokens[1]);
            return result;
        }
        db.truncate(*table);
        result.emplace_back("OK");
    } else if (cmd == "INTERSECTION") {
        auto lines = db.intersection();
        result.insert(result.end(), lines.begin(), lines.end());
        result.emplace_back("OK");
    } else if (cmd == "SYMMETRIC_DIFFERENCE") {
        auto lines = db.symmetric_difference();
        result.insert(result.end(), lines.begin(), lines.end());
        result.emplace_back("OK");
    } else {
        result.emplace_back("ERR unknown command " + cmd);
    }

    return result;
}

std::vector<std::string> CommandProcessor::split(const std::string& str) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

std::optional<Database::TableId> CommandProcessor::parse_table(const std::string& token) {
    if (token == "A") return Database::TableId::A;
    if (token == "B") return Database::TableId::B;
    return std::nullopt;
}