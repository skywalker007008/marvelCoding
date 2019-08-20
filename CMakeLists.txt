cmake_minimum_required(VERSION 3.8)
project(marvelCoding)

find_package(Threads)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES marvel_client.h
                 marvel_client.cpp
                 marvel_exception.h
                 marvel_constant.h
                 marvel_constant.cpp
                 marvel_exception.cpp
                 marvel_server.h
                 marvel_server.cpp
                 marvel_log.h
                 marvel_log.cpp
                 marvel_socket.h
                 marvel_socket.cpp
                 api_app.h
                 api_app.cpp
                 test_app.h
                 test_app.cpp
                 )

set(CODEC codec/codec.h
          codec/codec.cpp
          codec/gf.h
          codec/gf.cpp
          codec/test_codec.h
          codec/test_codec.cpp
          codec/header.h
          codec/header.cpp)

set(MATRIX matrix/Matrix.hpp
           matrix/SquareMatrix.hpp
           matrix/Vector.hpp
           matrix/Vector2.hpp
           matrix/Vector3.hpp)

set(APP marvel_app.h
        marvel_app.cpp
        test_app.h
        test_app.cpp)

set(DEBUG debug.h
          debug.cpp)

# add_executable(marvelCoding ${SOURCE_FILES} ${CODEC} ${APP} ${DEBUG} main.cpp)
add_executable(testClient ${SOURCE_FILES} ${CODEC} ${APP} ${DEBUG} test_client.cpp)
add_executable(testServer ${SOURCE_FILES} ${CODEC} ${APP} ${DEBUG} test_server.cpp)
# add_executable(test_codec ${MATRIX} ${CODEC} main.cpp)

target_link_libraries(testClient ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(testServer ${CMAKE_THREAD_LIBS_INIT})