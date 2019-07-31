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
#include <netinet/in.h>

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
        virtual void print();
    };

    class SocketCreateFailedException : public MarvelException {
    public:
        SocketCreateFailedException();
        void print();
    };

    class MessageOversizedException : public MarvelException {
    public:
        MessageOversizedException();
    };

    class SocketConnectFailedException : public MarvelException {
    public:
        SocketConnectFailedException(struct sockaddr_in sockaddr);
    };

    class MessageSendFailedException : public MarvelException {
    public:
        MessageSendFailedException(int pos, int size);
    };

    class MessageRecvFailedException : public MarvelException {
    public:
        MessageRecvFailedException(int pos, int size);
    };

    class SocketBindFailedException : public MarvelException {
    public:
        SocketBindFailedException(struct sockaddr_in sockaddr);
    };

    class SocketListenFailedException : public MarvelException {
    public:
        SocketListenFailedException(struct sockaddr_in sockaddr);
    };

    class SocketAcceptFailedException : public MarvelException {
    public:
        SocketAcceptFailedException(struct sockaddr_in sockaddr);
    };
} // namespace marvel::err



#endif //MARVELCODING_MARVELEXCEPTION_H
