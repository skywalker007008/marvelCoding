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

namespace marvel {
    class MarvelServer {
    public:
        MarvelServer(uint32_t host, uint16_t port, const std::string& name);
        ~MarvelServer();
        void start();
        std::ofstream GetStream();

    private:
        uint32_t host_;
        uint16_t port_;
        std::string name_;
        std::ofstream stream_;
    };
}

#endif //MARVELCODING_MARVELSERVER_H
