/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include <string>
#include <netinet/in.h>
#include <cstring>
#include "marvel_client.h"
#include "marvel_constant.h"
#include "marvel_exception.h"
#include "marvel_log.h"
#include "marvel_socket.h"

marvel::MarvelClient::MarvelClient(const std::string& name, uint32_t host, uint16_t port)
        :host_(host), port_(port), name_(name) {
    std::string file_name = "client_" + name_ + ".txt";
    if (stream_ != nullptr) {
        stream_.open(file_name, std::ios::out);
    }
}

marvel::MarvelClient::~MarvelClient() {
    if (stream_ != nullptr) {
        stream_.close();
    }
}

void marvel::MarvelClient::start(uint32_t host, uint16_t port, const char* msg) {
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct sockaddr_in serv_addr;
    char message[MAX_BUF_SIZE + 1];

    // check if socket created successfully
    if (sock == -1) {
        marvel::err::errMsg(err::SOCKET_CREATE_FAILED);
        exit(1);
    }

    // check if msg longer than maxmium length

    if (strlen(msg) > MAX_BUF_SIZE) {
        marvel::err::errMsg(err::DATA_OVERSIZED);
        exit(1);
    } else {
        strcpy(message, msg);
    }

    // init server's address
    PackSockaddr(&serv_addr, AF_INET, host, port);

    // check if connection successful
    if (connect(sock, (struct sockaddr*)&serv_addr, ADDR_SIZE) < 0) {
        err::SocketException exp(err::SOCKET_CONNECT_FAILED, serv_addr);

        // marvel::err::errMsg(SOCKET_CONNECT_FAILED);
        // exit(1);
    } else {
        marvel::log::client::SocketConnected(this, &serv_addr);
    }

    // start-to-send
    sendMessage(sock, message, &serv_addr);


}

void marvel::MarvelClient::sendMessage(int sock, char* msg, struct sockaddr_in* serv_addr) {
    int length = strlen(msg);
    int remain = length;
    int sendBytes;
    while (remain > 0) {
        if (remain >= PER_TRANS_SIZE) {
            sendBytes = send(sock, msg, PER_TRANS_SIZE, 0);
            remain -= PER_TRANS_SIZE;
            marvel::log::client::SendMessage(
                    this, serv_addr, msg, sendBytes, length - remain);
            if (sendBytes < 0) {
                marvel::err::errMsg(SEND_FAILED); // miss
                exit(1); // no need
            } else if (sendBytes < PER_TRANS_SIZE) {
                marvel::err::errMsg(SEND_INCOMPLETE); // loss
                exit(1); // actually no need
            }
            msg += PER_TRANS_SIZE;
        } else {
            sendBytes = send(sock, msg, remain, 0);
            marvel::log::client::SendMessage(
                    this, serv_addr, msg, sendBytes, length - remain);
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

std::ofstream marvel::MarvelClient::GetStream() {
    return stream_;
}


