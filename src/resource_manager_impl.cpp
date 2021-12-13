#include "include/resource_manager_impl.h"

namespace resource_manager {

ResourceManagerImpl::ResourceManagerImpl()
    : last_data_block_id_(DataContainer::kInvalidId), last_hash_id_(HashContainer::kInvalidId) {}

ResourceManagerImpl::~ResourceManagerImpl() {}

void ResourceManagerImpl::InsertDataBlock(const DataContainer& data_block) {
  std::lock_guard<std::mutex> lock(data_queue_mutex_);
  data_queue_.insert(data_block);
  last_data_block_id_ = data_block.id;
}

void ResourceManagerImpl::InsertHash(const HashContainer& hash) {
  std::lock_guard<std::mutex> lock(hash_queue_mutex_);
  hash_queue_.insert(hash);
  last_hash_id_ = hash.id;
}

bool ResourceManagerImpl::IsHashStorageEmpty() const {
  std::lock_guard<std::mutex> lock(hash_queue_mutex_);
  return hash_queue_.empty();
}

bool ResourceManagerImpl::GetHash(HashContainer& hash_container) {
  std::lock_guard<std::mutex> lock(hash_queue_mutex_);
  if (hash_queue_.empty()) {
    return false;
  }

  auto it_begin = hash_queue_.begin();
  hash_container = *it_begin;
  hash_queue_.erase(it_begin);
  return true;
}

std::size_t ResourceManagerImpl::GetLastHashId() const {
  std::lock_guard<std::mutex> lock(hash_queue_mutex_);
  return last_hash_id_;
}

bool ResourceManagerImpl::IsDataStorageEmpty() const {
  std::lock_guard<std::mutex> lock(data_queue_mutex_);
  return data_queue_.empty();
}

bool ResourceManagerImpl::GetDataBlock(DataContainer& data_block) {
  std::lock_guard<std::mutex> lock(data_queue_mutex_);
  if (data_queue_.empty()) {
    return false;
  }

  auto it_begin = data_queue_.begin();
  data_block = *it_begin;
  data_queue_.erase(it_begin);
  return true;
}

std::size_t ResourceManagerImpl::GetLastBlockId() const {
  std::lock_guard<std::mutex> lock(data_queue_mutex_);
  return last_data_block_id_;
}

}  // namespace resource_manager
