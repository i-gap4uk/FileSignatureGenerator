#ifndef INCLUDE_UTILS_TYPES_H
#define INCLUDE_UTILS_TYPES_H

#include <memory>
#include <set>
#include <string>
#include <vector>

namespace types
{

enum Result
{
    INVALID_ENUM = -1,
    DATA_IS_READ, /**< The data block is read successfully and reading is not
                     completed yet. */
    END_OF_FILE,  /**< The end of file is reached */
    SIGN_IS_GENERATED, /**< The file signature is generated and written into the
                          file.*/
    SUCCESS,
    ERROR
};

namespace file_manager
{

/**
 * @brief Structure DataContainer keeps data and his own identifier.
 */
struct DataContainer
{
    DataContainer(const std::size_t block_size = 0) : data(block_size) {}
    std::vector<char> data;
    std::size_t id;

    /**
     * @brief Compares two data containers by their identifiers.
     */
    bool operator<(const DataContainer & other) const { return id < other.id; }
};

using DataQueue = std::set<DataContainer>;
} // namespace file_manager

namespace hash_generator
{
using byte = unsigned char;

/**
 *@brief Wrapper for data for an array of unsigned char
 */
using DataContainer = std::vector<byte>;

/**
 * Structure HashContainer keeps hash and his own identifier.
 */
struct HashContainer
{
    std::string hash;
    std::size_t id;
    /**
     * @brief Compares two hash containers by their identifiers.
     */
    bool operator<(const HashContainer & other) const { return id < other.id; }
};

using HashQueue = std::set<HashContainer>;
} // namespace hash_generator

} // namespace types

#endif // INCLUDE_UTILS_TYPES_H
