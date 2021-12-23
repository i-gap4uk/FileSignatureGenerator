#include "common.h"

namespace common{
InitializeParameters ParseCommandLineArguments(int args, const char* argv[]) {
  InitializeParameters init_params;
  switch (args) {
    case 2: {
      init_params.source_file_path = argv[constants::ARGUMENT::INPUT_FILE];
    } break;
    case 3: {
      init_params.source_file_path = argv[constants::ARGUMENT::INPUT_FILE];
      init_params.signature_file_path = argv[constants::ARGUMENT::SIGNATURE_FILE];
    } break;
    case 4: {
      init_params.block_size =
          static_cast<std::size_t>(std::atoi(argv[constants::ARGUMENT::BLOCK_SIZE]));
      init_params.source_file_path = argv[constants::ARGUMENT::INPUT_FILE];
      init_params.signature_file_path = argv[constants::ARGUMENT::SIGNATURE_FILE];
    } break;
    default:
      std::cout << "Incorrect arguments count!\n";
  }
  return init_params;
}
} // namespace common
