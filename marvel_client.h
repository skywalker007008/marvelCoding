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
#include "marvel_constant.h"
#include "marvel_app.h"
#include "codec/codec.h"

// constexpr int CLIENT_SIZE = sizeof(marvel::MarvelClient);

namespace marvel {
    class MarvelApp;
    class MarvelClient {
    public:
        MarvelClient(MarvelApp* app, uint32_t host, uint16_t port);
        ~MarvelClient();
        int sendMessage(int sock, char* msg, struct sockaddr_in* serv_addr);
        int SendProcess(uint32_t host, uint16_t port, const char* msg);
        void BuildCodec();
        OFSTREAM* GetStream();
        void shutdown();
        void start();

    private:
        CODEC_LIB* _codec;
        MarvelApp* app_;
        uint32_t host_;
        uint16_t port_;
    };

    static void StartClient(MarvelClient* client);
}

#endif //MARVELCODING_MARVELCLIENT_H