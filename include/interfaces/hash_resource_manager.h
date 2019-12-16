#ifndef INCLUDE_INTERFACES_HASH_RESOURCE_MANAGER_H
#define INCLUDE_INTERFACES_HASH_RESOURCE_MANAGER_H

#include "utils/include/types.h"

namespace hash_resource_manager
{
/**
 * @brief Class HashResourceManager handles hashes.
 */
class HashResourceManager
{
  public:
    /**
     * @brief Inserts hash into the internal hash storage.
     *
     * @param hash Hash that should be kept.
     */
    virtual void
    InsertHash(const types::hash_generator::HashContainer & hash) = 0;

    /**
     * @brief Checks is the internal hash storage empty.
     *
     * @return TRUE if the internal data storage is empty, otherwise
     * FALSE.
     */
    virtual bool IsHashStorageEmpty() const = 0;

    /**
     * @brief Gets hash from the internal hash storage.
     *
     * @param out_hash_container Hash container that will be filled by hash.
     * If the hash container contains a data, it will be re-written.
     *
     * @return TRUE if hash container has been filled by hash
     * successfully, otherwise FALSE.
     */
    virtual bool
    GetHash(types::hash_generator::HashContainer & out_hash_container) = 0;

    /**
     * @brief Gets the last hash ID.
     *
     * @return ID of the last hash in the storage.
     */
    virtual std::size_t GetLastHashId() const = 0;
    virtual ~HashResourceManager() {}
};
} // namespace hash_resource_manager

#endif // INCLUDE_INTERFACES_HASH_RESOURCE_MANAGER_H
