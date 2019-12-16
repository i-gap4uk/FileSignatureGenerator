#include <chrono>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "include/file_signature_generator.h"
#include "utils/include/constants.h"

std::string timeToString(const std::chrono::duration<double> & time)
{
    std::stringstream ss;
    auto hours = std::chrono::duration_cast<std::chrono::hours>(time);
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(time);
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time);
    auto milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(time);

    ss << std::setw(2) << std::setfill('0') << hours.count() % 24 << std::right
       << ":" << std::setw(2) << std::setfill('0') << minutes.count() % 60
       << std::right << ":" << std::setw(2) << std::setfill('0')
       << seconds.count() % 60 << std::right << ":" << std::setw(3)
       << milliseconds.count() % 1000 << std::right << std::endl;
    return ss.str();
}

int main(int args, const char * argv[])
{
    auto start = std::chrono::high_resolution_clock::now();

    std::string input_file;
    std::string signature_file = constants::DEFAULT_SIGNATURE_FILE;
    std::size_t block_size = constants::DEFAULT_BLOCK_SIZE;
    std::size_t threads_count = constants::DEFAULT_THREADS_COUNT;

    types::Result result_code = types::Result::ERROR;
    switch (args)
    {
        case 2:
        {
            input_file = argv[constants::ARGUMENT::INPUT_FILE];
        }
        break;
        case 3:
        {
            input_file = argv[constants::ARGUMENT::INPUT_FILE];
            signature_file = argv[constants::ARGUMENT::SIGNATURE_FILE];
        }
        break;
        case 4:
        {
            block_size = static_cast<std::size_t>(
                std::atoi(argv[constants::ARGUMENT::BLOCK_SIZE]));
            input_file = argv[constants::ARGUMENT::INPUT_FILE];
            signature_file = argv[constants::ARGUMENT::SIGNATURE_FILE];
        }
        break;
        case 5:
        {
            block_size = static_cast<std::size_t>(
                std::atoi(argv[constants::ARGUMENT::BLOCK_SIZE]));
            input_file = argv[constants::ARGUMENT::INPUT_FILE];
            signature_file = argv[constants::ARGUMENT::SIGNATURE_FILE];

            // The threads count should be specified by "-j<threads count>" key
            // format. This argument should be specified as 4-th argument.
            std::string threads_argument = argv[4];
            auto threads_count_str =
                threads_argument.substr(2, threads_argument.size() - 1);
            threads_count =
                static_cast<std::size_t>(std::atoi(threads_count_str.c_str()));
        }
        break;
        default:
            std::cerr << "Incorrect arguments count!\n";
            return 1;
    }

    file_signature_generator::FileSignatureGenerator file_sign_generator(
        input_file, signature_file, block_size, threads_count);

    result_code = file_sign_generator.Run();

    if (types::Result::ERROR == result_code)
    {
        std::cerr << "Signature generation is failed!\n";
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto diff = end - start;

    std::cout << std::endl
              << "Execute time: " << timeToString(diff) << std::endl;
}
