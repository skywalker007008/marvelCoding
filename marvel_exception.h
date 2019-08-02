/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */


#ifndef MARVELCODING_MARVELEXCEPTION_H
#define MARVELCODING_MARVELEXCEPTION_H

#include <cstdint>
// #include <exception>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include "marvel_constant.h"

namespace marvel {
    namespace err {
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
        // void errMsg(ErrorType type, void* data = nullptr);

        class MarvelException {
        public:
            MarvelException();
            virtual STRING print() {}
        };

        class SocketCreateFailedException : public MarvelException {
        public:
            SocketCreateFailedException();

            STRING print();
        };

        class SocketConnectFailedException : public MarvelException {
        public:
            SocketConnectFailedException(struct sockaddr_in sockaddr);

            STRING print();

            uint32_t host_;
            uint16_t port_;
        };

        class SocketBindFailedException : public MarvelException {
        public:
            SocketBindFailedException(struct sockaddr_in sockaddr);

            STRING print();

            uint32_t host_;
            uint16_t port_;
        };

        class SocketListenFailedException : public MarvelException {
        public:
            SocketListenFailedException(struct sockaddr_in sockaddr);

            STRING print();

            uint32_t host_;
            uint16_t port_;
        };

        class SocketAcceptFailedException : public MarvelException {
        public:
            SocketAcceptFailedException(struct sockaddr_in sockaddr);

            STRING print();

            uint32_t host_;
            uint16_t port_;
        };

        class MessageOversizedException : public MarvelException {
        public:
            MessageOversizedException();

            STRING print();
        };

        class MessageSendFailedException : public MarvelException {
        public:
            MessageSendFailedException(int pos, int size);

            STRING print();

            int pos_;
            int size_;
        };

        class MessageRecvFailedException : public MarvelException {
        public:
            MessageRecvFailedException(int pos, int size);

            STRING print();

            int pos_;
            int size_;
        };
    } // namespace err
} // namespace marvel

#endif //MARVELCODING_MARVELEXCEPTION_H
