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
#include "marvel_exception.h"
#include "marvel_log.h"

int MARVEL NewSocket(OFSTREAM* stream) {
    int serv_socket;
    for (int i = 0; i < MAX_RETRY_TIME; i++) {
        serv_socket = socket(AF_INET, SOCK_DGRAM, 0);
        if (serv_socket < 0) {
            MARVEL_LOG log(stream, "Socket Create Failed. Retrying...");
            if (i == MAX_RETRY_TIME) {
                throw MARVEL_ERR SocketCreateFailedException();
            }
        } else {
            break;
        }
    }
    return serv_socket;
}

void MARVEL PackSockaddr(struct sockaddr_in *sockaddr, sa_family_t family, uint32_t host, uint16_t port) {
    memset(sockaddr, 0, ADDRIN_SIZE);
    sockaddr->sin_family = family;
    sockaddr->sin_addr.s_addr = host;
    // sockaddr->sin_port = htons(port);
    sockaddr->sin_port = (port);
}

void MARVEL BindSocket(OFSTREAM* stream, int sock, struct sockaddr_in *sockaddr) {
    for (int i = 0; i < MAX_RETRY_TIME; i++) {
        if (bind(sock, (struct sockaddr*)sockaddr, ADDRIN_SIZE) < 0) {
            MARVEL_LOG log(stream, "Socket Bind Failed. Retrying...");
            // err::errMsg(err::SOCKET_BIND_FAILED);
            if (i == MAX_RETRY_TIME) {
                throw MARVEL_ERR SocketBindFailedException(*sockaddr);
            }
        } else {
            break;
        }
    }
}

void MARVEL ListenSocket(OFSTREAM* stream, int sock, struct sockaddr_in* sock_addr) {
    for (int i = 0; i < MAX_RETRY_TIME; i++) {
        if (listen(sock, MAX_CONNECT_NUM) < 0) {
            MARVEL_LOG log(stream, "Socket Listen Failed. Retrying...");
            if (i == MAX_RETRY_TIME) {
                throw MARVEL_ERR SocketListenFailedException(*sock_addr);
            }
        } else {
            break;
        }
    }
}

int MARVEL AcceptSocket(OFSTREAM* stream, int sock, struct sockaddr_in* sockaddr,
                         struct sockaddr_in* clnt_addr, socklen_t* clnt_addr_size) {
    int clnt_socket = 0;
    for (int i = 0; i < MAX_RETRY_TIME; i++) {
        clnt_socket = accept(sock, (struct sockaddr*)clnt_addr, clnt_addr_size);
        if (clnt_socket < 0) {
            if (i == MAX_RETRY_TIME) {
                throw MARVEL_ERR SocketAcceptFailedException(*sockaddr);
            }
        } else {
            break;
        }
    }
    return clnt_socket;
}
