include(ExternalProject)
set(LIB_INSTALL_DIR ${CMAKE_BINARY_DIR}/lib)

# ==================== Google Test Libraries ==================== #

#[[set(GTEST_INSTALL_DIR ${LIB_INSTALL_DIR}/googletest)

ExternalProject_Add(
        googletest

        GIT_REPOSITORY https://github.com/google/googletest.git
        TIMEOUT 10
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
)
target_link_libraries(server gtest gtest_main pthread)
add_dependencies(server googletest)]]

# ==================== Boost Libraries ==================== #

# Attempt to locate system-installed libraries.
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.57.0 COMPONENTS system filesystem thread date_time)
if (Boost_FOUND)
    # Boost was located on the system.
    message("Boost libraries found. Using system-installed libraries.")

    # Link libraries.
    include_directories(server ${Boost_INCLUDE_DIRS})
    target_link_libraries(server ${Boost_LIBRARIES})
else ()
    message("Boost libraries not found. Compilation may fail.")
endif ()