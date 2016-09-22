cmake_minimum_required(VERSION 3.2.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(AppServer)

include_directories(include)
file(GLOB_RECURSE COMMONSRC "src/*.cpp" "src/*.c" )
list(REMOVE_ITEM COMMONSRC ${CMAKE_CURRENT_SOURCE_DIR}/src/AppServer.cpp)
file(GLOB_RECURSE TEST "test/*.cpp")
#Antes se generaba una libreria estatica con los files common y se la linkeaba para no compilar 2 veces lo mismo
#Pero al gcov no le gusta, no se ejecuta el fprogiles-arcs despues

add_executable(AppServer ${COMMONSRC} "src/AppServer.cpp")
add_executable(UnitTest ${COMMONSRC} ${TEST} )

set(COMMONLIBS pthread ${CMAKE_SOURCE_DIR}/lib/libleveldb.a ${CMAKE_SOURCE_DIR}/lib/liblog4cpp.a)
TARGET_LINK_LIBRARIES(AppServer ${COMMONLIBS})
TARGET_LINK_LIBRARIES(UnitTest ${COMMONLIBS} --coverage ${CMAKE_SOURCE_DIR}/lib/libgtest.a)
target_compile_options(UnitTest PUBLIC -O0 --coverage -g)
