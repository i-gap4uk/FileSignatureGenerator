#ifndef SRC_RESOURCE_MANAGER_IMPL_H
#define SRC_RESOURCE_MANAGER_IMPL_H

#include <mutex>

#include "include/interfaces/resource_manager.h"
#include "utils/include/common.h"

namespace resource_manager {
class ResourceManagerImpl : public ResourceManager {
 public:
  ResourceManagerImpl();
  ~ResourceManagerImpl() override;

  void InsertDataBlock(const DataContainer& data_block) override;
  bool IsDataStorageEmpty() const override;
  bool GetDataBlock(DataContainer& data_block) override;
  std::size_t GetLastBlockId() const override;

  void InsertHash(const HashContainer& hash) override;
  bool IsHashStorageEmpty() const override;
  bool GetHash(HashContainer& hash_container) override;
  std::size_t GetLastHashId() const override;

 private:
  common::DataQueue data_queue_;
  common::HashQueue hash_queue_;
  std::size_t last_data_block_id_;
  std::size_t last_hash_id_;

  mutable std::mutex data_queue_mutex_;
  mutable std::mutex hash_queue_mutex_;
};

}  // namespace resource_manager

#endif  // SRC_RESOURCE_MANAGER_IMPL_H
