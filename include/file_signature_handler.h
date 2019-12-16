#ifndef INCLUDE_FILE_SIGNATURE_HANDLER_IMPL_H
#define INCLUDE_FILE_SIGNATURE_HANDLER_IMPL_H

#include <atomic>
#include <condition_variable>
#include <memory>
#include <thread>
#include <vector>

#include "include/interfaces/file_manager.h"
#include "include/interfaces/resource_manager.h"
#include "utils/include/constants.h"
#include "utils/include/hash_generator.h"
#include "utils/include/types.h"

namespace file_signature_handler
{

/**
 * @brief Class FileSignatureHandler generates signature of specified input
 * file.
 */
class FileSignatureHandler
{
  public:
    /**
     * @brief FileSignatureHandler class constructor.
     *
     * @param data_file_path File path that's signature should be generated.
     * @param file_signature_path Path of the file that will contain the data
     * file signature. In the case when the parameter is omitted will be created
     * a file by default path.
     * @param block_size Size of the block by which data file will be split. In
     * the case when the parameter is omitted used default value.
     * @param threads_count Threads count which will be initialized and run.
     */
    FileSignatureHandler(
        const std::string & data_file_path,
        const std::string & file_signature_path =
            constants::DEFAULT_SIGNATURE_FILE,
        const std::size_t block_size = constants::DEFAULT_BLOCK_SIZE,
        const std::size_t threads_count = constants::DEFAULT_THREADS_COUNT);

    /**
     * @brief Generates signature data file.
     *
     * @return Signature generating result.
     * types::Result::ERROR - signature generating is failed.
     * types::Result::SIGN_IS_GENERATED - signature generating is completed
     * successfully.
     */
    types::Result GenerateFileSignature();

    /**
     * @brief Initializes FileSignatureHandler class instance. Should be called
     * before calling GenerateFileSignature method.
     *
     * @return TRUE if initialization is completed successfully, otherwise
     * FALSE.
     */
    bool Init();

    ~FileSignatureHandler();

  private:
    void InitThreads();
    void RunThreads();

    void ReadData();
    void GenerateHash();
    void WriteHash();

    types::Result
    ReadDataBlock(types::file_manager::DataContainer & data_container);
    void
    PutDataBlock(const types::file_manager::DataContainer & data_container);
    void WaitAvailableDataBlock();
    bool GetDataBlock(types::file_manager::DataContainer & data_container);
    void PutHash(const types::hash_generator::HashContainer & hash_container);
    void WaitAvailableHash();
    bool GetHash(types::hash_generator::HashContainer & hash_container);

    std::size_t block_size_;
    std::shared_ptr<file_manager::FileManager> file_manager_;
    std::shared_ptr<resource_manager::ResourceManager> resource_manager_;
    hash_generator::HashGenerator hash_generator_;

    // File manager completed to read data from a file.
    std::atomic<bool> data_reading_is_completed_;

    // Hash generator generated hashes from all data blocks.
    std::atomic<bool> hash_generating_is_completed_;

    // All data blocks have been generated into hashes.
    std::atomic<bool> end_of_data_;

    // Hash blocks are over.
    std::atomic<bool> end_of_hashes_;

    // All hash blocks are written into a file.
    std::atomic<bool> data_is_written_;

    std::size_t max_thread_count_;
    std::vector<std::thread> reading_threads_;
    std::vector<std::thread> hash_generating_threads_;
    std::unique_ptr<std::thread> writing_hash_thread_;

    std::mutex push_data_mutex_;
    std::mutex push_hash_muttex_;
    std::mutex sign_is_written_mutex_;

    std::condition_variable write_data_cv_;
    std::condition_variable push_hash_cv_;
    std::condition_variable push_data_cv_;
    std::condition_variable signature_is_written_cv_;
};

} // namespace file_signature_handler

#endif // INCLUDE_FILE_SIGNATURE_HANDLER_IMPL_H
