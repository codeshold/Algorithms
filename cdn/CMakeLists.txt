cmake_minimum_required(VERSION 3.6)
project(huawei)

set(CMAKE_CXX_STANDARD 11)
# include路径
include_directories(${PROJECT_SOURCE_DIR}/lib)

# 设置可执行文件生成路径
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)

# 生成debug版本
# SET(CMAKE_BUILD_TYPE "release")
SET(CMAKE_BUILD_TYPE "debug")
if (CMAKE_BUILD_TYPE STREQUAL debug)
    add_definitions(-D_DEBUG)
endif ()
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -std=c++11")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -std=c++11")

#set(SOURCE_FILES minicost.cpp lib_io.cpp deploy.cpp cdn.cpp deploy.h lib/lib_io.h lib/lib_time.h minicost.h)
set(SOURCE_FILES minicost.cpp lib_io.cpp deploy.cpp cdn.cpp deploy.h lib/lib_io.h lib/lib_time.h minicost.h)

add_executable(huawei ${SOURCE_FILES})