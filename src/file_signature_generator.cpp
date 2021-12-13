#include "file_signature_generator.h"

#include <iostream>

namespace file_signature_generator
{
using FileSignatureHandler = file_signature_handler::FileSignatureHandler;

FileSignatureGenerator::FileSignatureGenerator(
    const std::string & input_file, const std::string & signature_file,
    const std::size_t block_size, const std::size_t threads_count)
    : file_signature_handler_(input_file, signature_file, block_size,
                              threads_count)
{
}

FileSignatureGenerator::~FileSignatureGenerator() {}

common_types::Result FileSignatureGenerator::Run()
{
    const bool init_result = file_signature_handler_.Init();
    if (!init_result)
    {
        std::cerr << "FileSignatureHandler initialization is failed!\n";
        return common_types::Result::ERROR;
    }

    return file_signature_handler_.GenerateFileSignature();
}

} // namespace file_signature_generator
