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
#include "marvel_exception.h"
#include "marvel_log.h"
#include "marvel_socket.h"

template <typename APP>

marvel::MarvelClient::MarvelClient(APP* app, uint32_t host, uint16_t port)
        :host_(host), port_(port), app_(app) {
}

marvel::MarvelClient::~MarvelClient() {
}

void marvel::MarvelClient::SendProcess(uint32_t host, uint16_t port, const char* msg) {
    int sock;
    struct sockaddr_in serv_addr;
    char message[MAX_BUF_SIZE + 1];

    // check if msg longer than maxmium length
    if (strlen(msg) > MAX_BUF_SIZE) {
        throw marvel::err::MessageOversizedException();
    } else {
        strcpy(message, msg);
    }

    // create a socket
    for (int i = 0; i < MAX_RETRY_TIME; i++) {
        // check if socket created successfully
        sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sock < 0) {
            app_ -> log("Socket Create Failed. Retrying...");
            // log::log(stream_, "Socket Create Failed. Retrying...");
            // marvel::err::errMsg(err::SOCKET_CREATE_FAILED);
            if (i == MAX_RETRY_TIME) {
                throw err::SocketCreateFailedException();
            }
        }
        else {
            break;
        }
    }

    // init server's address
    PackSockaddr(&serv_addr, AF_INET, host, port);

    // connect
    for (int i = 0; i < MAX_RETRY_TIME; i++) {
        // check if connection successful
        if (connect(sock, (struct sockaddr *) &serv_addr, ADDR_SIZE) < 0) {
            app_ -> log("Socket Connect Failed. Retrying...");
            if (i == MAX_RETRY_TIME) {
                throw err::SocketConnectFailedException(serv_addr);
            }
        } else {
            marvel::log::client::SocketConnected(this, &serv_addr);
            break;
        }
    }

    // start-to-send
    try {
        sendMessage(sock, message, &serv_addr);
    } catch (err::MarvelException exp) {
        throw exp;
    }

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
            if (sendBytes < 0) { // no Retry, only stop here
                throw err::MessageSendFailedException(
                        length - remain - PER_TRANS_SIZE, PER_TRANS_SIZE);
                // marvel::err::errMsg(SEND_FAILED); // miss
                // exit(1); // no need
            } else if (sendBytes < PER_TRANS_SIZE) {
                log::log(stream_, "Message Sent Loss. Lost size: "
                                  + std::to_string(PER_TRANS_SIZE - sendBytes));
                // marvel::err::errMsg(SEND_INCOMPLETE); // loss
                // exit(1); // actually no need
            }
            msg += PER_TRANS_SIZE;
        } else {
            sendBytes = send(sock, msg, remain, 0);
            marvel::log::client::SendMessage(
                    this, serv_addr, msg, sendBytes, length - remain);
            if (sendBytes < 0) {
                throw err::MessageSendFailedException(length - remain, remain);
                // marvel::err::errMsg(SEND_FAILED); // miss
                // exit(1); // no need
            } else if (sendBytes < remain) {
                log::log(stream_, "Message Sent Loss. Lost size: "
                                  + std::to_string(remain - sendBytes));
                // marvel::err::errMsg(SEND_INCOMPLETE); // loss
                // exit(1); // actually no need
            }
            break;
        }
    }
}

std::ofstream marvel::MarvelClient::GetStream() {
    return app_ -> get_stream();
}


