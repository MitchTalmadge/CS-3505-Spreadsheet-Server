cmake_minimum_required (VERSION 2.8.11)
project(Pigrammers_Spreadsheet_Server)
set(CMAKE_CXX_STANDARD 11)  # enable C++11 standard

include_directories(src)
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(server ${SOURCES})