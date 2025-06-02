#include "FileHelper.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Show help message")
        ("included_dirs,i", po::value<std::vector<std::string>>()->multitoken()->required(), 
            "Directories to scan (can be multiple)")
        ("excluded_dirs,e", po::value<std::vector<std::string>>()->multitoken(), 
            "Directories to exclude from scanning (can be multiple)")
        ("level,l", po::value<int>()->default_value(0), 
            "Scanning depth level (0 - only specified directory without subdirectories)")
        ("file_size,s", po::value<size_t>()->default_value(1), 
            "Minimum file size in bytes (default: 1)")
        ("masks,m", po::value<std::vector<std::string>>()->multitoken(), 
            "File name masks for comparison (case insensitive, default: * (all files))")
        ("bulk,b", po::value<size_t>()->default_value(4096), 
            "Block size for file reading")
        ("hash_algorithm,a", po::value<std::string>()->default_value("crc32"), 
            "Hash algorithm (crc32 or md5)");

    try {
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        
        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 0;
        }
        
        po::notify(vm);

        auto included_dirs = vm["included_dirs"].as<std::vector<std::string>>();
        auto excluded_dirs = vm.count("excluded_dirs") ? 
            vm["excluded_dirs"].as<std::vector<std::string>>() : std::vector<std::string>{};
        int level = vm["level"].as<int>();
        size_t min_size = vm["file_size"].as<size_t>();
        auto masks = vm.count("masks") ? 
            vm["masks"].as<std::vector<std::string>>() : std::vector<std::string>{"*"};
        size_t block_size = vm["bulk"].as<size_t>();
        std::string hash_algorithm = vm["hash_algorithm"].as<std::string>();
        
        DuplicateFinder finder(min_size, masks, block_size, hash_algorithm);
        
        for (const auto& dir : included_dirs) {
            finder.find_in_directory(dir, excluded_dirs, level);
        }
        
        finder.print_duplicates();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        std::cerr << desc << "\n";
        return 1;
    }

    return 0;
}