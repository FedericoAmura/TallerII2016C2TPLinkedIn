cmake_minimum_required(VERSION 3.2.2)

project(AppServer)

include_directories(include)
file(GLOB_RECURSE COMMONSRC "src/*.cpp" "src/*.c" )
list(REMOVE_ITEM COMMONSRC "src/AppServer.cpp")
file(GLOB_RECURSE TEST "test/*.cpp")
add_library(libcommonsrc STATIC ${COMMONSRC})

add_executable(AppServer "src/AppServer.cpp")
add_executable(UnitTest ${TEST})

set(COMMONLIBS libcommonsrc pthread ${CMAKE_SOURCE_DIR}/lib/libleveldb.a)
TARGET_LINK_LIBRARIES(AppServer ${COMMONLIBS})
TARGET_LINK_LIBRARIES(UnitTest ${COMMONLIBS} ${CMAKE_SOURCE_DIR}/lib/libgtest.a)

