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

namespace marvel::log {
    namespace server {
        void RecvMessage(uint32_t host, uint16_t port, char *msg, int recv_bytes, int total_bytes);
    } // namespace server
    namespace client {
        void SendMessage(uint32_t host, uint16_t port, char *msg,
                         int send_bytes, int total_bytes, std::string &client_name);
    } // namespace client
 // namespace marvel::log

#endif //MARVELCODING_MARVELLOG_H
