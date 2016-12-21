cmake_minimum_required(VERSION 3.2.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER "gcc-4.9")
set(CMAKE_CXX_COMPILER "/usr/bin/g++-4.9")

project(AppServer)

#add_definitions(-DMG_ENABLE_THREADS)

file(GLOB_RECURSE COMMONSRC "src/*.cpp" "src/*.c" )
list(REMOVE_ITEM COMMONSRC ${CMAKE_CURRENT_SOURCE_DIR}/src/AppServer.cpp)
file(GLOB_RECURSE TEST "test/*.cpp")
#Antes se generaba una libreria estatica con los files common y se la linkeaba para no compilar 2 veces lo mismo
#Pero al gcov no le gusta, no se ejecuta el fprogiles-arcs despues

add_executable(AppServer ${COMMONSRC} "src/AppServer.cpp")
#Para la generacion del paquete de instalacion
install(TARGETS AppServer DESTINATION appserver)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/default.jpg DESTINATION appserver)
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jobify Team")
include(CPack)

add_executable(UnitTest ${COMMONSRC} ${TEST} )

find_package(CURL)
include_directories(include)
include_directories(${CURL_INCLUDE_DIRS})
set(COMMONLIBS pthread ${CURL_LIBRARIES} ${CMAKE_SOURCE_DIR}/lib/libleveldb.a ${CMAKE_SOURCE_DIR}/lib/libjpeg.a)
target_link_libraries(AppServer ${COMMONLIBS})
target_link_libraries(UnitTest ${COMMONLIBS} --coverage ${CMAKE_SOURCE_DIR}/lib/libgtest.a)
target_compile_options(UnitTest PUBLIC -O0 --coverage -g)
