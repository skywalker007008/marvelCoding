/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#ifndef MARVELCODING_MARVELSERVER_H
#define MARVELCODING_MARVELSERVER_H

#include <cstdint>
#include <string>
#include <sys/socket.h>
#include <fstream>
#include "marvel_constant.h"

constexpr int SERVER_SIZE = sizeof(class marvel::MarvelServer);

namespace marvel {
    template <class APP> class MarvelServer {
    public:
        MarvelServer(APP* app, uint32_t host, uint16_t port);
        ~MarvelServer();
        void start();
        void RecvProcess();
        std::ofstream GetStream();
        void shutdown();

    private:
        void RecvMessage(int serv_socket, struct sockaddr_in* serv_addr);
        uint32_t host_;
        uint16_t port_;
        APP* app_;
    };
}

#endif //MARVELCODING_MARVELSERVER_H
