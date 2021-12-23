#include "utils/include/hash_generator.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#include <openssl/evp.h>

#include "utils/include/common.h"

namespace hash_generator {

using Byte = unsigned char;
using MD5Buffer = std::vector<Byte>;

/**
 * @brief Converts hash value to hex value.
 *
 * @param hash_value Hash value as array of bytes (in digits representation).
 * @param hash_size The size of the hash value.
 * @param out_hash_string Reference to the string which will contain in the hex
 * representation.
 */
void hex_format(const Byte* hash_value, const std::size_t hash_size, std::string& out_hash_string) {
  std::stringstream sstream;
  for (std::size_t i = 0; i < hash_size; ++i) {
    sstream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash_value[i]);
  }
  out_hash_string = sstream.str();
}

HashGenerator::HashGenerator() : hash_id_(common::HashContainer::kInvalidId) {}

bool HashGenerator::GenerateHash(const Data& data, common::HashContainer& out_hash_container) {
  EVP_MD_CTX mctx;
  MD5Buffer md5_buffer(EVP_MAX_IV_LENGTH);
  EVP_MD_CTX_init(&mctx);

  unsigned int read_length = 0;

  int error = 1;

  error = EVP_DigestInit(&mctx, EVP_md5());
  if (0 == error) {
    std::cout << "DigestInit is failed!" << std::endl;
    return false;
  }

  error = EVP_DigestUpdate(&mctx, &(data[0]), data.size());
  if (0 == error) {
    std::cout << "DigestUpdate is failed!" << std::endl;
    return false;
  }

  error = EVP_DigestFinal(&mctx, &(md5_buffer[0]), &read_length);
  if (0 == error) {
    std::cout << "DigestFinal is failed!" << std::endl;
    return false;
  }

  error = EVP_MD_CTX_cleanup(&mctx);
  if (0 == error) {
    std::cout << "CTX CleanUp is failed!" << std::endl;
    return false;
  }

  {
    std::lock_guard<std::mutex> lock(hash_id_mutex_);
    hex_format(&(md5_buffer[0]), md5_buffer.size(), out_hash_container.hash);
    out_hash_container.id = ++hash_id_;
  }

  return true;
}

}  // namespace hash_generator
