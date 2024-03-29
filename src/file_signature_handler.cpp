#include "include/file_signature_handler.h"

#include <iostream>

#include "include/file_manager_impl.h"
#include "include/resource_manager_impl.h"
#include "utils/include/constants.h"
#include "utils/include/hash_generator.h"

namespace {

class DataBlockIDComparator {
 public:
  DataBlockIDComparator(const std::size_t current_block_id,
                        resource_manager::ResourceManager& resource_manager)
      : current_data_block_id_(current_block_id), resource_manager_(resource_manager) {}

  bool operator()() {
    return current_data_block_id_ == (resource_manager_.GetLastBlockId() + 1);
  }

 private:
  const std::size_t current_data_block_id_;
  resource_manager::ResourceManager& resource_manager_;
};

class HashBlockIDComparator {
 public:
  HashBlockIDComparator(const std::size_t current_hash_id,
                        resource_manager::ResourceManager& resource_manager)
      : current_hash_id_(current_hash_id), resource_manager_(resource_manager) {}

  bool operator()() {
    return current_hash_id_ == (resource_manager_.GetLastHashId() + 1);
  }

 private:
  const std::size_t current_hash_id_;
  resource_manager::ResourceManager& resource_manager_;
};
}  // namespace

namespace file_signature_handler {

FileSignatureHandler::FileSignatureHandler(const common::InitializeParameters& init_params)
    : block_size_(common::InitializeParameters::kInvalidSize == init_params.block_size
                      ? constants::DEFAULT_BLOCK_SIZE
                      : init_params.block_size),
      file_manager_(std::make_shared<file_manager::FileManagerImpl>(
          init_params.source_file_path, init_params.signature_file_path)),
      resource_manager_(std::make_shared<resource_manager::ResourceManagerImpl>()),
      data_reading_is_completed_(false),
      hash_generating_is_completed_(false),
      end_of_data_(false),
      end_of_hashes_(false),
      data_is_written_(false),
      max_thread_count_(constants::DEFAULT_THREADS_COUNT) {}

common::Result FileSignatureHandler::GenerateFileSignature() {
  if (max_thread_count_ < 3) {
    common::DataContainer data_container(block_size_);
    while (!end_of_data_) {
      auto read_result = file_manager_->ReadDataBlock(data_container);

      common::HashContainer hash_container;
      if (!data_container.data.empty()) {
        hash_generator_.GenerateHash(data_container.data, hash_container);
      } else {
        return common::Result::ERROR;
      }

      file_manager_->WriteHashToFile(hash_container.hash);
      if (common::Result::END_OF_FILE == read_result) {
        end_of_data_ = true;
      }
    }
    return common::Result::SIGN_IS_GENERATED;
  }

  RunThreads();
  std::unique_lock<std::mutex> lock(sign_is_written_mutex_);
  auto signature_handler = [this]() {
    return (end_of_data_ && end_of_hashes_ && data_is_written_);
  };
  signature_is_written_cv_.wait(lock, signature_handler);
  std::cout << "Generating is completed\n";
  return common::Result::SIGN_IS_GENERATED;
}

bool FileSignatureHandler::Init() {
  if (!file_manager_->Init()) {
    std::cerr << "FileManager initialization is failed!\n";
    return false;
  }
  InitThreads();

  return true;
}

void FileSignatureHandler::InitThreads() {
  // Book one thread for writing and leave another one for systems needs
  max_thread_count_ = std::thread::hardware_concurrency() - 2;

  std::size_t reading_threads = max_thread_count_ / 2;
  std::size_t hash_generating_threads = max_thread_count_ - reading_threads;
  for (std::size_t i = 0; i < reading_threads; ++i) {
    reading_threads_.emplace_back(&FileSignatureHandler::ReadData, this);
  }

  for (std::size_t i = 0; i < hash_generating_threads; ++i) {
    hash_generating_threads_.emplace_back(&FileSignatureHandler::GenerateHash, this);
  }

  writing_hash_thread_.reset(new std::thread(&FileSignatureHandler::WriteHash, this));
}

void FileSignatureHandler::RunThreads() {
  std::cout << "Will be started " << max_thread_count_ << " threads\n";
  for (auto& thread : reading_threads_) {
    thread.join();
  }

  for (auto& thread : hash_generating_threads_) {
    thread.join();
  }

  if (writing_hash_thread_) {
    writing_hash_thread_->join();
  }
}

void FileSignatureHandler::ReadData() {
  std::cout << "Data reading thread[ " << std::this_thread::get_id() << "] is started\n";
  while (true) {
    if (data_reading_is_completed_) {
      std::cout << "Data reading thread [" << std::this_thread::get_id() << "] is completed work\n";
      return;
    }

    common::DataContainer data_container(block_size_);

    const common::Result read_result = ReadDataBlock(data_container);

    switch (read_result) {
      case common::Result::ERROR: {
        return;
      }
      case common::Result::END_OF_FILE: {
        data_reading_is_completed_ = true;
        std::cout << "The last data block [" << data_container.id
                  << "] has been read succssesfully\n";
      } break;
      case common::Result::DATA_IS_READ: {
        std::cout << "Data block [" << data_container.id << "] has been read succssesfully\n";
      } break;
      default:
        std::cout << "Unexpected result: " << read_result << std::endl;
        return;
    }
    PutDataBlock(data_container);
  }
}

void FileSignatureHandler::GenerateHash() {
  std::cout << "Hash generating thread[ " << std::this_thread::get_id() << "] is started\n";
  while (true) {
    if (hash_generating_is_completed_) {
      std::cout << "Hash generating thread [" << std::this_thread::get_id()
                << "] is completed work\n";
      return;
    }

    WaitAvailableDataBlock();

    common::DataContainer data_container;
    const bool result = GetDataBlock(data_container);

    if (result) {
      common::HashContainer hash_container;
      hash_generator_.GenerateHash(data_container.data, hash_container);
      std::cout << "Hash [" << hash_container.id << "] is generated successfully" << std::endl;
      PutHash(hash_container);
      if (end_of_data_ &&
          (resource_manager_->GetLastBlockId() == resource_manager_->GetLastHashId())) {
        hash_generating_is_completed_ = true;
      }

      write_data_cv_.notify_all();
    }
  }
}

void FileSignatureHandler::WriteHash() {
  std::cout << "Writing thread [ " << std::this_thread::get_id() << "] is started\n";
  while (true) {
    if (data_is_written_) {
      std::cout << "Writing thread [" << std::this_thread::get_id() << "] is completed work\n";
      signature_is_written_cv_.notify_all();
      return;
    }

    WaitAvailableHash();

    common::HashContainer hash_container;
    bool hasah_getting_result = GetHash(hash_container);

    if (hasah_getting_result) {
      const bool read_result = file_manager_->WriteHashToFile(hash_container.hash);

      if (read_result) {
        std::cout << "Hash [" << hash_container.id << "] has been written successfully\n";
      } else {
        std::cerr << "Hash " << hash_container.id << " writing is failed\n";
      }

      if (end_of_data_ && end_of_hashes_) {
        data_is_written_ = true;
      }
    } else {
      std::cout << "Hash queue is empty!\n";
    }
  }
}

common::Result FileSignatureHandler::ReadDataBlock(common::DataContainer& data_container) {
  common::Result result = common::Result::ERROR;

  result = file_manager_->ReadDataBlock(data_container);
  return result;
}

void FileSignatureHandler::PutDataBlock(const common::DataContainer& data_container) {
  if (data_container.id != (resource_manager_->GetLastBlockId() + 1)) {
    DataBlockIDComparator dbc(data_container.id, *resource_manager_);
    std::unique_lock<std::mutex> lock(push_data_mutex_);
    push_data_cv_.wait(lock, dbc);
  }

  resource_manager_->InsertDataBlock(data_container);
  push_data_cv_.notify_all();
}

void FileSignatureHandler::WaitAvailableDataBlock() {
  if (resource_manager_->IsDataStorageEmpty() && !data_reading_is_completed_) {
    std::unique_lock<std::mutex> lock(push_data_mutex_);
    auto data_handler = [this]() {
      return !resource_manager_->IsDataStorageEmpty() || end_of_data_;
    };
    push_data_cv_.wait(lock, data_handler);
  }
}

void FileSignatureHandler::WaitAvailableHash() {
  if (resource_manager_->IsHashStorageEmpty() && !end_of_hashes_) {
    std::unique_lock<std::mutex> write_lock(push_hash_mutex_);
    auto hash_handler = [this]() { return !resource_manager_->IsHashStorageEmpty(); };

    write_data_cv_.wait(write_lock, hash_handler);
  }
}

bool FileSignatureHandler::GetHash(common::HashContainer& hash_container) {
  const bool result = resource_manager_->GetHash(hash_container);
  if (hash_generating_is_completed_ && resource_manager_->IsHashStorageEmpty()) {
    end_of_hashes_ = true;
  }

  return result;
}

bool FileSignatureHandler::GetDataBlock(common::DataContainer& data_container) {
  const bool result = resource_manager_->GetDataBlock(data_container);
  if (resource_manager_->IsDataStorageEmpty() && data_reading_is_completed_) {
    end_of_data_ = true;
  }
  return result;
}

void FileSignatureHandler::PutHash(const common::HashContainer& hash_container) {
  if (hash_container.id != (resource_manager_->GetLastHashId() + 1)) {
    std::unique_lock<std::mutex> hash_lock(push_hash_mutex_);
    HashBlockIDComparator hb_c(hash_container.id, *resource_manager_);
    push_hash_cv_.wait(hash_lock, hb_c);
  }

  resource_manager_->InsertHash(hash_container);
  push_hash_cv_.notify_all();
}

}  // namespace file_signature_handler
