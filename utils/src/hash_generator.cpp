#include <iomanip>
#include <iostream>
#include <openssl/evp.h>
#include <sstream>

#include "utils/include/hash_generator.h"
#include "utils/include/types.h"

namespace hash_generator
{

/**
 * @brief Converts hash value to hex value.
 *
 * @param hash_value Hash value as array of bytes (in digits representation).
 * @param hash_size The size of the hash value.
 * @param out_hash_string Reference to the string which will contain in the hex
 * representation.
 */
void hex_format(const types::hash_generator::byte * hash_value,
                const std::size_t hash_size, std::string & out_hash_string)
{
    std::stringstream sstream;
    for (std::size_t i = 0; i < hash_size; ++i)
    {
        sstream << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(hash_value[i]);
    }
    out_hash_string = sstream.str();
}

HashGenerator::HashGenerator() : hash_id_(0) {}

bool HashGenerator::GenerateHash(
    const types::file_manager::DataContainer & data_container,
    types::hash_generator::HashContainer & out_hash_container)
{
    std::lock_guard<std::mutex> lock(hash_id_mutex_);
    EVP_MD_CTX mctx;
    types::hash_generator::DataContainer md5_buffer(EVP_MAX_IV_LENGTH);
    EVP_MD_CTX_init(&mctx);

    unsigned int read_length = 0;

    int error = 1;

    error = EVP_DigestInit(&mctx, EVP_md5());
    if (0 == error)
    {
        std::cout << "DigestInit is failed!" << std::endl;
        return false;
    }

    error = EVP_DigestUpdate(&mctx, &(data_container.data[0]),
                             data_container.data.size());
    if (0 == error)
    {
        std::cout << "DigestUpdate is failed!" << std::endl;
        return false;
    }

    error = EVP_DigestFinal(&mctx, &(md5_buffer[0]), &read_length);
    if (0 == error)
    {
        std::cout << "DigestFinal is failed!" << std::endl;
        return false;
    }

    error = EVP_MD_CTX_cleanup(&mctx);
    if (0 == error)
    {
        std::cout << "CTX CleanUp is failed!" << std::endl;
        return false;
    }

    hex_format(&(md5_buffer[0]), md5_buffer.size(), out_hash_container.hash);

    out_hash_container.id = ++hash_id_;
    return true;
}

} // namespace hash_generator
