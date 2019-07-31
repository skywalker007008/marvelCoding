/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include <cstring>
#include <netinet/in.h>
#include "marvel_socket.h"
#include "marvel_constant.h"

using namespace marvel;

void PackSockaddr(struct sockaddr_in *sockaddr, int family, uint32_t host, uint16_t port) {
    memset(sockaddr, 0, ADDRIN_SIZE);
    sockaddr -> sin_family = family;
    sockaddr -> sin_addr.s_addr = host;
    sockaddr -> sin_port = htons(port);
}
