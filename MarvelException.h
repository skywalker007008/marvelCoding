/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */


#ifndef MARVELCODING_MARVELEXCEPTION_H
#define MARVELCODING_MARVELEXCEPTION_H

#include <cstdint>
#include <exception>
#include <sys/socket.h>

namespace marvel::err {
    enum ErrorType {
        // socket
        SOCKET_CREATE_FAILED,
        SOCKET_CONNECT_FAILED,
        SOCKET_BIND_FAILED,
        SOCKET_LISTEN_FAILED,

        // send & recv
        SEND_FAILED,
        SEND_INCOMPLETE,
        ACCEPT_FAILED,
        RECV_FAILED,

        // data
        DATA_OVERSIZED,
    };
    void errMsg(ErrorType type, void* data = nullptr);

    class MarvelException : public std::exception {
    public:
        void print();
    };

    class SocketException : public MarvelException {
    public:
        SocketException(ErrorType type, struct sockaddr_in& sockaddr);
        void print();
    };
} // namespace marvel::err



#endif //MARVELCODING_MARVELEXCEPTION_H
