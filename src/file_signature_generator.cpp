#include "file_signature_generator.h"

#include <iostream>

#include "utils/include/common.h"

namespace file_signature_generator {
using FileSignatureHandler = file_signature_handler::FileSignatureHandler;

FileSignatureGenerator::FileSignatureGenerator(int args, const char* argv[])
    : file_signature_handler_(common::ParseCommandLineArguments(args, argv)) {}

common::Result FileSignatureGenerator::Generate() {
  if (!file_signature_handler_.Init()) {
    std::cerr << "FileSignatureHandler initialization is failed!\n";
    return common::Result::ERROR;
  }

  return file_signature_handler_.GenerateFileSignature();
}

}  // namespace file_signature_generator
