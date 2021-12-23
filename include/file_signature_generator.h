#ifndef FILE_SIGNATURE_GENERATOR
#define FILE_SIGNATURE_GENERATOR

#include "file_signature_handler.h"

namespace file_signature_generator {
/**
 * @brief Class FileSignatureGenerator generates a file signature and writes it into a file.
 */
class FileSignatureGenerator {
 public:
  FileSignatureGenerator(int args, const char* argv[]);

  ~FileSignatureGenerator() = default;

  /**
   * @brief Starts file's signature generating process.
   * @return Result code as a result of signature generation process.
   * common::Result::ERROR - file signature generating is failed.
   * common::Result::SIGN_IS_GENERATED - file signature generating is completed
   * successfully.
   */
  common::Result Generate();

 private:
  file_signature_handler::FileSignatureHandler file_signature_handler_;
};
}  // namespace file_signature_generator

#endif  // FILE_SIGNATURE_GENERATOR
