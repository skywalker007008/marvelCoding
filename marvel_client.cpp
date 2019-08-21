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
#include <unistd.h>
#include "marvel_exception.h"
#include "marvel_log.h"
#include "marvel_socket.h"
#include "codec/header.h"


MARVEL_CLIENT::MarvelClient(MARVEL_APP* app, uint32_t host, uint16_t port)
        :host_(host), port_(port), app_(app) {
    addr_.host = host;
    BuildCodec();
    id_ = 0;
}

MARVEL_CLIENT::~MarvelClient() {
}

void MARVEL_CLIENT::BuildCodec() {
    // _codec = new CODEC_LIB();
}

ssize_t MARVEL_CLIENT::SendProcess(uint32_t host, uint16_t port, char* msg, int packet_size) {
    int sock;
    ssize_t send_bytes = 0;
    size_t len = ROUND(strlen(msg), packet_size);
    int packet_sum = len / packet_size;
    char* message = (char*)malloc(len * sizeof(char));
    GFType** coef;

    // check if msg longer than maxmium length
    if (strlen(msg) > kMaxMsgLength) {
        throw MARVEL_ERR MessageOversizedException();
    } else {
        // add encode
        CODEC codec(packet_sum, packet_size);
        for (int i = 0; i < packet_sum; i++) {
            codec.RecvMessage(msg + i * packet_size, std_coef[i]);
        }
        coef = codec.encode();
        codec.get_encode_message(message);
    }

    // create a socket
    for (int i = 0; i < MAX_RETRY_TIME; i++) {
        // check if socket created successfully
        sock = socket(AF_INET, SOCK_DGRAM, 0);
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

    Address dest_addr;
    dest_addr.host = host;

    // start-to-send
    try {
        for (int i = 0; i < packet_sum; i++) {
            EbrHeaderMsg* header_msg = NewEbrHeaderMsg(MSG_TYPE, 0, 0, 0,
                                             packet_sum, id_, i,
                                             addr_, dest_addr, port_, port,
                                             packet_size, nullptr, message + i * packet_size, coef[i]);
            send_bytes += sendMessage(sock, header_msg);
        }
    } catch (MARVEL_ERR MarvelException exp) {
        throw exp;
    }
    id_++;
    close(sock);
    return send_bytes;

}

ssize_t MARVEL_CLIENT::sendMessage(int sock, EbrHeaderMsg* header_msg) {
    /*int length = strlen(msg);
    int remain = length;
    int totalBytes = 0;
    int sendBytes;*/
    ssize_t send_bytes;
    send_bytes = sendto(sock, header_msg, HEADER_MSG_SIZE, 0, (struct sockaddr*)&broadcast_addr, sizeof(struct sockaddr));
    // sendto(sock, header_msg, HEADER_MSG_SIZE, 0);
    /*while (remain > 0) {
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
    }*/
    return send_bytes;
}

OFSTREAM* MARVEL_CLIENT::GetStream() {
    return app_ -> get_stream();
}

void MARVEL_CLIENT::shutdown() {

}

void MARVEL_CLIENT::start() {

}



