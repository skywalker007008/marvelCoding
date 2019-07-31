/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 *
 * This file defines the ops and attrs of the Client
 */

#ifndef MARVELCODING_MARVELCLIENT_H
#define MARVELCODING_MARVELCLIENT_H



#include <sys/socket.h>
#include <cstdint> // useless
#include <string>

namespace marvel {
    class MarvelClient {
    public:
        MarvelClient(uint32_t host, uint16_t port, const std::string& name);
        void sendMessage(int sock, char* msg);
        void start(const char* msg);

    private:
        std::string name_;
        uint32_t host_;
        uint16_t port_;
    };
}

#endif //MARVELCODING_MARVELCLIENT_H