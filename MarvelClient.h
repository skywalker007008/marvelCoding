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
        MarvelClient(const std::string& host, uint16_t port);
        void send();
        void start();

    private:
        std::string host_;
        uint16_t port_;
    };
}

#endif //MARVELCODING_MARVELCLIENT_H