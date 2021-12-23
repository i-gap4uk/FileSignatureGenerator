#ifndef SRC_FILE_MANAGER_IMPL_H
#define SRC_FILE_MANAGER_IMPL_H

#include <fstream>
#include <mutex>

#include "include/interfaces/file_manager.h"
#include "utils/include/common.h"

namespace file_manager {
/**
 * Class FileManagerImpl handles input and output files.
 */
class FileManagerImpl : public FileManager {
 public:
  FileManagerImpl(const std::string& output_file, const std::string& input_file);

  /**
   * @brief Opens input and output files. Keeps them open during the life time
   * of the object.
   * @return TRUE if the input and output files have been opened successfully,
   * otherwise returns FALSE.
   */
  bool Init() override;

  /**
   * @brief Reads data from a file by data block size.
   * @param data_block A data block that will be filled by data.
   * @return Result code as the result of operation.
   * common::Result::DATA_IS_READ - another data block has been read.
   * successfully. common::Result::ERROR - in the case when the end of file is
   * reached and was a try to read a data
   * common::Result::END_OF_FILE - the end of file has been reached.
   */
  common::Result ReadDataBlock(common::DataContainer& data_block) override;

  bool WriteHashToFile(const std::string& hash_string) override;

  ~FileManagerImpl() override;

 private:
  bool OpenInputFile();
  bool OpenOutputFile();

  void CloseInputFile();
  void CloseOutputFile();

  void CalculateFileSize();

  std::size_t file_size_;
  std::size_t already_read_size_;

  std::size_t data_block_id_;

  const std::string input_file_path_;
  const std::string output_file_path_;

  std::ifstream input_stream_;
  std::ofstream output_steam_;

  std::mutex read_file_mutex_;
  std::mutex write_file_mutex_;
};
}  // namespace file_manager

#endif  // SRC_FILE_MANAGER_IMPL_H
