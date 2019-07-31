/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#ifndef MARVELCODING_MARVELLOG_H
#define MARVELCODING_MARVELLOG_H

#include <cstdint>
#include <string>
#include "marvel_client.h"
#include "marvel_server.h"

namespace marvel::log {
    namespace server {
        void RecvMessage(MarvelServer *server, struct sockaddr_in* sockaddr,
                         char *msg, int recv_bytes, int total_bytes);
        void SocketAccepted(MarvelServer* server, struct sockaddr_in* sockaddr,
                            struct sockaddr_in* clntaddr);
    } // namespace server
    namespace client {
        void SendMessage(MarvelClient* client, struct sockaddr_in* sockaddr,
                         char *msg, int send_bytes, int total_bytes);
        void SocketConnected(MarvelClient* client, struct sockaddr_in* sockaddr);
    } // namespace client
} // namespace marvel::log

#endif //MARVELCODING_MARVELLOG_H
