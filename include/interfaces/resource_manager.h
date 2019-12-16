#ifndef INCLUDE_INTERFACES_RESOURCCE_MANAGER_H
#define INCLUDE_INTERFACES_RESOURCCE_MANAGER_H

#include "include/interfaces/data_resource_manager.h"
#include "include/interfaces/hash_resource_manager.h"

namespace resource_manager
{

class ResourceManager : public data_resource_manager::DataResourceManager,
                        public hash_resource_manager::HashResourceManager
{
  public:
    virtual void InsertDataBlock(
        const types::file_manager::DataContainer & data_block) = 0;
    virtual bool IsDataStorageEmpty() const = 0;
    virtual bool
    GetDataBlock(types::file_manager::DataContainer & data_block) = 0;
    virtual std::size_t GetLastBlockId() const = 0;

    virtual void
    InsertHash(const types::hash_generator::HashContainer & hash) = 0;
    virtual bool IsHashStorageEmpty() const = 0;
    virtual bool
    GetHash(types::hash_generator::HashContainer & hash_container) = 0;
    virtual std::size_t GetLastHashId() const = 0;

    virtual ~ResourceManager() {}
};

} // namespace resource_manager

#endif // INCLUDE_INTERFACES_RESOURCCE_MANAGER_H
