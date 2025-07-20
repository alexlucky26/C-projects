// File: src/database.cpp
#include "Database.h"
#include <sstream>

bool Database::insert(TableId table, int id, const std::string& name, std::string& error) {
    auto& target = (table == TableId::A) ? tableA : tableB;
    if (target.count(id)) {
        error = "duplicate " + std::to_string(id);
        return false;
    }
    target[id] = name;
    return true;
}

void Database::truncate(TableId table) {
    if (table == TableId::A) {
        tableA.clear();
    } else {
        tableB.clear();
    }
}

std::vector<std::string> Database::intersection() const {
    std::vector<std::string> result;
    for (const auto& [id, nameA] : tableA) {
        auto it = tableB.find(id);
        if (it != tableB.end()) {
            std::ostringstream oss;
            oss << id << "," << nameA << "," << it->second;
            result.push_back(oss.str());
        }
    }
    return result;
}

std::vector<std::string> Database::symmetric_difference() const {
    std::vector<std::string> result;
    std::map<int, std::pair<std::string, std::string>> combined;

    for (const auto& [id, name] : tableA) {
        combined[id].first = name;
    }
    for (const auto& [id, name] : tableB) {
        combined[id].second = name;
    }

    for (const auto& [id, names] : combined) {
        bool inA = !names.first.empty();
        bool inB = !names.second.empty();
        if (inA ^ inB) {
            std::ostringstream oss;
            oss << id << "," << names.first << "," << names.second;
            result.push_back(oss.str());
        }
    }
    return result;
}