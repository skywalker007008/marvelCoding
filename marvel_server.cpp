/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include "marvel_server.h"
#include "marvel_exception.h"
#include <unistd.h>
#include "marvel_log.h"
#include "marvel_socket.h"
#include "codec/header.h"

MARVEL_SERVER::MarvelServer(
        MARVEL_APP* app, uint32_t host, uint16_t port)
        :host_(host), port_(port), app_(app) {
    codec_num_ = 0;
    codec_ = (CODEC**)malloc(MARVEL kMaxCacheSize * sizeof(CODEC*));
    map_header_ = (HeaderSymbol**)malloc(MARVEL kMaxCacheSize * sizeof(HeaderSymbol*));
    for (int i = 0; i < MARVEL kMaxCacheSize; i++) {
        codec_status_[i] = false;
    }
}

MARVEL_SERVER::~MarvelServer() {
}

ssize_t MARVEL_SERVER::RecvProcess(char* msg, Address* host, uint16_t* port) { //
    int serv_socket;
    struct sockaddr_in serv_addr;
    ssize_t length = 0;

    try {
        // create a socket
        serv_socket = NewSocket(app_->get_stream());
        std::cout << "new socket\n";
        // add details
        // PackSockaddr(&serv_addr, AF_INET, host_, port_);
        PackSockaddr(&serv_addr, AF_INET, htonl(INADDR_ANY), port_);
        std::cout << "pack socket\n";
        // bind the socket
        BindSocket(app_-> get_stream(), serv_socket, &serv_addr);
        std::cout << "bind socket\n";
        /*// now listen
        ListenSocket(app_->get_stream(), serv_socket, &serv_addr);
        std::cout << "listen socket\n";*/
    } catch (MARVEL_ERR MarvelException exp) {
        throw exp;
    }

    // now recv
    try {
        length = RecvMessage(serv_socket, &serv_addr, host, port, msg);
    } catch (MARVEL_ERR SocketAcceptFailedException exp) {
        throw exp; // ???
    } catch (MARVEL_ERR MessageRecvFailedException exp) {
        throw exp; // ???
    }
    return length;

}

