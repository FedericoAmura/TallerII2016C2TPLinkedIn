cmake_minimum_required(VERSION 3.2.2)

project(AppServer)
include_directories(include)
file(GLOB_RECURSE COMMON "src/*.cpp" "src/*.c" )
list(REMOVE_ITEM COMMON "src/AppServer.cpp")
file(GLOB_RECURSE TEST "test/*.cpp")
add_library(libcommon STATIC ${COMMON})

add_executable(AppServer "src/AppServer.cpp")
add_executable(UnitTest ${TEST})

TARGET_LINK_LIBRARIES(AppServer libcommon pthread)
TARGET_LINK_LIBRARIES(UnitTest libcommon pthread ${CMAKE_SOURCE_DIR}/lib/libgtest.a)

