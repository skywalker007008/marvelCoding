/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include <cstring>
#include <netinet/in.h>
#include "header.h"
#include <arpa/inet.h>
#include "../include/marvel_constant.h"

struct sockaddr_in broadcast_addr;
struct sockaddr_in any_addr;

#ifdef MARVELCODING_QUEUE_H
// struct ClientCacheList client_cache_list;

#endif

#define bit(n) (1 << (n))

EbrHeaderMsg* NewEbrHeaderMsg(char type, char range, char code_type, char code_number,
                        short pac_sum, short str_num, short pac_num,
                        Address source_addr, Address dest_addr, short source_port, short dest_port,
                        int length, char* check, char* payload, GFType* coef) {
    EbrHeader* header = (EbrHeader*)malloc(HEADER_SIZE);
    header -> type = type;
    header -> range = range;
    header -> codetype = code_type;
    header -> codenumber = code_number;
    header -> pacsum = pac_sum;
    header -> strnum = str_num;
    header -> pacnum = pac_num;
    header -> sourceaddr = source_addr;
    header -> destaddr = dest_addr;
    header -> sourceport = source_port;
    header -> destport = dest_port;
    header -> length = length;
    /*if (check == nullptr) {
        //memset(header -> check, 0, 4);
    }*/
    EbrHeaderMsg* header_msg = (EbrHeaderMsg*)malloc(HEADER_MSG_SIZE);
    header_msg -> header = *header;
    memcpy(header_msg -> payload, payload, MSG_SIZE);
    memcpy(header_msg -> coef, coef, COEF_SIZE);
    return header_msg;
}

void init_addr() {
    broadcast_addr.sin_port = kDefaultPort;
    broadcast_addr.sin_family = AF_INET;
    // broadcast_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    broadcast_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    any_addr.sin_port = kDefaultPort;
    any_addr.sin_family = AF_INET;
    any_addr.sin_addr.s_addr = htonl(INADDR_ANY);
}

EbrHeaderMsg* CopyEbrHeaderMsg(EbrHeaderMsg* header_msg) {
    EbrHeaderMsg* header_msg_new = (EbrHeaderMsg*)malloc(HEADER_MSG_SIZE);
    (header_msg_new -> header).type = (header_msg -> header).type;
    (header_msg_new -> header).range = (header_msg -> header).range;
    (header_msg_new -> header).codetype = (header_msg -> header).codetype;
    (header_msg_new -> header).codenumber = (header_msg -> header).codenumber;
    (header_msg_new -> header).pacsum = (header_msg -> header).pacsum;
    (header_msg_new -> header).strnum = (header_msg -> header).strnum;
    (header_msg_new -> header).pacnum = (header_msg -> header).pacnum;
    (header_msg_new -> header).sourceaddr = (header_msg -> header).sourceaddr;
    (header_msg_new -> header).destaddr = (header_msg -> header).destaddr;
    (header_msg_new -> header).sourceport = (header_msg -> header).sourceport;
    (header_msg_new -> header).destport = (header_msg -> header).destport;
    (header_msg_new -> header).length = (header_msg -> header).length;
    (header_msg_new -> header).check = (header_msg -> header).check;
    // (header_msg_new -> header).check = header -> check;
    //memcpy((header_msg_new -> header).check, (header_msg -> header).check, 4);
    memcpy(header_msg_new -> payload, header_msg -> payload, MSG_SIZE);
    memcpy(header_msg_new -> coef, header_msg -> coef, COEF_SIZE);
    return header_msg_new;
}

#ifdef MARVEL_TCP
EbrResendMsg* NewEbrResendMsg(ServerCacheHeaderMsg* cache_msg, Address sourceaddr, uint16_t sourceport) {
    EbrResendMsg* resend_msg = (EbrResendMsg*)malloc(sizeof(EbrResendMsg));
    EbrHeader header = resend_msg -> header;
    header.pacnum = 0;
    header.pacsum = cache_msg->size;
    header.sourceport = sourceport;
    header.sourceaddr.host = sourceaddr.host;
    header.destaddr.host = cache_msg -> sourceaddr.host;
    header.destport = cache_msg -> sourceport;
    header.strnum = cache_msg -> strnum;
    header.codenumber = 0;

    header.codetype = 0;
    header.range = 0;
    header.type = RESEND_TYPE;
    header.length = cache_msg -> size;

    uint16_t check = ResendValue(cache_msg->recv);
    header.check = check;
    resend_msg->symbol = check;
    return resend_msg;
}

