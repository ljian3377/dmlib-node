#include <napi.h>
#include <iostream>
#include <string>
#include "dmlib.h"

Napi::Value UploadFile(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() < 3)
  {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString() || !info[1].IsString() || !info[2].IsString())
  {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string accountName = info[0].As<Napi::String>();
  std::string accountKey = info[1].As<Napi::String>();
  std::string filePath = info[2].As<Napi::String>();

  std::cout << accountName << " " << accountKey << " " << filePath << std::endl;

  std::string blobName = "dmlibtestblob";
  std::string containerName = "dmlibtestcontainer";

  unsigned int taskId = upload_from_file_async(
      filePath.c_str(),
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

  Napi::Number statusEnv = Napi::Number::New(env, status);
  return statusEnv;
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "uploadFile"), Napi::Function::New(env, UploadFile));
  return exports;
}

NODE_API_MODULE(addon, Init)