ssize_t MARVEL_SERVER::RecvMessage(
        int serv_socket, struct sockaddr_in* serv_addr, Address* host, uint16_t* port, char* msg) {
    socklen_t clnt_addr_size = ADDR_SIZE;
    struct sockaddr_in clnt_addr;
    ssize_t length;
    int recv_bytes = 0;
    EbrHeaderMsg* header_msg;
    char* msg_buf;
    char* header_msg_buf = (char*)malloc(HEADER_MSG_SIZE);

    while (true) {
        length = recvfrom(serv_socket, header_msg_buf, HEADER_MSG_SIZE, 0, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
        if (length < 0) {
            throw MARVEL_ERR MessageRecvFailedException(recv_bytes, PER_TRANS_SIZE);
        }
#ifdef MARVELCODING_DEBUG_H
        std::cout << "Recv Message\n";
#endif
        header_msg = (EbrHeaderMsg*)header_msg_buf;
        if ((header_msg->header).type == MSG_TYPE || (header_msg->header).type == RESEND_MSG_TYPE) {
            if (MatchAddr(header_msg)) {
                msg_buf = LoadHeader(header_msg);
                if (msg_buf != nullptr) {
                    recv_bytes = (header_msg -> header).length * (header_msg -> header).pacsum;
                    memcpy(msg, msg_buf, recv_bytes);
                    memcpy(host, &(header_msg -> header).sourceaddr, sizeof(Address));
                    *port = (header_msg -> header).sourceport;
                    return recv_bytes;
                }
            } else {
                if (AbleToTransfer(header_msg)) {
                    TransferMessage(header_msg);
                }
            }
        } else if ((header_msg->header).type == RESEND_TYPE) {
            // TODO: Require RESEND
        } else if ((header_msg->header).type == ACK_TYPE) {
            // TODO: ACK
        }
    }
}

OFSTREAM* MARVEL_SERVER::GetStream() {
    return app_->get_stream();
}

void MARVEL_SERVER::shutdown() {

}

void MARVEL_SERVER::start() {
    /*try {
        RecvProcess();
    } catch (MARVEL_ERR MarvelException exp) {
        app_->HandleException(exp);
    }*/
}

bool MARVEL_SERVER::MatchAddr(EbrHeaderMsg* header_msg) {
    if (((header_msg -> header).destaddr).host == host_ && (header_msg -> header).destport == port_) {
        return true;
    } else {
        return false;
    }
}

char* MARVEL_SERVER::LoadHeader(EbrHeaderMsg* header_msg) {
    // TODO: Find the codec for the header
    CODEC* codec = FindCodec(header_msg);
    if (codec == nullptr) {
        return nullptr;
    }
    // TODO: Insert msg and coef into codec
    codec -> RecvMessage(header_msg -> payload, header_msg -> coef);
    if (codec -> is_enough()) {
        return nullptr;
    }
    if (codec -> LinkMsg()) {
        codec -> decode();
        char* msg = (char*)malloc(codec -> get_vec_size() * codec -> get_packet_size() * sizeof(char));
        codec -> get_decode_message(msg);
        // TODO: Send Message to App
        // app_ -> RecvMessage(msg, header_msg);
        // TODO: whether free this codec
        return msg;
    } else {
        return nullptr;
    }
}

CODEC* MARVEL_SERVER::FindCodec(EbrHeaderMsg* header_msg) {
    uint16_t num = ((header_msg -> header).strnum);
    int pl;
    for (pl = 0; pl < MARVEL kMaxCacheSize; pl++) {
        if (codec_status_[pl] && IsMatchHeader(header_msg, pl)) {
            return codec_[pl];
        }
    }
    if (codec_num_ == MARVEL kMaxCacheSize) {
        return nullptr;
    }
    // TODO: Init a codec
    for (pl = 0; pl < MARVEL kMaxCacheSize; pl++) {
        if (!codec_status_[pl]) {
            break;
        }
    }

    CODEC* codec = new CODEC((header_msg -> header).pacsum, (header_msg -> header).length);
    codec_[pl] = codec;
    codec_status_[pl] = true;
    HeaderSymbol* header_symbol;
    header_symbol = (HeaderSymbol*) malloc(sizeof(HeaderSymbol));
    header_symbol -> strnum = num;
    header_symbol -> destport = (header_msg -> header).destport;
    header_symbol -> destaddr = (header_msg -> header).destaddr;
    header_symbol -> sourceport = (header_msg -> header).sourceport;
    header_symbol -> sourceaddr = (header_msg -> header).sourceaddr;
    map_header_[pl] = header_symbol;
    codec_num_++;
    return codec;
}

bool MARVEL_SERVER::IsMatchHeader(EbrHeaderMsg* header_msg, int pl) {
    HeaderSymbol* header_symbol = map_header_[pl];
    if (header_symbol -> destaddr.host == (header_msg -> header).destaddr.host
        && header_symbol -> sourceaddr.host == (header_msg -> header).sourceaddr.host
        && header_symbol -> destport == (header_msg -> header).destport
        && header_symbol -> sourceport == (header_msg -> header).sourceport
        && header_symbol -> strnum == (header_msg -> header).strnum) {
        return true;
    } else {
        return false;
    }
}

void MARVEL_SERVER::TransferMessage(EbrHeaderMsg* header_msg) {
    CODEC* codec = FindCodec(header_msg);
    uint8_t num;
    GFType** coef;
    if (codec == nullptr) {
        codec = new CODEC(1, (header_msg -> header).length);
        codec -> RecvMessage(header_msg -> payload, header_msg -> coef);
        coef = codec -> encode();
        num = 1;
    } else {
        if (codec -> is_enough()) {
            return;
        }
        codec -> RecvMessage(header_msg -> payload, header_msg -> coef);

        // TODO: Signal to encode message.

        // TODO: Transfer the message.
        coef = codec -> encode();
        num = codec -> get_recv_num();

    }
    char *msg = (char*)malloc(num * (header_msg -> header).length);
    // EbrHeader* header_new = CopyEbrHeader(header);
    // memset(header_new -> payload, 0, header_new -> strnum);
    codec -> get_encode_message(msg);
    int sock = NewSocket(app_ -> get_stream());
    int pack_size = (header_msg -> header).length;

    try {
        for (uint8_t i = 0; i < num; i++) {
            EbrHeaderMsg* header_msg_new = CopyEbrHeaderMsg(header_msg);
            (header_msg -> header).pacsum = num;
            (header_msg -> header).pacnum = i;
            // TODO: Add codenumber
            (header_msg_new -> header).codenumber++;
            memcpy(header_msg_new -> payload, msg + i * pack_size, pack_size);
            memcpy(header_msg_new -> coef, coef[i], num * sizeof(GFType));
            sendto(sock, header_msg_new, HEADER_MSG_SIZE, 0, (struct sockaddr*)&broadcast_addr, ADDR_SIZE);
            //send_bytes = sendMessage(sock, header);
        }
    } catch (MARVEL_ERR MarvelException exp) {
        throw exp;
    }
    free(msg);
}

bool MARVEL_SERVER::AbleToTransfer(EbrHeaderMsg* header_msg) {
    return true;
}
