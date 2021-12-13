#ifndef UTILS_CONSTANTS_H
#define UTILS_CONSTANTS_H

#include <string>

namespace constants
{

/**
 *@brief Command line argument.
 */
enum ARGUMENT
{
    INPUT_FILE = 1, /**< The index of the input file path as a first command
                       line argument */
    SIGNATURE_FILE, /**< The index of the signature file path as a second
                       command line argiment */
    BLOCK_SIZE      /**< The data block size as a third command line argument */
};
} // namespace constants

#endif // UTILS_CONSTANTS_H
