#ifndef INCLUDE_INTERFACES_RESOURCE_MANAGER_H
#define INCLUDE_INTERFACES_RESOURCE_MANAGER_H

#include "include/interfaces/data_resource_manager.h"
#include "include/interfaces/hash_resource_manager.h"

namespace resource_manager {

using DataContainer = common::DataContainer;
using HashContainer = common::HashContainer;

class ResourceManager : public data_resource_manager::IDataResourceManager,
                        public hash_resource_manager::IHashResourceManager {
 public:
  void InsertDataBlock(const DataContainer& data_block) override = 0;
  bool IsDataStorageEmpty() const override = 0;
  bool GetDataBlock(DataContainer& data_block) override = 0;
  std::size_t GetLastBlockId() const override = 0;

  void InsertHash(const HashContainer& hash) override = 0;
  bool IsHashStorageEmpty() const override = 0;
  bool GetHash(HashContainer& hash_container) override = 0;
  std::size_t GetLastHashId() const override = 0;

  ~ResourceManager() override {}
};

}  // namespace resource_manager

#endif  // INCLUDE_INTERFACES_RESOURCE_MANAGER_H
