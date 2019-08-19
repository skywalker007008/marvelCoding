cmake_minimum_required(VERSION 3.8)
project(marvelCoding)

find_package(Threads)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp
                 marvel_client.h
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
                 sample.cpp
                 marvel_app.h
                 marvel_app.cpp)

set(CODEC codec/codec.h
          codec/codec.cpp
          codec/gf.h
          codec/gf.cpp
          codec/test_codec.h
          codec/test_codec.cpp codec/header.cpp)

set(MATRIX matrix/Matrix.hpp
           matrix/SquareMatrix.hpp
           matrix/Vector.hpp
           matrix/Vector2.hpp
           matrix/Vector3.hpp)

set(APP
        app_ebr.h codec/header.h)


add_executable(marvelCoding ${SOURCE_FILES} ${CODEC} ${APP} main.cpp)
# add_executable(test_codec ${MATRIX} ${CODEC} main.cpp)

# target_link_libraries(marvelCoding ${CMAKE_THREAD_LIBS_INIT})