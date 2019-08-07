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

MARVEL_CLIENT::MarvelClient(MARVEL_APP* app, uint32_t host, uint16_t port)
        :host_(host), port_(port), app_(app) {
    BuildCodec();
}

MARVEL_CLIENT::~MarvelClient() {
}

void MARVEL_CLIENT::BuildCodec() {
    _codec = new CODEC_LIB();
}

int MARVEL_CLIENT::SendProcess(uint32_t host, uint16_t port, uint8_t num, const char* msg) {
    int sock;
    struct sockaddr_in serv_addr;
    int send_bytes = 0;
    char message[MAX_BUF_SIZE + 1];

    // check if msg longer than maxmium length
    if (strlen(msg) > MAX_BUF_SIZE) {
        throw MARVEL_ERR MessageOversizedException();
    } else {
        strcpy(message, msg);
    }

    // create a socket
    for (int i = 0; i < MAX_RETRY_TIME; i++) {
        // check if socket created successfully
        sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sock < 0) {
            app_ -> log("Socket Create Failed. Retrying...");
            if (i == MAX_RETRY_TIME) {
                throw MARVEL_ERR SocketCreateFailedException();
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
                throw MARVEL_ERR SocketConnectFailedException(serv_addr);
            }
        } else {
            MARVEL_LOG SocketConnected(GetStream(), &serv_addr);
            break;
        }
    }

    // start-to-send
    try {
        send_bytes = sendMessage(sock, message, &serv_addr);
    } catch (MARVEL_ERR MarvelException exp) {
        throw exp;
    }
    return send_bytes;

}

int MARVEL_CLIENT::sendMessage(int sock, char* msg, struct sockaddr_in* serv_addr) {
    int length = strlen(msg);
    int remain = length;
    int totalBytes = 0;
    int sendBytes;
    while (remain > 0) {
        if (remain >= PER_TRANS_SIZE) {
            sendBytes = send(sock, msg, PER_TRANS_SIZE, 0);
            remain -= PER_TRANS_SIZE;
            MARVEL_LOG SendMessage(
                    GetStream(), serv_addr, msg, sendBytes, length - remain);
            if (sendBytes < 0) { // no Retry, only stop here
                throw MARVEL_ERR MessageSendFailedException(
                        length - remain - PER_TRANS_SIZE, PER_TRANS_SIZE);
            } else if (sendBytes < PER_TRANS_SIZE) {
                MARVEL_LOG log(GetStream(), "Message Sent Loss. Lost size: "
                                  + TO_STRING(PER_TRANS_SIZE - sendBytes));
            }
            msg += PER_TRANS_SIZE;
            totalBytes += sendBytes;
        } else {
            sendBytes = send(sock, msg, remain, 0);
            MARVEL_LOG SendMessage(
                    GetStream(), serv_addr, msg, sendBytes, length - remain);
            if (sendBytes < 0) {
                throw MARVEL_ERR MessageSendFailedException(length - remain, remain);
            } else if (sendBytes < remain) {
                MARVEL_LOG log(GetStream(), "Message Sent Loss. Lost size: "
                                  + TO_STRING(remain - sendBytes));
            }
            totalBytes += sendBytes;
            break;
        }
    }
    return totalBytes;
}

OFSTREAM* MARVEL_CLIENT::GetStream() {
    return app_ -> get_stream();
}

void MARVEL_CLIENT::shutdown() {

}

void MARVEL_CLIENT::start() {

}



