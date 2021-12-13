#ifndef INCLUDE_FILE_SIGNATURE_GENERATOR
#define INCLUDE_FILE_SIGNATURE_GENERATOR

#include <string>

#include "file_signature_handler.h"
#include "utils/include/common_types.h"

namespace file_signature_handler
{
class FileSignatureHandler;
} // namespace file_signature_handler

namespace file_signature_generator
{

/**
 * @brief Class FileSignatureGenerator generates a file signature and writes it
 * into a file.
 */
class FileSignatureGenerator
{
  public:
    FileSignatureGenerator(const std::string & input_file,
                           const std::string & signature_file,
                           const std::size_t block_size,
                           const std::size_t threads_count);
    ~FileSignatureGenerator();

    /**
     * @brief Starts file signature generating process.
     *
     * @return Result code as a result of signature generation process.
     * common_types::Result::ERROR - file signature generating is failed.
     * common_types::Result::SIGN_IS_GENERATED - file signature generating is completed
     * successfully.
     */
    common_types::Result Run();

  private:
    file_signature_handler::FileSignatureHandler file_signature_handler_;
};

} // namespace file_signature_generator

#endif // INCLUDE_FILE_SIGNATURE_GENERATOR
