#include "Database.h"
#include <optional>
#include <string>
#include <vector>

class CommandProcessor {
public:
    explicit CommandProcessor() {};
    std::vector<std::string> execute(const std::string& command);

private:
    Database db;

    static std::vector<std::string> split(const std::string& str);
    std::optional<Database::TableId> parse_table(const std::string& token);
};