uint16_t ResendValue(bool recv[]) {
    uint16_t check = 0;
    for (int i = 0; i < 16; i++) {
        if (recv[i]) {
            check |= bit(i);
        }
    }
    return check;
}

#endif

void NewClientCacheHeader(ClientCacheHeader* header, uint8_t strnum, Address destaddr, uint16_t destport) {
    header -> strnum = strnum;
    (header -> destaddr).host = destaddr.host;
    header -> destport = destport;
}

void NewClientCacheHeaderMsg(ClientCacheHeaderMsg* header_msg, uint8_t strnum, uint8_t pacsum, uint16_t pacsize,
                             char* msg, GFType** coef, Address destaddr, uint16_t destport) {
    ClientCacheHeader* header = (ClientCacheHeader*)malloc(sizeof(ClientCacheHeader));
    NewClientCacheHeader(header, strnum, destaddr, destport);
    header_msg -> header = header;
    header_msg -> pacsum = pacsum;
    header_msg -> pacsize = pacsize;
    header_msg -> coef = coef;
    header_msg -> msg = msg;
}

void CopyCacheHeaderMsg(ClientCacheHeaderMsg* src_header, ClientCacheHeaderMsg* dest_header) {
    memcpy(dest_header->header, src_header->header, sizeof(ClientCacheHeader));
    uint8_t pacsum = src_header -> pacsum;
    dest_header -> pacsize = src_header -> pacsize;
    dest_header -> pacsum = pacsum;
    alloc_array<char>(dest_header->msg, dest_header->pacsize);
    alloc_array2<GFType>(dest_header->coef, pacsum, pacsum);
    memcpy(dest_header->msg, src_header->msg, dest_header->pacsize * pacsum);
    for (int i = 0; i < pacsum; i++) {
        memcpy(dest_header->coef[i], src_header->coef[i], pacsum * sizeof(GFType));
    }
}

bool MatchCacheHeader(ClientCacheHeaderMsg* header_client, EbrResendMsg* header_request) {
    ClientCacheHeader* header1 = header_client -> header;
    EbrHeader header2 = header_request -> header;
    /*if (memcmp(header1, &header2, sizeof(ClientCacheHeader))) {
        */
    if (header1->strnum == header2.strnum &&
            header1->destport == header2.sourceport &&
            (header1->destaddr).host == (header2.sourceaddr).host) {
        return true;
    } else {
        return false;
    }
}

void NewServerCacheMsg(Address sourceaddr, uint16_t sourceport, uint8_t strnum,
                       CODEC* codec, uint8_t size, ServerCacheHeaderMsg* header) {
    header -> strnum = strnum;
    (header->sourceaddr).host = sourceaddr.host;
    header -> sourceport = sourceport;
    header -> codec = codec;
    header -> size = size;
    header -> recvnum = 0;
    memset(header->recv, false, 16 * sizeof(bool));
}

void NewServerCacheMsg(EbrHeaderMsg* header_msg, ServerCacheHeaderMsg* header) {
    CODEC codec((header_msg->header).pacsum, (header_msg->header).length);
    NewServerCacheMsg((header_msg->header).sourceaddr, (header_msg->header).sourceport,
                      (header_msg->header).strnum, &codec, (header_msg->header).pacsum, header);
}

bool MatchServerCacheMsg(ServerCacheHeaderMsg* header, EbrHeaderMsg* msg) {
    if (header->sourceport == (msg->header).sourceport
        && (header->sourceaddr).host == ((msg->header).sourceaddr).host
        && header->strnum == (msg->header).strnum) {
        return true;
    } else {
        return false;
    }
}