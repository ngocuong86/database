cmake_minimum_required(VERSION 3.10)
project(database)

set(CMAKE_CXX_STANDARD 17)
include_directories(${PROJECT_SOURCE_DIR})
file(GLOB_RECURSE SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/database/*"
        "${PROJECT_SOURCE_DIR}/Libraries/*"
        "${PROJECT_SOURCE_DIR}/Sources/*"
        "${PROJECT_SOURCE_DIR}/net/*"
        "${PROJECT_SOURCE_DIR}/session/*"
        "${PROJECT_SOURCE_DIR}/controller/*"
        )
link_libraries(pthread ssl crypto rt sqlite3 curl dl mosquitto event event_openssl event_pthreads gobject-2.0 gio-2.0 glib-2.0)

#find_package(LibXml2 REQUIRED)
#include_directories(${LIBXML2_INCLUDE_DIR})
# find GIO
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(GIO2 REQUIRED gio-2.0)
#include_directories(${GIO2_INCLUDE_DIRS})
#
#find_package(PkgConfig REQUIRED)
#pkg_search_module(GLIB2 REQUIRED glib-2.0)
#include_directories(${GLIB2_INCLUDE_DIRS})
####mosquitto
link_libraries(mosquitto)

##### Find Leveldb package #####


#### Print table bprinter ####
#add_subdirectory(third_party/bprinter)
#add_subdirectory(third_party/threadpool)

#### Library ####
add_library(database SHARED ${SOURCE_FILES} database/database-command.hpp database/record.hpp)

#target_include_directories(database PUBLIC
#        third_party
#        ${LEVELDB_INCLUDE_DIRS})

#target_link_libraries(database
#        ${SQLite3_LIBRARIES}
#        leveldb
#        pthread
#        )

#install(TARGETS database
#        LIBRARY DESTINATION third_party)

#### Google Test ####
#enable_testing()
#
#add_executable(test-database
#        ${SOURCES}
#        test/test-database.cpp)
#
#target_include_directories(test-database PUBLIC
#        third_party
#        ${LEVELDB_INCLUDE_DIRS} )
#
#target_link_libraries(test-database
#        database
#        gtest
#        gtest_main
#        ${SQLite3_LIBRARIES}
#        leveldb
#        pthread
#        bprinter)
#
#add_test(
#        NAME UnitTests
#        COMMAND test-database
#)

#### Benchmark ####
#add_executable(benchmark-database
#        ${SOURCES}
#        test/benchmark-database.cpp main.cpp session/session.hpp session/session.cpp Libraries/Utils/Exception2.hpp net/Dbus/dbus-client.hpp net/Dbus/dbus-client.cpp controller/controller.hpp controller/controller.cpp database_config.hpp session/local_session.hpp session/local_session.cpp
#        Libraries/Utils/json.hpp)
#
#target_include_directories(benchmark-database PUBLIC
#        third_party
#        ${LEVELDB_INCLUDE_DIRS} )
#
#target_link_libraries(benchmark-database
#        database
#        gtest
#        gtest_main
#        net
#        ${SQLite3_LIBRARIES}
#        leveldb
#        pthread
#        bprinter
#        )
#
#add_test(
#        NAME BenchmarkTests
#        COMMAND benchmark-database
#)

#if (CMAKE_BUILD_TYPE STREQUAL Debug)
#    message(STATUS "Enable SANITIZER")
#    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -g")
#    set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -g")
#    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address -g")
#endif ()
if(PLATFORM STREQUAL "MT7688")

    add_definitions(-DMT7688)

    #set(STAGING_DIR $ENV{HOME}/mt7688-g1/staging_dir)

    set(TOOLS_PATH ${STAGING_DIR}/toolchain-mipsel_24kec+dsp_gcc-4.8-linaro_uClibc-0.9.33.2)

    set(TARGET ${STAGING_DIR}/target-mipsel_24kec+dsp_uClibc-0.9.33.2)



    message(STATUS "STAGING_DIR:=${STAGING_DIR}")

    message(STATUS "TOOLS_PATH:=${TOOLS_PATH}")

    message(STATUS "TARGET:=${TARGET}")



    set(CMAKE_C_COMPILER ${TOOLS_PATH}/bin/mipsel-openwrt-linux-gcc)

    set(CMAKE_CXX_COMPILER ${TOOLS_PATH}/bin/mipsel-openwrt-linux-g++)



    include_directories(${TARGET}/usr/include)
    include_directories(${TARGET}/root-ramips/usr/include)

    link_directories(${TARGET}/usr/lib)
    link_directories(${TARGET}/root-ramips/usr/lib)

    link_libraries(uci mraa avahi-client avahi-common)

elseif (PLATFORM STREQUAL "MT7688_OW19")

    add_definitions(-DMT7688)
    add_definitions(-DOW19)

    #set(STAGING_DIR $ENV{HOME}/mt7688-g1/staging_dir)

    set(TOOLS_PATH ${STAGING_DIR}/toolchain-mipsel_24kc_gcc-8.3.0_musl)

    set(TARGET ${STAGING_DIR}/target-mipsel_24kc_musl)



    message(STATUS "STAGING_DIR:=${STAGING_DIR}")

    message(STATUS "TOOLS_PATH:=${TOOLS_PATH}")

    message(STATUS "TARGET:=${TARGET}")



    set(CMAKE_C_COMPILER ${TOOLS_PATH}/bin/mipsel-openwrt-linux-gcc)

    set(CMAKE_CXX_COMPILER ${TOOLS_PATH}/bin/mipsel-openwrt-linux-g++)

    set(CMAKE_C_FLAGS "-I${TARGET}/usr/include")
    set(CMAKE_CXX_FLAGS "-I${TARGET}/usr/include")
    include_directories(${TARGET}/root-ramips/usr/include)


    set(CMAKE_SYSROOT ${TARGET})
    # RPATH settings for linking shared libraries
    # skip the full RPATH for the build tree
    set(CMAKE_SKIP_BUILD_RPATH TRUE)
    # when building, use the install RPATH already
    # (so it doesn't need to relink when installing)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    # the RPATH to be used when installing
    set(CMAKE_INSTALL_RPATH "${TARGET}/root-ramips/lib/;${TARGET}/usr/lib/")

    # add the automatically determined parts of the RPATH
    # which point to directories outside the build tree to the install RPATH
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    # I also found somebody using the following, but the used variable seems to be deprecated as I could not find it in the current CMake documentation:
    set(CMAKE_SHARED_LIBRARY_RPATH_LINK_C_FLAG "-Wl,-rpath-link,-Wall")

   # set(CMAKE_C_COMPILER "${STAGING_DIR}/usr/bin/arm-linux-gnueabihf-gcc")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
        -I${STAGING_DIR}/target-mipsel_24kc_musl/usr/include/glib-2.0 \
        -I${STAGING_DIR}/target-mipsel_24kc_musl/usr/lib/glib-2.0/include \
        -I/home/cuong/Downloads/database_service/third_party/leveldb/include \
        -I/home/cuong/Downloads/database_service/third_party")

    #set(CMAKE_CXX_COMPILER "${STAGING_DIR}/usr/bin/arm-linux-gnueabihf-g++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
        -std=c++17 \
        -I${STAGING_DIR}/target-mipsel_24kc_musl/usr/include/glib-2.0 \
        -I${STAGING_DIR}/target-mipsel_24kc_musl/usr/lib/glib-2.0/include \
        -I/home/cuong/Downloads/database_service/third_party/leveldb/include \
        -I/home/cuong/Downloads/database_service/third_party/leveldb \
        -I/home/cuong/Downloads/database_service/third_party/leveldb/third_party/googletest/googlemock/include")

    link_directories(${TARGET}/usr/lib)
    link_directories(${TARGET}/root-ramips/usr/lib)

    link_libraries(uci avahi-client avahi-common /home/cuong/Downloads/database_service/third_party/leveldb/build/libleveldb.a)
elseif(PLATFORM STREQUAL "SSD202D")
    add_definitions(-DSSD202D)
    message(STATUS "Build for SSD202D")
    message(STATUS "TOOLS_PATH:" ${TOOLS_PATH})
    set(SYSROOT "${TOOLS_PATH}/usr/arm-buildroot-linux-gnueabihf/sysroot")

    set(CMAKE_C_COMPILER "${TOOLS_PATH}/usr/bin/arm-linux-gnueabihf-gcc")
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated -Wno-psabi -I${SYSROOT}/usr/include/glib-2.0 -I${SYSROOT}/usr/lib/glib-2.0/include -I/home/cuong/Downloads/database_service/third_party/leveldb/include")

    set(CMAKE_CXX_COMPILER "${TOOLS_PATH}/usr/bin/arm-linux-gnueabihf-g++")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wno-psabi -I${SYSROOT}/usr/include/glib-2.0 -I${SYSROOT}/usr/lib/glib-2.0/include -I/home/cuong/Downloads/database_service/third_party/leveldb/include")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wno-psabi \
        -std=c++17 \
        -I${SYSROOT}/usr/include/glib-2.0 \
        -I${SYSROOT}/usr/lib/glib-2.0/include \
        -I/home/cuong/Downloads/database_service/third_party/leveldb/include")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated -Wno-psabi \
        -I${SYSROOT}/usr/include/glib-2.0 \
        -I${SYSROOT}/usr/lib/glib-2.0/include \
        -I/home/cuong/Downloads/database_service/third_party/leveldb/include")
    link_directories(${TARGET}/usr/lib)
    link_directories(${TARGET}/root-ramips/usr/lib)
    link_libraries( avahi-client avahi-common /home/cuong/Downloads/database_service/third_party/leveldb/build/libleveldb.a)
endif()
add_executable(test  test/benchmark.cpp
        )
add_executable(sq  test/BmSQ.cpp
        )
add_executable(main ${SOURCE_FILES} main.cpp  ${SOURCE_FILES})
add_executable(benchmarkLevel test/benchmarkLevel.cpp)
target_link_libraries(main
        database
        )
install(TARGETS database RUNTIME DESTINATION bin)
