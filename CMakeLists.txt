cmake_minimum_required(VERSION 3.0.0)
project(Pigrammers_Spreadsheet_Server)

# Enable C++11 standard.
set(CMAKE_CXX_STANDARD 11)

# Add server source configuration.
include(src/CMakeLists.txt.in)

# Add server test configuration.
include(test/CMakeLists.txt.in)

# Add third-party libraries.
include(lib/CMakeLists.txt.in)