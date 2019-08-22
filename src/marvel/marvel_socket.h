/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#ifndef MARVELCODING_MARVELSOCKET_H
#define MARVELCODING_MARVELSOCKET_H

#include <cstdint>
#include <fstream>
#include "../include/marvel_constant.h"

namespace marvel {
    void PackSockaddr(struct sockaddr_in *sockaddr, sa_family_t family, uint32_t host, uint16_t port);
    int NewSocket(OFSTREAM* stream);
    void BindSocket(OFSTREAM* stream, int sock, struct sockaddr_in* sock_addr);
    void ListenSocket(OFSTREAM* stream, int sock, struct sockaddr_in* sock_addr);
    int AcceptSocket(OFSTREAM* stream, int sock, struct sockaddr_in* sock_addr,
                     struct sockaddr_in* clnt_addr, socklen_t* clnt_addr_size);
}

#endif //MARVELCODING_MARVELSOCKET_H
