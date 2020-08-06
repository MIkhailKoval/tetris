cmake_minimum_required(VERSION 2.6)
project(game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
#set(CMAKE_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/SFML)
set(CMAKE_MODULE_PREFIX ${CMAKE_SOURCE_DIR}/SFML/lib/cmake/SFML)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/SFML/lib/cmake/SFML)

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
include_directories(${CMAKE_SOURCE_DIR})
add_executable(game main.cpp)

target_link_libraries(game sfml-graphics sfml-audio)
