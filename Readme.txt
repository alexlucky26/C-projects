Данный код реализует поиск дубликатов файлов на основе хешей содержимого. Система:
1. Обходит директории рекурсивно (с ограничением по глубине);
2. Фильтрует файлы по маске и минимальному размеру;
3. Разбивает файлы на блоки заданного размера;
4. Считает хеши блоков (CRC32) для каждого файла;
5. Сравнивает хеши, чтобы определить, являются ли два файла дубликатами;
6. Группирует дубликаты и выводит их список.

Список аргументов:
  -h [ --help ]                        Show help message
  -i [ --included_dirs ] arg           Directories to scan (can be multiple)
  -e [ --excluded_dirs ] arg           Directories to exclude from scanning
                                       (can be multiple)
  -l [ --level ] arg (=0)              Scanning depth level (0 - only specified
                                       directory without subdirectories)
  -s [ --file_size ] arg (=1)          Minimum file size in bytes (default: 1)
  -m [ --masks ] arg                   File name masks for comparison (case
                                       insensitive, default: * (all files))
  -b [ --bulk ] arg (=4096)            Block size for file reading
  -a [ --hash_algorithm ] arg (=crc32) Hash algorithm (only crc32 supported)