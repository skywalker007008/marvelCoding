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
#include "../exception/marvel_exception.h"
#include <unistd.h>
#include "../include/marvel_log.h"
#include "marvel_socket.h"
#include "../codec/header.h"

#ifdef MARVEL_DEBUG

#include "../include/debug.h"

#endif



MARVEL_SERVER::MarvelServer(
        MARVEL_APP* app, uint32_t host, uint16_t port)
        :host_(host), port_(port), app_(app) {
    codec_num_ = 0;
    codec_ = (CODEC**)malloc(MARVEL kMaxCacheSize * sizeof(CODEC*));
    map_header_ = (HeaderSymbol**)malloc(MARVEL kMaxCacheSize * sizeof(HeaderSymbol*));
    for (int i = 0; i < MARVEL kMaxCacheSize; i++) {
        codec_status_[i] = false;
    }
#ifdef MARVELCODING_QUEUE_H
    TAILQ_INIT(&server_cache_list_);
#endif
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
        std::cout << pthread_self() << "pack socket\n";
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
        length = recvfrom(serv_socket, header_msg_buf, HEADER_MSG_SIZE, 0, (struct sockaddr *) &clnt_addr,
                          &clnt_addr_size);
        if (length < 0) {
            throw MARVEL_ERR MessageRecvFailedException(recv_bytes, PER_TRANS_SIZE);
        }

        header_msg = (EbrHeaderMsg *) header_msg_buf;
#ifdef MARVELCODING_DEBUG_H
        log_recv_message(header_msg);
#endif
        if ((header_msg->header).type == MSG_TYPE || (header_msg->header).type == RESEND_MSG_TYPE) {
            if (MatchAddr(header_msg)) {
                msg_buf = LoadHeader(header_msg);
                if (msg_buf != nullptr) {
                    recv_bytes = (header_msg->header).length * (header_msg->header).pacsum;
                    memcpy(msg, msg_buf, recv_bytes);
                    memcpy(host, &(header_msg->header).sourceaddr, sizeof(Address));
                    *port = (header_msg->header).sourceport;
                    close(serv_socket);
#ifdef MARVELCODING_DEBUG_H
                    log_recv_final_message(header_msg, msg, recv_bytes);
#endif // MARVELCODING_DEBUG_H
                    return recv_bytes;
                }
            } else {
                if (AbleToTransfer(header_msg)) {
                    TransferMessage(header_msg);
                }
            }
        }
#ifdef MARVEL_TCP
        else if ((header_msg->header).type == RESEND_TYPE) {
            // TODO: Require RESEND
        }
#endif
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

#ifdef MARVELCODING_QUEUE_H
    ServerCacheHeaderMsg *header;
    TAILQ_FOREACH(header, &server_cache_list_, cache_link) {
        if (MatchServerCacheMsg(header, header_msg)) {
            break;
        }
    }
    if (header == nullptr) {
        header = (ServerCacheHeaderMsg *) malloc(sizeof(ServerCacheHeaderMsg));
        NewServerCacheMsg(header_msg, header);
        std::thread t(MARVEL_SERVER::AskResend, this);
        TAILQ_INSERT_TAIL(&server_cache_list_, header, cache_link);
    }
    if (!header->recv[(header_msg->header).pacnum]) {
        header->codec->RecvMessage(header_msg->payload, header_msg->coef);
        (header->recvnum)++;
    }
    if (header->recvnum == header->size) {
        header->codec->decode();
        char *msg = (char *) malloc(MARVEL kMaxMsgLength);
        header->codec->get_decode_message(msg);
        if (TAILQ_FIRST(&server_cache_list_) == header) {
            TAILQ_FIRST(&server_cache_list_) = TAILQ_NEXT(header, cache_link);
        }
        TAILQ_REMOVE(&server_cache_list_, header, cache_link);
        return msg;
    } else {
        return nullptr;
    }
#else
    /*// TODO: Find the codec for the header
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
    }*/
#endif
}

CODEC* MARVEL_SERVER::FindCodec(EbrHeaderMsg* header_msg) {
    /*uint16_t num = ((header_msg -> header).strnum);
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
    header_symbol -> codenumber = (header_msg -> header).codenumber;
    map_header_[pl] = header_symbol;
    codec_num_++;
    return codec;*/
}

