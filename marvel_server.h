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
#include "marvel_app.h"

// constexpr int SERVER_SIZE = sizeof(class marvel::MarvelServer);


namespace marvel {
    class MarvelApp;
    class MarvelServer {
    public:
        MarvelServer(MarvelApp* app, uint32_t host, uint16_t port);
        ~MarvelServer();
        void start();
        void RecvProcess();
        OFSTREAM* GetStream();
        void shutdown();

    private:
        void RecvMessage(int serv_socket, struct sockaddr_in* serv_addr);
        uint32_t host_;
        uint16_t port_;
        MarvelApp* app_;
    };

    static void StartServer(MarvelServer* server);
}

#endif //MARVELCODING_MARVELSERVER_H
