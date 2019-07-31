/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#ifndef MARVELCODING_MARVELSOCKET_H
#define MARVELCODING_MARVELSOCKET_H

#include <cstdint>

namespace marvel {
    void PackSockaddr(struct sockaddr_in *sockaddr, int family, uint32_t host, uint16_t port);
}

#endif //MARVELCODING_MARVELSOCKET_H
