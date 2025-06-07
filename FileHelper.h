#pragma once
#include <boost/filesystem.hpp>
#include <boost/crc.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/functional/hash.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>


namespace fs = boost::filesystem;

using HashType = uint32_t; // Для CRC32

// Хеширует данные блоками с использованием алгоритма CRC32
class FileHasher {
public:
    // конструктор принимает размер блока и название алгоритма (по-дефолту "crc32")
    FileHasher(size_t block_size, const std::string& algorithm) : block_size_(block_size), algorithm_(algorithm) {}
    // Считает хеш одного блока данных
    HashType compute_hash(const char* data, size_t size);
    // Разбивает файл на блоки и вычисляет хеши каждого блока
    std::vector<HashType> compute_file_hashes(const fs::path& filepath);
private:
    size_t block_size_;  //Размер блока для хеширования
    std::string algorithm_; // Название алгоритма (пока что только "crc32" поддерживается)
};

// Поиск дубликатов на основе размера и хешей содержимого
class DuplicateFinder {
public:
    //Конструктор принимает минимальный размер файла, маски, блок-хешер и алгоритм
    DuplicateFinder(size_t min_size, const std::vector<std::string>& masks, size_t block_size, const std::string& hash_algorithm)
        : min_size_(min_size), masks_(masks), hasher_(block_size, hash_algorithm) {}
    // Рекурсивный поиск файлов в директории
    void find_in_directory(const fs::path& dir, const std::vector<std::string>& excluded_dirs, int max_depth);
    void print_duplicates();
private:
    bool is_excluded(const fs::path& path, const std::vector<std::string>& excluded_dirs);
    bool matches_mask(const fs::path& file);
    // обрабатывает файл - ищет дубликаты, добавляет в группы
    void process_file(const fs::path& filepath);

    struct FileInfo {
        fs::path path;
        size_t size;
        std::vector<HashType> block_hashes;
    };

    bool compare_files(FileInfo& new_file, FileInfo& existing_file);

    // хеш-функция для вектора хешей (используется в unordered_map)
    struct VectorHash {
        size_t operator()(const std::vector<HashType>& vec) const {
            size_t seed = vec.size();
            for (const auto& h : vec) {
                boost::hash_combine(seed, h);
            }
            return seed;
        }
    };

    // карта дубликатов по хешу
    std::unordered_map<std::vector<HashType>, std::vector<FileInfo>, VectorHash> duplicates_;
    size_t min_size_;
    std::vector<std::string> masks_;
    FileHasher hasher_;
     //для предварительной фильтрации группировка файлов по размеру
    std::unordered_map<size_t, std::vector<FileInfo>> size_groups_;
};

