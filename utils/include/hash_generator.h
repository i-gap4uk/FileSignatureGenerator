#ifndef HASH_GENERATOR_H
#define HASH_GENERATOR_H

#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "utils/include/common.h"

namespace hash_generator {

using Data = std::vector<char>;

/**
 * @brief Class HashGenerator generates a hash value from data. Hash represents
 * a string in the HEX format.
 */
class HashGenerator {
 public:
  HashGenerator();

  /**
   * @brief Generates hash from a specified data.
   * @param data_container Data container which contains data as a source for
   * a hash. In case the data container contains data, it will be rewritten.
   * @param out_hash_container Hash container which will contain a generated
   * hash.
   * @return TRUE if hash generating completed successfully, otherwise FALSE.
   */
  bool GenerateHash(const Data& data, common::HashContainer& out_hash_container);

 private:
  std::size_t hash_id_;
  std::mutex hash_id_mutex_;
};

}  // namespace hash_generator

#endif  // HASH_GENERATOR_H
