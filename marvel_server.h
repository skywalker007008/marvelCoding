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

#ifdef MARVELCODING_DEBUG_H

#include "debug.h"

#endif

// constexpr int SERVER_SIZE = sizeof(class marvel::MarvelServer);


namespace marvel {
    class MarvelApp;
    class MarvelServer {
    public:
        MarvelServer(MarvelApp* app, uint32_t host, uint16_t port);
        ~MarvelServer();
        void start();
        ssize_t RecvProcess(char* msg, Address* host, uint16_t* port);
        OFSTREAM* GetStream();
        void shutdown();
        bool MatchAddr(EbrHeaderMsg* header_msg);
        char* LoadHeader(EbrHeaderMsg* header_msg);
        CODEC* FindCodec(EbrHeaderMsg* header_msg);
        bool IsMatchHeader(EbrHeaderMsg* header_msg, int pl);
        void TransferMessage(EbrHeaderMsg* header_msg);
        bool AbleToTransfer(EbrHeaderMsg* header_msg);

    private:
        ssize_t RecvMessage(int serv_socket, struct sockaddr_in* serv_addr,
                            Address* host, uint16_t* port, char* msg);
        uint32_t host_;
        uint16_t port_;
        MarvelApp* app_;
        CODEC** codec_;
        bool codec_status_[MARVEL kMaxCacheSize];
        HeaderSymbol** map_header_;
        int codec_num_;

    };

    static void StartServer(MarvelServer* server);
}

#endif //MARVELCODING_MARVELSERVER_H
