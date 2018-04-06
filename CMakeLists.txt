cmake_minimum_required(VERSION 2.8.11)
project(Pigrammers_Spreadsheet_Server)
set(CMAKE_CXX_STANDARD 11)  # enable C++11 standard

# Add source code
include_directories(src)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define executable
add_executable(server ${SOURCES})

# Require threads
find_package(Threads)
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})

# Add third-party libraries
include(lib/CMakeLists.txt)