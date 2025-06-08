#include "FileHelper.h"

HashType FileHasher::compute_hash(const char *data, size_t size)
{
	if (algorithm_ == "crc32")
	{
		boost::crc_32_type result;
		result.process_bytes(data, size);
		return result.checksum();
	}
	throw std::runtime_error("Unsupported hash algorithm");
}

std::vector<HashType> FileHasher::compute_file_hashes(const fs::path &filepath)
{
	std::ifstream file(filepath.string(), std::ios::binary);
	if (!file)
		throw std::runtime_error("Cannot open file: " + filepath.string());

	std::vector<HashType> hashes;
	std::vector<char> buffer(block_size_, 0);

	while (file)
	{
		file.read(buffer.data(), block_size_);
		size_t bytes_read = file.gcount();

		if (bytes_read == 0)
			break;

		// Дополняем нулями если нужно
		if (bytes_read < block_size_)
		{
			std::fill(buffer.begin() + bytes_read, buffer.end(), 0);
		}

		hashes.push_back(compute_hash(buffer.data(), block_size_));
	}

	return hashes;
}

void DuplicateFinder::find_in_directory(const fs::path &dir, const std::vector<std::string> &excluded_dirs, int max_depth)
{
	if (is_excluded(dir, excluded_dirs))
		return;

	try
	{
		for (const auto &entry : fs::directory_iterator(dir))
		{
			if (fs::is_directory(entry.status()))
			{
				if (max_depth > 0)
				{
					find_in_directory(entry.path(), excluded_dirs, max_depth - 1);
				}
			}
			else 
			{
				try 
				{
					if (fs::is_regular_file(entry.status()))
					{
						process_file(entry.path());
					}
				} 
				catch (const boost::filesystem::filesystem_error& e) 
				{
					std::cerr << "Skipping inaccessible file: " << e.what() << std::endl;
				}
			}
		}
	}
	catch (const fs::filesystem_error &e)
	{
		std::cerr << "Error accessing " << dir << ": " << e.what() << '\n';
	}
}

void DuplicateFinder::print_duplicates()
{
	for (const auto &group : duplicates_)
	{
		if (group.second.size() > 1)
		{
			for (const auto &file : group.second)
			{
				std::cout << file.path.string() << '\n';
			}
			std::cout << '\n';
		}
	}
}

bool DuplicateFinder::is_excluded(const fs::path &path, const std::vector<std::string> &excluded_dirs)
{
	std::string path_str = path.string();
	for (const auto &excluded : excluded_dirs)
	{
		if (path_str.find(excluded) == 0)
		{
			return true;
		}
	}
	return false;
}

bool DuplicateFinder::matches_mask(const fs::path &file)
{
	if (masks_.empty() || masks_[0] == "*")
		return true;

	std::string filename = file.filename().string();
	boost::algorithm::to_lower(filename);

	for (const auto &mask : masks_)
	{
		std::string lower_mask = mask;
		boost::algorithm::to_lower(lower_mask);

		if (filename.find(lower_mask) != std::string::npos)
		{
			return true;
		}
	}
	return false;
}

void DuplicateFinder::process_file(const fs::path &filepath)
{
	try
	{
		size_t file_size = fs::file_size(filepath);
		if (file_size >= min_size_ && matches_mask(filepath))
		{
			FileInfo file_info{filepath, file_size, {}};

			// Сначала группируем по размеру
			auto &size_group = size_groups_[file_size];

			// Для каждого файла с таким же размером проверяем дубликатность
			for (auto &candidate : size_group)
			{
				if (compare_files(file_info, candidate))
				{
					// Нашли дубликат - добавляем в группу
					duplicates_[candidate.block_hashes].push_back(candidate);
					duplicates_[candidate.block_hashes].push_back(file_info);
					return;
				}
			}

			// Не нашли дубликатов - добавляем в группу по размеру
			size_group.push_back(file_info);
		}
	}
	catch (const fs::filesystem_error &e)
	{
		std::cerr << "Error processing " << filepath << ": " << e.what() << '\n';
	}
}

bool DuplicateFinder::compare_files(FileInfo &new_file, FileInfo &existing_file)
{
	// Если хеши ещё не вычислены для существующего файла
	if (existing_file.block_hashes.empty())
	{
		existing_file.block_hashes = hasher_.compute_file_hashes(existing_file.path);
	}

	// Вычисляем хеши для нового файла только при необходимости
	new_file.block_hashes = hasher_.compute_file_hashes(new_file.path);

	return new_file.block_hashes == existing_file.block_hashes;
}
