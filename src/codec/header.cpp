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

void NewClientCacheRequest(ClientCacheRequest* request, uint8_t strnum,
                           Address destaddr, uint16_t destport,
                           uint8_t* miss_packet, uint8_t pacsum) {
    ClientCacheHeader* header = (ClientCacheHeader*)malloc(sizeof(ClientCacheHeader));
    NewClientCacheHeader(header, strnum, destaddr, destport);
    memcpy(&(request->header), header, sizeof(ClientCacheHeader));
    memset(request->misscoef, 0, RLNC kMaxPartNum * sizeof(uint8_t));
    memcpy(request->misscoef, miss_packet, pacsum * sizeof(uint8_t));
}

bool MatchCacheHeader(ClientCacheHeaderMsg* header_client, ClientCacheRequest* header_request) {
    ClientCacheHeader* header1 = header_client -> header;
    ClientCacheHeader header2 = header_request -> header;
    /*if (memcmp(header1, &header2, sizeof(ClientCacheHeader))) {
        */
    if (header1->strnum == header2.strnum &&
            header1->destport == header2.destport &&
            (header1->destaddr).host == (header2.destaddr).host) {
        return true;
    } else {
        return false;
    }
}