bool MARVEL_SERVER::IsMatchHeader(EbrHeaderMsg* header_msg, int pl) {
    /*HeaderSymbol* header_symbol = map_header_[pl];
    if (header_symbol -> destaddr.host == (header_msg -> header).destaddr.host
        && header_symbol -> sourceaddr.host == (header_msg -> header).sourceaddr.host
        && header_symbol -> destport == (header_msg -> header).destport
        && header_symbol -> sourceport == (header_msg -> header).sourceport
        && header_symbol -> strnum == (header_msg -> header).strnum) {
        return true;
    } else {
        return false;
    }*/
    return true;
}

void MARVEL_SERVER::TransferMessage(EbrHeaderMsg* header_msg) {
    GFType** coef;
    int size = (header_msg -> header).pacsum;
    CODEC* codec = new CODEC(size, (header_msg -> header).length);
    codec -> RecvMessage(header_msg -> payload, header_msg -> coef);
    coef = codec -> encode();
    char *msg = (char*)malloc((header_msg -> header).length);
    // EbrHeader* header_new = CopyEbrHeader(header);
    // memset(header_new -> payload, 0, header_new -> strnum);
    codec -> get_encode_message(msg);
    free(codec);
    int sock = NewSocket(app_ -> get_stream());
    int pack_size = (header_msg -> header).length;

    try {
        EbrHeaderMsg *header_msg_new = CopyEbrHeaderMsg(header_msg);
        // (header_msg -> header).pacsum = num;
        // (header_msg -> header).pacnum = i;
        // TODO: Add codenumber
        (header_msg_new->header).codenumber++;
        (header_msg_new->header).range--;
        memcpy(header_msg_new->payload, msg , pack_size);
        memcpy(header_msg_new->coef, coef[0], size * sizeof(GFType));
        sendto(sock, header_msg_new, HEADER_MSG_SIZE, 0, (struct sockaddr *) &broadcast_addr, ADDR_SIZE);
#ifdef MARVELCODING_DEBUG_H
        log_transfer_message(header_msg_new);
#endif // MARVELCODING_DEBUG_H
        free(header_msg_new);

    } catch (MARVEL_ERR MarvelException exp) {
        throw exp;
    }
    free(msg);
    close(sock);
}

bool MARVEL_SERVER::AbleToTransfer(EbrHeaderMsg* header_msg) {
    /*int pl;
    for (pl = 0; pl < MARVEL kMaxCacheSize; pl++) {
        if (codec_status_[pl] && IsMatchHeader(header_msg, pl)) {
            HeaderSymbol *symbol = map_header_[pl];
            if (symbol -> codenumber != (header_msg->header).codenumber) {
                return false;
            } else {
                return true;
            }
        }
    }
    for (pl = 0; pl < MARVEL kMaxCacheSize; pl++) {
        if (!codec_status_[pl]) {
            break;
        }
    }

    HeaderSymbol* header_symbol;
    header_symbol = (HeaderSymbol*) malloc(sizeof(HeaderSymbol));
    header_symbol -> strnum = (header_msg -> header).strnum;
    header_symbol -> destport = (header_msg -> header).destport;
    header_symbol -> destaddr = (header_msg -> header).destaddr;
    header_symbol -> sourceport = (header_msg -> header).sourceport;
    header_symbol -> sourceaddr = (header_msg -> header).sourceaddr;
    header_symbol -> codenumber = (header_msg -> header).codenumber;
    map_header_[pl] = header_symbol;
    codec_status_[pl] = true;*/
#ifdef MARVEL_ROUTE

#else
    return (header_msg->header).range > 0;
#endif
}

#ifdef MARVELCODING_QUEUE_H
void MARVEL_SERVER::RemoveCache() {
    // USE MACRO
    sleep(3000);
    ServerCacheHeaderMsg* header = TAILQ_FIRST(&server_cache_list_);
    TAILQ_FIRST(&server_cache_list_) = TAILQ_NEXT(header, cache_link);
    TAILQ_REMOVE(&server_cache_list_, header, cache_link);
    free(header->codec);
    free(header);
}
#endif

#ifdef MARVELCODING_QUEUE_H
void MARVEL_SERVER::AskResend() {
    // USE MACRO
    sleep(3000);
    ServerCacheHeaderMsg* header = TAILQ_FIRST(&server_cache_list_);
    if (header -> recvnum != header -> size) {
        EbrResendMsg *resend_msg = (EbrResendMsg *) malloc(sizeof(EbrResendMsg));
        Address addr;
        addr.host = host_;
        resend_msg = NewEbrResendMsg(header, addr, port_);
        app_ -> SendResendRequest(resend_msg);
    }
    std::thread t(MARVEL_SERVER::RemoveCache, this);
}
#endif