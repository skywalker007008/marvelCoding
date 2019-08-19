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
    codec_ = (CODEC**)malloc(256 * sizeof(CODEC*));
    map_header_ = (HeaderSymbol**)malloc(256 * sizeof(HeaderSymbol*));
    for (int i = 0; i < 256; i++) {
        map_codec_[i] = -1;
    }
}

MARVEL_SERVER::~MarvelServer() {
}

void MARVEL_SERVER::RecvProcess() { //
    int serv_socket;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    try {
        // create a socket
        serv_socket = NewSocket(app_->get_stream());
        std::cout << "new socket\n";
        // add details
        PackSockaddr(&serv_addr, AF_INET, host_, port_);
        std::cout << "pack socket\n";
        // bind the socket
        BindSocket(app_->get_stream(), serv_socket, &serv_addr);
        std::cout << "bind socket\n";
        // now listen
        ListenSocket(app_->get_stream(), serv_socket, &serv_addr);
        std::cout << "listen socket\n";
    } catch (MARVEL_ERR MarvelException exp) {
        throw exp;
    }

    // now accept and recv
    try {
        RecvMessage(serv_socket, &serv_addr);
    } catch (MARVEL_ERR SocketAcceptFailedException exp) {
        throw exp; // ???
    } catch (MARVEL_ERR MessageRecvFailedException exp) {
        throw exp; // ???
    }

}

void MARVEL_SERVER::RecvMessage(int serv_socket, struct sockaddr_in* serv_addr) {
    int id = INIT_SERVER_ID;
    int clnt_socket;
    socklen_t clnt_addr_size = ADDR_SIZE;
    struct sockaddr_in clnt_addr;
    int length;
    int recv_bytes = 0;
    EbrHeaderMsg* header_msg;
    for(int i = 0; i < MAX_CONNECT_NUM; i++) {
        recv_bytes = 0;
        try {
            clnt_socket = AcceptSocket(GetStream(), serv_socket,
                                       serv_addr, &clnt_addr, &clnt_addr_size);
        } catch (MARVEL_ERR SocketAcceptFailedException exp) {
            throw exp;
        }
        MARVEL_LOG SocketAccepted(GetStream(), serv_addr, &clnt_addr);

        while ((length = recv(clnt_socket, header_msg, HEADER_MSG_SIZE, 0)) != 0) {
            if (length < 0) {
                throw MARVEL_ERR MessageRecvFailedException(recv_bytes, PER_TRANS_SIZE);
            }
            if ((header_msg -> header).type == MSG_TYPE) {
                if (MatchAddr(header_msg)) {
                    LoadHeader(header_msg);
                } else {
                    TransferMessage(header_msg);
                }
            }
        }

        id++;
        // close(clnt_socket);
        // shutdown(clnt_socket, SHUT_RDWR);

    }
    // close(serv_socket);
    // shutdown(serv_socket, SHUT_RDWR);
}

OFSTREAM* MARVEL_SERVER::GetStream() {
    return app_->get_stream();
}

void MARVEL_SERVER::shutdown() {

}

void MARVEL_SERVER::start() {
    try {
        RecvProcess();
    } catch (MARVEL_ERR MarvelException exp) {
        app_->HandleException(exp);
    }
}

bool MARVEL_SERVER::MatchAddr(EbrHeaderMsg* header_msg) {
    if (((header_msg -> header).destaddr).host == host_ && (header_msg -> header).destport == port_) {
        return true;
    } else {
        return false;
    }
}

void MARVEL_SERVER::LoadHeader(EbrHeaderMsg* header_msg) {
    // TODO: Find the codec for the header
    CODEC* codec = FindCodec(header_msg);
    // TODO: Insert msg and coef into codec
    codec -> RecvMessage(header_msg -> payload, header_msg -> coef);
    if (codec -> is_enough()) {
        return;
    }
    if (codec -> LinkMsg()) {
        codec -> decode();
        char* msg = (char*)malloc(codec -> get_vec_size() * codec -> get_packet_size() * sizeof(char));
        codec->get_decode_message(msg);
        // TODO: Send Message to App
        app_ -> RecvMessage(msg, header_msg);
        // TODO: whether free this codec
    }
}

CODEC* MARVEL_SERVER::FindCodec(EbrHeaderMsg* header_msg) {
    uint16_t num = ((header_msg -> header).strnum);
    int pl = map_codec_[num];
    if (pl == -1 || !IsMatchHeader(header_msg, pl)) {
        // TODO: Init a codec
        map_codec_[num] = codec_num_;
        CODEC* codec = new CODEC((header_msg -> header).pacsum, (header_msg -> header).length);
        codec_[codec_num_] = codec;
        HeaderSymbol* header_symbol;
        header_symbol = (HeaderSymbol*) malloc(sizeof(HeaderSymbol));
        header_symbol -> strnum = num;
        header_symbol -> destport = (header_msg -> header).destport;
        header_symbol -> destaddr = (header_msg -> header).destaddr;
        header_symbol -> sourceport = (header_msg -> header).sourceport;
        header_symbol -> sourceaddr = (header_msg -> header).sourceaddr;
        map_header_[codec_num_] = header_symbol;
        codec_num_++;
        return codec;
    } else {
        // TODO: Return the codec;
        return codec_[pl];
    }
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
    codec -> RecvMessage(header_msg -> payload, header_msg -> coef);
    uint8_t num;
    // TODO: Signal to encode message.

    // TODO: Transfer the message.
    GFType** coef = codec -> encode();
    num = codec -> get_recv_num();
    char* msg;
    // EbrHeader* header_new = CopyEbrHeader(header);
    // memset(header_new -> payload, 0, header_new -> strnum);
    codec -> get_encode_message(msg);
    int sock = NewSocket(app_ -> get_stream());
    if (connect(sock, (struct sockaddr*)&broadcast_addr, ADDR_SIZE) < 0) {
        app_ -> log("Socket Connect Failed.");
    }
    int pack_size = (header_msg -> header).length;

    try {
        for (uint8_t i = 0; i < num; i++) {
            EbrHeaderMsg* header_msg_new = CopyEbrHeaderMsg(header_msg);
            (header_msg -> header).pacsum = num;
            (header_msg -> header).pacnum = i;
            memcpy(header_msg_new -> payload, msg + i * pack_size, pack_size);
            memcpy(header_msg_new -> coef, coef[i], num * sizeof(GFType));
            send(sock, header_msg_new, HEADER_MSG_SIZE, 0);
            //send_bytes = sendMessage(sock, header);
        }
    } catch (MARVEL_ERR MarvelException exp) {
        throw exp;
    }



}
