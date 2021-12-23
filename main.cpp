#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "include/file_signature_generator.h"
#include "utils/include/constants.h"

std::string timeToString(const std::chrono::duration<double>& time) {
  std::stringstream ss;
  auto hours = std::chrono::duration_cast<std::chrono::hours>(time);
  auto minutes = std::chrono::duration_cast<std::chrono::minutes>(time);
  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time);
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time);

  ss << std::setw(2) << std::setfill('0') << hours.count() % 24 << std::right << ":" << std::setw(2)
     << std::setfill('0') << minutes.count() % 60 << std::right << ":" << std::setw(2)
     << std::setfill('0') << seconds.count() % 60 << std::right << ":" << std::setw(3)
     << milliseconds.count() % 1000 << std::right << std::endl;
  return ss.str();
}

int main(int args, const char* argv[]) {
  auto start = std::chrono::high_resolution_clock::now();

  common::Result result_code = common::Result::ERROR;
  file_signature_generator::FileSignatureGenerator file_sign_generator(args, argv);

  result_code = file_sign_generator.Generate();

  if (common::Result::ERROR == result_code) {
    std::cerr << "Signature generation is failed!\n";
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto diff = end - start;

  std::cout << std::endl << "Execute time: " << timeToString(diff) << std::endl;
}
