cmake_minimum_required(VERSION 3.8)
project(marvelCoding)

find_package(Threads)

set(CMAKE_CXX_STANDARD 14)
# Control whether debug mode
add_definitions(-D MARVEL_DEBUG)
# Control which protocal we use
add_definitions(-D MARVEL_TCP)
# Test MACRO
# Test Resend
add_definitions(-D TEST_RESEND)

# add_definitions(-D MODE_WINDOW)

set(SOURCE_FILES
        src/exception/marvel_exception.h
        src/exception/marvel_exception.cpp
        src/include/marvel_constant.h
        src/include/marvel_constant.cpp
        src/include/marvel_log.h
        src/include/marvel_log.cpp
        src/marvel/marvel_socket.h
        src/marvel/marvel_socket.cpp
        src/marvel/marvel_app.h
        src/marvel/marvel_app.cpp
        src/marvel/marvel_client.h
        src/marvel/marvel_client.cpp
        src/marvel/marvel_server.h
        src/marvel/marvel_server.cpp
        src/api/api_app.h
        src/api/api_app.cpp
        src/include/queue.h
        src/include/list.h
        src/include/list.cpp
        src/include/alloc.hpp
        src/include/RS-FEC.h
        )

set(TEST test/test_app.h
         test/test_app.cpp)

set(CODEC src/codec/codec.h
        src/codec/codec.cpp
        src/codec/gf.h
        src/codec/gf.cpp
        src/codec/test_codec.h
        src/codec/test_codec.cpp
        src/codec/header.h
        src/codec/header.cpp)

set(APP src/marvel/marvel_app.h
        src/marvel/marvel_app.cpp
        )

set(DEBUG src/include/debug.h
          src/include/debug.cpp)

# add_executable(marvelCoding ${SOURCE_FILES} ${CODEC} ${APP} ${DEBUG} main.cpp)

add_executable(testClient ${SOURCE_FILES} ${CODEC} ${APP} ${TEST} ${DEBUG} test/test_client.cpp)
add_executable(testServer ${SOURCE_FILES} ${CODEC} ${APP} ${TEST} ${DEBUG} test/test_server.cpp)
# add_executable(test_codec ${MATRIX} ${CODEC} main.cpp)

target_link_libraries(testClient ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(testServer ${CMAKE_THREAD_LIBS_INIT})