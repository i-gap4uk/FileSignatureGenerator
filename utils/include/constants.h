#ifndef UTILS_CONSTANTS_H
#define UTILS_CONSTANTS_H

#include <string>

namespace constants
{
const std::string DEFAULT_SIGNATURE_FILE = "file_signature.txt";
const std::size_t DEFAULT_BLOCK_SIZE = 1048576; // 1Mb
const std::size_t DEFAULT_THREADS_COUNT = 1;

/**
 *@brief Command line argument.
 */
enum ARGUMENT
{
    INPUT_FILE = 1, /**< The index of the input file path as a first command
                       line argiment */
    SIGNATURE_FILE, /**< The index of the signature file path as a second
                       command line argiment */
    BLOCK_SIZE      /**< The data block size as a third command line argument */
};
} // namespace constants

#endif // UTILS_CONSTANTS_H
