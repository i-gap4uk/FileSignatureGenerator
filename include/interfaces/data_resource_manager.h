#ifndef INCLUDE_INTERFACES_DATA_RESOURCE_MANAGER_H
#define INCLUDE_INTERFACES_DATA_RESOURCE_MANAGER_H

#include "utils/include/types.h"

namespace data_resource_manager
{
/**
 * @brief Class DataResourceManager handles data blocks.
 */
class DataResourceManager
{
  public:
    /**
     * @brief Inserts data block into the internal data storage.
     *
     * @param data_block Data block that should be kept.
     */
    virtual void
    InsertDataBlock(const types::file_manager::DataContainer & data_block) = 0;

    /**
     * @brief Checks is the internal data storage empty.
     *
     * @return TRUE if the internal data storage is empty, otherwise
     * FALSE.
     */
    virtual bool IsDataStorageEmpty() const = 0;

    /**
     * @brief Gets data block from the internal data storage.
     *
     * @param out_data_block Data container that will be filled by data.
     * If the data container contains a data, it will be re-written.
     *
     * @return TRUE if data container has been filled by data
     * successfully, otherwise FALSE.
     */
    virtual bool
    GetDataBlock(types::file_manager::DataContainer & out_data_block) = 0;

    /**
     * @brief Gets the last data block ID.
     *
     * @return ID of the last data block in the storage.
     */
    virtual std::size_t GetLastBlockId() const = 0;

    virtual ~DataResourceManager() {}
};
} // namespace data_resource_manager

#endif // INCLUDE_INTERFACES_DATA_RESOURCE_MANAGER_H
