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

extern clock_t base;

namespace marvel {
    namespace log {
        void RecvMessage(OFSTREAM& stream, struct sockaddr_in *sockaddr,
                             char *msg, int recv_bytes, int total_bytes);

        void SocketAccepted(OFSTREAM& stream, struct sockaddr_in *sockaddr,
                                struct sockaddr_in *clntaddr);

        void SendMessage(OFSTREAM& stream, struct sockaddr_in *sockaddr,
                             char *msg, int send_bytes, int total_bytes);

        void SocketConnected(OFSTREAM& stream, struct sockaddr_in *sockaddr);

        void log(OFSTREAM& stream, const STRING &string);

        inline clock_t pass_time();
    } // namespace log
} // namespace marvel

#endif //MARVELCODING_MARVELLOG_H
