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

set(BOOST_INSTALL_DIR ${LIB_INSTALL_DIR}/boost)

set(BOOST_BOOTSTRAP_COMMAND)
if (UNIX)
    set(BOOST_BOOTSTRAP_COMMAND ./bootstrap.sh)
    set(BOOST_B2_COMMAND ./b2)
else ()
    set(BOOST_BOOTSTRAP_COMMAND cmd /C bootstrap.bat gcc)
    set(BOOST_B2_COMMAND b2.exe)
endif ()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(BOOST_ADDRESS_MODEL 64)
else ()
    set(BOOST_ADDRESS_MODEL 32)
endif ()

if (${BUILD_SHARED_LIBS} MATCHES OFF)
    set(BUILD_LIBS "static")
elseif (${BUILD_SHARED_LIBS} MATCHES ON)
    set(BUILD_LIBS "shared")
endif ()

ExternalProject_Add(
        boost

        BUILD_IN_SOURCE 1
        GIT_REPOSITORY "https://github.com/boostorg/boost.git"
        GIT_TAG "boost-1.66.0"
        UPDATE_COMMAND ""
        CONFIGURE_COMMAND ${BOOST_BOOTSTRAP_COMMAND} --prefix=${BOOST_INSTALL_DIR}/lib
        BUILD_COMMAND ${BOOST_B2_COMMAND} install -j8 --prefix=${BOOST_INSTALL_DIR} --with-thread --with-filesystem --with-system --with-date_time --with-program_options --with-atomic address-model=${BOOST_ADDRESS_MODEL} link=static,shared
        INSTALL_COMMAND ""
)
include_directories(server ${CMAKE_BINARY_DIR}/boost-prefix/src/boost/)
link_libraries(server ${BOOST_INSTALL_DIR}/lib)
add_dependencies(server boost)