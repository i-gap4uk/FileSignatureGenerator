#ifndef INCLUDE_UTILS_TYPES_H
#define INCLUDE_UTILS_TYPES_H

#include <limits>
#include <set>
#include <string>
#include <vector>

namespace common_types {

/**
 * @brief DataContainer keeps data and his own identifier.
 */
struct DataContainer {
  static constexpr std::size_t kInvalidId = std::numeric_limits<std::size_t>::max();

  explicit DataContainer(const std::size_t block_size = 0) : data(0, block_size), id(kInvalidId) {}

  bool operator<(const DataContainer& other) const {
    return id < other.id;
  }

  std::vector<char> data;
  std::size_t id;
};

using DataQueue = std::set<common_types::DataContainer>;

/**
 * Structure HashContainer keeps hash and his own identifier.
 */
struct HashContainer {
  static constexpr std::size_t kInvalidId = std::numeric_limits<std::size_t>::max();

  HashContainer() : hash({}), id(kInvalidId) {}

  bool operator<(const HashContainer& other) const {
    return id < other.id;
  }

  std::string hash;
  std::size_t id;
};

using HashQueue = std::set<common_types::HashContainer>;

enum Result {
  INVALID_ENUM = -1,
  DATA_IS_READ,      /**< The data block is read successfully and reading is not
                        completed yet. */
  END_OF_FILE,       /**< The end of file is reached */
  SIGN_IS_GENERATED, /**< The file signature is generated and written into the
                        file.*/
  SUCCESS,
  ERROR
};

}  // namespace common_types

#endif  // INCLUDE_UTILS_TYPES_H
