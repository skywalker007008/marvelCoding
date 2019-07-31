/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include "MarvelServer.h"
#include "MarvelConstant.h"
#include <unistd.h>

using namespace marvel;

MarvelServer::MarvelServer(
        const std::string& host, uint16_t port)
        :host_(host), port_(port) {}

void MarvelServer::start(const char* msg) {
    int serv_socket;
    int clnt_socket;

    int id = INIT_SERVER_ID;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[MAX_BUF_SIZE];
    int length;
    int recv_bytes = 0;

    serv_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serv_socket < 0) {
        err::errMsg(SOCKET_CREATE_FAILED);
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = addr(host_);
    serv_addr.sin_port = htons(port_);

    if (bind(serv_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        err::errMsg(SOCKET_BIND_FAILED);
    }

    if (listen(serv_socket, MAX_CONNECT_NUM) < 0) {
        err::errMsg(SOCKET_LISTEN_FAILED);
    }

    clnt_addr_size = sizeof(struct sockaddr);

    for(int i = 0; i < MAX_CONNECT_NUM; i++) {
        recv_bytes = 0;
        clnt_socket = accept(serv_socket, (sockaddr*)&clnt_addr, &clnt_addr_size);
        if (clnt_socket < 0) {
            err::errMsg(ACCEPT_FAILED);
        }

        while ((length = recv(clnt_socket, message, PER_TRANS_SIZE, 0)) != 0) {
            if (length < 0) {
                err::errMsg(RECV_FAILED);
            }
            recv_bytes += length;
            log::server::recvMessage(host_, msg, recv_bytes);
        }

        id++;
        close(clnt_socket);
        // shutdown(clnt_socket, SHUT_RDWR);

    }
    close(serv_socket);
    // shutdown(serv_socket, SHUT_RDWR);
}

