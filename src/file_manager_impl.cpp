#include "include/file_manager_impl.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>

namespace file_manager {

FileManagerImpl::FileManagerImpl(const std::string& output_file, const std::string& input_file)
    : file_size_(0),
      already_read_size_(0),
      data_block_id_(0),
      input_file_path_(input_file),
      output_file_path_(output_file)

{}

bool FileManagerImpl::Init() {
  try {
    if (!OpenInputFile()) {
      std::cerr << "Input file opening is failed!\n" << std::endl;
      return false;
    }
    std::cout << "The input file is open successfully." << std::endl;

    CalculateFileSize();

    if (!OpenOutputFile()) {
      std::cerr << "Output file opening is failed!\n" << std::endl;
      return false;
    }
    std::cout << "The uotput file is open successfully." << std::endl;
  } catch (std::exception e) {
    std::cerr << "Opening file is failed!" << std::endl;
    std::cerr << e.what() << std::endl;
    return false;
  }

  return true;
}

common_types::Result FileManagerImpl::ReadDataBlock(common_types::DataContainer& data_block) {
  std::lock_guard<std::mutex> local_lock(read_file_mutex_);
  try {
    if (already_read_size_ == file_size_) {
      return common_types::Result::ERROR;
    }

    data_block.id = ++data_block_id_;

    const std::size_t remined_size = file_size_ - already_read_size_;
    std::size_t block_size = data_block.data.size();
    if (remined_size < block_size) {
      // The case when file data size less than the specified block
      // size. Data block should have size as size for hashing block and
      // remain space should be filled by zero.

      std::fill(data_block.data.begin(), data_block.data.end(), 0);
      input_stream_.read(&(data_block.data[0]), static_cast<long>(remined_size));
      std::cout << "Block count: " << data_block_id_ << std::endl;
      already_read_size_ += static_cast<std::size_t>(input_stream_.gcount());
      return common_types::Result::END_OF_FILE;
    }

    input_stream_.read(&(data_block.data[0]), static_cast<long>(data_block.data.size()));

    already_read_size_ += static_cast<std::size_t>(input_stream_.gcount());
    return common_types::Result::DATA_IS_READ;
  } catch (std::exception e) {
    std::cerr << "Reading file is failed!\n" << std::endl;
    std::cerr << e.what();
    return common_types::Result::ERROR;
  }
}

bool FileManagerImpl::WriteHashToFile(const std::string& hash_string) {
  std::lock_guard<std::mutex> lock(write_file_mutex_);
  try {
    long buffer_size = static_cast<long>(hash_string.size());
    output_steam_.write(hash_string.c_str(), buffer_size);
    return true;
  } catch (std::exception e) {
    std::cerr << "Writing file is failed!\n";
    std::cerr << e.what();
    return false;
  }
}

FileManagerImpl::~FileManagerImpl() {
  CloseInputFile();
  CloseOutputFile();
}

bool FileManagerImpl::OpenInputFile() {
  input_stream_.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  input_stream_.open(input_file_path_, std::ifstream::binary);
  return input_stream_.is_open();
}

bool FileManagerImpl::OpenOutputFile() {
  output_steam_.exceptions(std::ofstream::failbit | std::ofstream::badbit);
  output_steam_.open(output_file_path_, std::ofstream::out | std::ofstream::trunc);
  return output_steam_.is_open();
}

void FileManagerImpl::CloseInputFile() {
  if (input_stream_.is_open()) {
    input_stream_.close();
    std::cout << "Input file is closed." << std::endl;
  }
}

void FileManagerImpl::CloseOutputFile() {
  if (output_steam_.is_open()) {
    output_steam_.close();
    std::cout << "Output file is closed." << std::endl;
  }
}

void FileManagerImpl::CalculateFileSize() {
  input_stream_.seekg(0, input_stream_.end);
  file_size_ = static_cast<std::size_t>(input_stream_.tellg());
  input_stream_.seekg(0, input_stream_.beg);
}

}  // namespace file_manager
