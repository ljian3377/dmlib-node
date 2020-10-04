#include <iostream>
#include <string>
#include "dmlib.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " fileName" << std::endl;
        return 1;
    }
    else
    {
        std::cout << argc << " " << argv[1] << std::endl;
    }

    std::string fileName = argv[1];
    std::string blobName = "dmlibtestblob";
    std::string containerName = "dmlibtestcontainer";
    std::string accountName = "";
    std::string accountKey = "";

    unsigned int taskId = upload_from_file_async(
        fileName.c_str(),
        blobName.c_str(),
        cloud_storage_type::block_blob,
        accountName.c_str(),
        containerName.c_str(),
        AzureStorageCredentialType::shared_key,
        accountKey.c_str(),
        0,
        nullptr,
        nullptr,
        nullptr);

    if (taskId < 0)
    {
        std::cerr << "negative taskId: " << taskId << " " << std::endl;
    }

    int status = WaitForJob(taskId);
    if (status != 0)
    {
        std::cerr << "Status: " << status << " " << GetJobErrorInfo(taskId) << std::endl;
    }

    return 0;
}