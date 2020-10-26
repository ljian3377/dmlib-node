Node.js wrapper for native DMLib Track2 library.

# How to use

After installing the dmlib-node NPM pacakge, use it just like a normal JavaScript library.

```js
const DmlibAddon = require("dmlib-node");

async function upload() {
  // Enter your storage account name and shared key
  const accountName = process.env.ACCOUNT_NAME || "";
  const accountSAS = process.env.ACCOUNT_SAS || "";
  const filePath = process.env.FILE_PATH || "";

  return new Promise((reslove, reject) => {
    DmlibAddon.uploadFile(
      filePath,
      accountName,
      accountSAS,
      async (status, msg) => {
        if (status === 0) {
          reslove();
        } else {
          reject(new Error(msg));
        }
      }
    );
  });
}
```

### Note

Currently Electron (V1.4.x+) can only call modules built using CMake.js from the main process. To call such a module from a render process use the Electron remote module in your require statement:

`var yourModule = require('electron').remote.require('pathToYourModule/cmakeModuleName.node')`

# How to compile from source code

## Windows

```bash
# clone this project
https://github.com/ljian3377/dmlib-node.git

# init and update submodules
git submodule update --init --recursive

# configure dependencies for native DMLib via vcpkg
cd dmlib/vcpkg
.\bootstrap-vcpkg.bat
vcpkg install --triplet x64-windows-static curl[winssl] libxml2 dirent

# complie with cmake-js
npm install -g cmake-js
# initialize envs
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
cmake-js -G "Ninja" --CDCMAKE_TOOLCHAIN_FILE="./dmlib/vcpkg/scripts/buildsystems/vcpkg.cmake" --CDVCPKG_TARGET_TRIPLET="x64-windows-static" --CDBUILD_CURL_TRANSPORT=ON --CDBUILD_DMLIB_TESTS=OFF --CDBUILD_DMLIB_SAMPLES=OFF  --CDINSTALL_GTEST=OFF --CDBUILD_TESTING=OFF --CDBUILD_STORAGE_SAMPLES=OFF rebuild
```
