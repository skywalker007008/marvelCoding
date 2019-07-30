cmake_minimum_required(VERSION 3.8)
project(marvelCoding)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp MarvelClient.h MarvelClient.cpp)
add_executable(marvelCoding ${SOURCE_FILES})