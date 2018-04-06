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
find_package(Boost 1.57.0 COMPONENTS system filesystem thread date_time atomic regex serialization)
if (Boost_FOUND)
    # Boost was located on the system.
    message("Boost libraries found. Using system-installed libraries.")

    # Link libraries.
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(server ${Boost_LIBRARIES})
else ()
    # Boost could not be found, and will be downloaded instead.
    message("Boost libraries not found. Boost will be downloaded and compiled.")

    # The location that Boost will be installed into.
    set(BOOST_INSTALL_DIR ${LIB_INSTALL_DIR}/boost)

    # Decide whether to use Windows or Unix install scripts.
    set(BOOST_BOOTSTRAP_COMMAND)
    if (UNIX)
        set(BOOST_BOOTSTRAP_COMMAND ./bootstrap.sh)
        set(BOOST_B2_COMMAND ./b2)
    else ()
        set(BOOST_BOOTSTRAP_COMMAND cmd /C bootstrap.bat gcc)
        set(BOOST_B2_COMMAND b2.exe)
    endif ()

    # Decide whether to compile as 32 or 64 bit.
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(BOOST_ADDRESS_MODEL 64)
    else ()
        set(BOOST_ADDRESS_MODEL 32)
    endif ()

    # Download and install from git.
    ExternalProject_Add(
            boost

            GIT_REPOSITORY "https://github.com/boostorg/boost.git"
            GIT_TAG "boost-1.57.0"

            SOURCE_DIR ${BOOST_INSTALL_DIR}/src
            BUILD_IN_SOURCE 1

            UPDATE_COMMAND ""
            CONFIGURE_COMMAND ${BOOST_BOOTSTRAP_COMMAND} --prefix=${BOOST_INSTALL_DIR}/lib
            BUILD_COMMAND ${BOOST_B2_COMMAND} install -j8 --prefix=${BOOST_INSTALL_DIR} --with-system --with-filesystem --with-thread --with-date_time --with-atomic --with-regex --with-serialization address-model=${BOOST_ADDRESS_MODEL} link=static
            INSTALL_COMMAND ""
    )

    # Link libraries.
    include_directories(server ${BOOST_INSTALL_DIR}/src/boost/)
    link_libraries(server ${BOOST_INSTALL_DIR}/lib)
    add_dependencies(server boost)
endif ()