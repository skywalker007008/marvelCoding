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
#include "../exception/marvel_exception.h"
#include "../include/marvel_log.h"
#include "marvel_socket.h"

#ifdef MARVEL_DEBUG

#include "../include/debug.h"
#include "../include/alloc.h"

#endif

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
#ifdef MARVELCODING_DEBUG_H
    log_send_init_message(host, port, msg, strlen(msg), id_);
#endif

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
    int id = id_;

    // start-to-send
    try {
        for (int i = 0; i < packet_sum; i++) {
            EbrHeaderMsg* header_msg = NewEbrHeaderMsg(MSG_TYPE, 0, 0, 0,
                                             packet_sum, id_, i,
                                             addr_, dest_addr, port_, port,
                                             packet_size, nullptr, message + i * packet_size, coef[i]);
            send_bytes += sendMessage(sock, header_msg);
            free(header_msg);
        }
    } catch (MARVEL_ERR MarvelException exp) {
        throw exp;
    }
    id++;
    id_ = id;
    close(sock);
#ifdef MARVEL_TCP
    try {
        AddCache(message, coef, id_, dest_addr, port, packet_size, packet_sum);
    } catch (MARVEL_ERR AppCacheFullException exp) {
        throw exp;
    }
#endif
    free_array2((void**)coef, packet_sum);
    return send_bytes;
}

ssize_t MARVEL_CLIENT::sendMessage(int sock, EbrHeaderMsg* header_msg) {

    ssize_t send_bytes;
    send_bytes = sendto(sock, header_msg, HEADER_MSG_SIZE, 0, (struct sockaddr*)&broadcast_addr, sizeof(struct sockaddr));

#ifdef MARVELCODING_DEBUG_H
    log_send_message(header_msg);
#endif // MARVELCODING_DEBUG_H
    return send_bytes;
}

OFSTREAM* MARVEL_CLIENT::GetStream() {
    return app_ -> get_stream();
}

void MARVEL_CLIENT::shutdown() {

}

void MARVEL_CLIENT::start() {

}

void MARVEL_CLIENT::AddCache(char* msg, GFType** coef, uint8_t strnum, Address dest_addr,
                             uint16_t dest_port, uint16_t packet_size, uint8_t packet_sum) {
    // TODO: Add the msg into the str
    ClientCacheHeaderMsg* header_msg = (ClientCacheHeaderMsg*)malloc(sizeof(ClientCacheHeaderMsg));
    NewClientCacheHeaderMsg(header_msg, strnum, packet_sum, packet_size, msg, coef, dest_addr, dest_port);
    try {
        app_->AddCache(header_msg);
    } catch (MARVEL_ERR AppCacheFullException exp) {
        throw exp;
    }
}



