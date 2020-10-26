# Copyright (c) Microsoft Corporation. All rights reserved.
# SPDX-License-Identifier: MIT

cmake_minimum_required (VERSION 3.15)
set (CMAKE_CXX_STANDARD 14)

# Name of the project (will be the name of the plugin)
project(dmlib-addon VERSION 0.1)

# define NPI_VERSION
add_definitions(-DNAPI_VERSION=4) # bump up to use Napi::ThreadSafeFunction

include_directories(${CMAKE_JS_INC})

# Build a shared library named after the project from the files in `src/`
file(GLOB SOURCE_FILES "src/*.cc" "src/*.h")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})

# Gives our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
# set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/DELAYLOAD:node.exe" )
target_link_libraries(${PROJECT_NAME} PRIVATE delayimp)

# Essential include files to build a node addon,
# You should add this line in every CMake.js based project
# target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})

# Essential library files to link to a node addon
# You should add this line in every CMake.js based project
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_JS_LIB})

# Include N-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})

# Include DMLib
add_subdirectory(dmlib)
target_include_directories(${PROJECT_NAME} PRIVATE dmlib/dmlib_core/inc)
target_link_libraries(${PROJECT_NAME} PRIVATE dmlib)

# hello.exe
# add_executable(hello test/hello.cc)
# target_include_directories(hello PRIVATE dmlib/dmlib_core/inc)
# target_link_libraries(hello PRIVATE dmlib)

# call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
# cmake-js -G "Ninja" --CDCMAKE_TOOLCHAIN_FILE="./dmlib/vcpkg/scripts/buildsystems/vcpkg.cmake" --CDVCPKG_TARGET_TRIPLET="x64-windows-static" --CDBUILD_CURL_TRANSPORT=ON --CDBUILD_DMLIB_TESTS=OFF --CDBUILD_DMLIB_SAMPLES=OFF  --CDINSTALL_GTEST=OFF --CDBUILD_TESTING=OFF --CDBUILD_STORAGE_SAMPLES=OFF rebuild
