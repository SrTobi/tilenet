cmake_minimum_required(VERSION 2.6) 



project(tilenet)

set(Boost_USE_STATIC_LIBS        OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)


find_package(Boost 1.49 COMPONENTS thread system filesystem REQUIRED)
find_package(SFML 2 COMPONENTS system window graphics network)

ADD_DEFINITIONS("-g -std=c++0x")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories("include")
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(test)
add_subdirectory(tilenet)
