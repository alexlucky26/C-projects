#pragma once
#include <string>
#include <map>
#include <vector>

class Database {
public:
    enum class TableId { A, B };

    struct Row {
        int id;
        std::string name;
    };

    bool insert(TableId table, int id, const std::string& name, std::string& error);
    void truncate(TableId table);
    std::vector<std::string> intersection() const;
    std::vector<std::string> symmetric_difference() const;

private:
    std::map<int, std::string> tableA;
    std::map<int, std::string> tableB;
};