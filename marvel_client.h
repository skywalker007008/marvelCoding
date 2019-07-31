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
#include <fstream>

namespace marvel {
    class MarvelClient {
    public:
        MarvelClient(const std::string& name, uint32_t host, uint16_t port);
        ~MarvelClient();
        void sendMessage(int sock, char* msg, struct sockaddr_in* serv_addr);
        void start(uint32_t host, uint16_t port, const char* msg);
        std::ofstream GetStream();

    private:
        std::string name_;
        uint32_t host_;
        uint16_t port_;
        std::ofstream stream_;
    };
}

#endif //MARVELCODING_MARVELCLIENT_H