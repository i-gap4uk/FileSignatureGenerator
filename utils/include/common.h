#ifndef COMMON
#define COMMON

#include <iostream>
#include <limits>
#include <set>
#include <string>
#include <vector>

#include "constants.h"

namespace common {
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

using DataQueue = std::set<DataContainer>;

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

using HashQueue = std::set<HashContainer>;

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

struct InitializeParameters {
  static constexpr std::size_t kInvalidSize = std::numeric_limits<std::size_t>::max();

  std::string source_file_path;
  std::string signature_file_path;
  std::size_t block_size{kInvalidSize};
};

InitializeParameters ParseCommandLineArguments(int args, const char* argv[]);

}  // namespace common

#endif  // COMMON
