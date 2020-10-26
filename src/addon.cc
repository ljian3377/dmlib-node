#include <cstdio>
#include <iostream>
#include <string>
#include "dmlib.h"
#define NAPI_EXPERIMENTAL
#include <napi.h>

typedef struct
{
  int status;
  char message[1024];
} CallbackData;

bool CompleteCallback(void *context, unsigned int jobId, int statusCode)
{
  Napi::ThreadSafeFunction tsfn = *((Napi::ThreadSafeFunction *)context);
  CallbackData *data = (CallbackData *)malloc(sizeof(*data));
  data->status = statusCode;
  if (statusCode != 0)
  {
    snprintf(data->message, sizeof(data->message), "Task failed, status: %d", statusCode);
  }
  else
  {
    snprintf(data->message, sizeof(data->message), "Task succeeded, status: %d", statusCode);
  }

  // call tsfn
  auto callback = [](Napi::Env env, Napi::Function jsCallback, const CallbackData *data) {
    jsCallback.Call({Napi::Number::New(env, data->status), Napi::String::New(env, data->message)});
    free((void *)data);
  };

  // tsfn.Acquire();
  auto napiStatus = tsfn.BlockingCall(data, callback);
  if (napiStatus != napi_ok)
  {
    std::cerr << "BlockingCall failed with napi_status: " << napiStatus << std::endl;
  }
  tsfn.Release();

  return true;
}

Napi::Value UploadFile(const Napi::CallbackInfo &info)
{
  std::string usage = "uploadFile(filePath, accountName, SAS, jsCallback, existingContainerName? = \"dmlibtestcontainer\", newBlobName? = \"dmlibtestblob\")";

  Napi::Env env = info.Env();
  if (info.Length() < 4)
  {
    Napi::TypeError::New(env, usage).ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsString() || !info[1].IsString() || !info[2].IsString() || !info[3].IsFunction())
  {
    Napi::TypeError::New(env, usage).ThrowAsJavaScriptException();
    return env.Null();
  }

  // required parameters
  std::string filePath = info[0].As<Napi::String>();
  std::string accountName = info[1].As<Napi::String>();
  std::string SAS = info[2].As<Napi::String>();
  const Napi::Function jsCallback = info[3].As<Napi::Function>();

  // optional parameters
  std::string containerName = "dmlibtestcontainer";
  if (info.Length() >= 5)
  {
    containerName = info[4].As<Napi::String>();
  }
  std::string blobName = "dmlibtestblob";
  if (info.Length() >= 6)
  {
    blobName = info[5].As<Napi::String>();
  }

  char message[1024];
  snprintf(message, sizeof(message), "filePath: %s, accountName: %s, SAS: %s, existingContainerName: %s, newBlobName: %s",
           filePath.c_str(), accountName.c_str(), SAS.c_str(), containerName.c_str(), blobName.c_str());
  std::cout << std::string(message) << std::endl;

  // create thread-safe callback
  Napi::ThreadSafeFunction *Ptsfn = (Napi::ThreadSafeFunction *)malloc(sizeof(*Ptsfn));
  *Ptsfn = Napi::ThreadSafeFunction::New(
      env,
      jsCallback,
      "",
      0, // unlimited queue
      1, // Only one thread will use this initially
      [](Napi::Env env, Napi::ThreadSafeFunction *p) {
        // std::cout << "finalize callback" << std::endl;
        free(p);
      },
      Ptsfn);

  TransferOptions transfer_options = {0, 0, 0, 0, 1, ShouldTransferOptions::NoIfDestExists}; // use https
  unsigned int jobId = upload_from_file_async_e(
      filePath.c_str(),
      blobName.c_str(),
      cloud_storage_type::block_blob,
      accountName.c_str(),
      containerName.c_str(),
      AzureStorageCredentialType::sas_token,
      SAS.c_str(),
      0,
      transfer_options,
      nullptr,
      nullptr,
      nullptr,
      CompleteCallback,
      Ptsfn);

  if (jobId < 0)
  {
    Ptsfn->Abort();

    std::cerr << "negative jobId: " << jobId << " " << std::endl;
    snprintf(message, sizeof(message), "Submit task failed, errorCode: %d", jobId);
    Napi::TypeError::New(env, message).ThrowAsJavaScriptException();
    return env.Null();
  }

  return env.Null();
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "uploadFile"), Napi::Function::New(env, UploadFile));
  return exports;
}

NODE_API_MODULE(addon, Init)
