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
#include "marvel_log.h";

using namespace marvel;

int marvel::NewSocket(std::ofstream stream) {
    int serv_socket;
    for (int i = 0; i < MAX_RETRY_TIME; i++) {
        serv_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (serv_socket < 0) {
            log::log(stream, "Socket Create Failed. Retrying...");
            if (i == MAX_RETRY_TIME) {
                throw err::SocketCreateFailedException();
            }
        } else {
            break;
        }
    }
    return serv_socket;
}

void marvel::PackSockaddr(struct sockaddr_in *sockaddr, int family, uint32_t host, uint16_t port) {
    memset(sockaddr, 0, ADDRIN_SIZE);
    sockaddr->sin_family = family;
    sockaddr->sin_addr.s_addr = host;
    sockaddr->sin_port = htons(port);
}

void marvel::BindSocket(std::ofstream stream, int sock, struct sockaddr_in *sockaddr) {
    for (int i = 0; i < MAX_RETRY_TIME; i++) {
        if (bind(sock, (struct sockaddr*)sockaddr, ADDRIN_SIZE) < 0) {
            log::log(stream, "Socket Bind Failed. Retrying...");
            // err::errMsg(err::SOCKET_BIND_FAILED);
            if (i == MAX_RETRY_TIME) {
                throw err::SocketBindFailedException(*sockaddr);
            }
        } else {
            break;
        }
    }
}

void marvel::ListenSocket(std::ofstream stream, int sock, struct sockaddr_in* sock_addr) {
    for (int i = 0; i < MAX_RETRY_TIME; i++) {
        if (listen(sock, MAX_CONNECT_NUM) < 0) {
            log::log(stream, "Socket Listen Failed. Retrying...");
            if (i == MAX_RETRY_TIME) {
                throw err::SocketListenFailedException(*sock_addr);
            }
        } else {
            break;
        }
    }
}

int marvel::AcceptSocket(std::ofstream stream, int sock, struct sockaddr_in* sockaddr,
                         struct sockaddr_in* clnt_addr, socklen_t* clnt_addr_size) {
    int clnt_socket = 0;
    for (int i = 0; i < MAX_RETRY_TIME; i++) {
        clnt_socket = accept(sock, (struct sockaddr*)clnt_addr, clnt_addr_size);
        if (clnt_socket < 0) {
            if (i == MAX_RETRY_TIME) {
                throw err::SocketAcceptFailedException(*sockaddr);
                // err::errMsg(err::ACCEPT_FAILED);
            }
        } else {
            break;
        }
        // log::server::SocketAccepted(this, serv_addr, &clnt_addr);
    }
    return clnt_socket;
}
