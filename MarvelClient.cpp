/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include <string>
#include <netinet/in.h>
#include <cstring>
#include "MarvelClient.h"
#include "MarvelConstant.h"

marvel::MarvelClient(const std::string& host, uint16_t port)
        :host_(host), port_(port) {}

void marvel::MarvelClient::start(const char* msg) {
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct sockaddr_in serv_addr;
    char message[MAX_BUF_SIZE];

    int str_len;
    int idx;
    int read_len;

    // check if socket created successfully
    if (sock == -1) {
        marvel::err::errMsg(SOCKET_CREATE_FAILED);
        exit(1);
    }

    // check if msg longer than maxmium length

    if (strlen(msg) > MAX_BUF_SIZE) {
        marvel::err::errMsg(DATA_OVERSIZED);
        exit(1);
    } else {
        strcpy(message, msg);
    }

    // init server's address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = addr(host_);
    serv_addr.sin_port = htons(port_);

    // check if connection successful
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        marvel::err::errMsg(SOCKET_CONNECT_FAILED);
        exit(1);
    } else {
        marvel::log::socket_connected(&serv_addr);
    }

    // start-to-send
    sendMessage(sock, msg);


}

void marvel::MarvelClient::sendMessage(int sock, char* msg) {
    int remain = strlen(msg);
    int sendBytes;
    while (remain > 0) {
        if (remain >= PER_TRANS_SIZE) {
            sendBytes = send(sock, msg, PER_TRANS_SIZE, 0);
            marvel::log::client::sendMessage(host_, msg, PER_TRANS_SIZE);
            if (sendBytes < 0) {
                marvel::err::errMsg(SEND_FAILED); // miss
                exit(1); // no need
            } else if (sendBytes < PER_TRANS_SIZE) {
                marvel::err::errMsg(SEND_INCOMPLETE); // loss
                exit(1); // actually no need
            }
            msg += PER_TRANS_SIZE;
            remain -= PER_TRANS_SIZE;
        } else {
            sendBytes = send(sock, msg, remain, 0);
            marvel::log::client::sendMessage(host_, msg, remain);
            if (sendBytes < 0) {
                marvel::err::errMsg(SEND_FAILED); // miss
                exit(1); // no need
            } else if (sendBytes < remain) {
                marvel::err::errMsg(SEND_INCOMPLETE); // loss
                exit(1); // actually no need
            }
            break;
        }
    }
}


