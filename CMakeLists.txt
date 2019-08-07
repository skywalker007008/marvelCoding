cmake_minimum_required(VERSION 3.8)
project(marvelCoding)

find_package(Threads)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp marvel_client.h marvel_client.cpp marvel_exception.h marvel_constant.h marvel_constant.cpp marvel_exception.cpp marvel_server.h marvel_server.cpp marvel_log.h marvel_log.cpp marvel_socket.h marvel_socket.cpp api_app.h api_app.cpp test_app.h test_app.cpp sample.cpp marvel_app.h marvel_app.cpp codec/codec.h codec/codec.cpp codec/matrix.h codec/matrix.cpp codec/test_codec.h codec/test_codec.cpp)
add_executable(marvelCoding ${SOURCE_FILES})

target_link_libraries(marvelCoding ${CMAKE_THREAD_LIBS_INIT})