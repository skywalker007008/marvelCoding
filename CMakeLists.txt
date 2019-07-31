cmake_minimum_required(VERSION 3.8)
project(marvelCoding)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp MarvelClient.h MarvelClient.cpp MarvelException.h MarvelConstant.h MarvelConstant.cpp MarvelException.cpp MarvelServer.h MarvelServer.cpp MarvelLog.h MarvelLog.cpp)
add_executable(marvelCoding ${SOURCE_FILES})