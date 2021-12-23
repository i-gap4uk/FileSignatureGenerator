/**
 * Developed by Igor Gapchuk
 * 2019
 */

#ifndef INCLUDE_FILE_MANGER_H
#define INCLUDE_FILE_MANGER_H

#include <memory>
#include <string>
#include <vector>

#include "utils/include/common.h"

namespace file_manager {

/**
 * @brief Class FileManager reads a data from a file and writes a hash into another one file.
 */
class FileManager {
 public:
  virtual bool Init() = 0;

  /**
   * @brief Reads data from a file by data block size.
   * @param data_block A data block that will be filled by data.
   * @return Result code as the result of operation.
   */
  virtual common::Result ReadDataBlock(common::DataContainer& data_block) = 0;

  /**
   * @brief Writes hash into a file.
   * @param hash_string Hash that should be written into the file.
   * @return TRUE if hash has been written successfully, otherwise FALSE.
   */
  virtual bool WriteHashToFile(const std::string& hash_string) = 0;

  virtual ~FileManager() {}
};
}  // namespace file_manager

#endif  // INCLUDE_FILE_MANGER_H
