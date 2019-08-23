
cmake_minimum_required (VERSION 3.5)
project(usocket)

# include
include_directories(./include)
# find config.h
include_directories("${PROJECT_BINARY_DIR}")

# debug
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# sub dir
add_subdirectory(core)
add_subdirectory(test)

# intall dir
SET(CMAKE_INSTALL_PREFIX /usr/local/usocket)

# version number
set(Smurfs_VERSION_MAJOR 1)
set(Smurfs_VERSION_MINOR 0)

# configure a header file to pass some of CMake settings to source file
configure_file(
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)

# install
install (FILES "${PROJECT_BINARY_DIR}/config.h" DESTINATION include)
