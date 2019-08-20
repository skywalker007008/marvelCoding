/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#ifndef MARVELCODING_HEADER_H
#define MARVELCODING_HEADER_H

#include "codec.h"
#include "../marvel_constant.h"

#define MSG_TYPE 0
#define RESEND_TYPE 1
#define RESEND_MSG_TYPE 2
#define ACK_TYPE 3

#define HEADER_SIZE (sizeof(EbrHeader))
#define MSG_SIZE ((MARVEL kMaxPacketLength) * sizeof(char))
#define COEF_SIZE ((RLNC kMaxPartNum) * sizeof(GFType))
#define HEADER_MSG_SIZE (HEADER_SIZE + MSG_SIZE + COEF_SIZE)

extern struct sockaddr_in broadcast_addr;
extern struct sockaddr_in any_addr;

constexpr uint16_t kDefaultPort = 666;

typedef struct {
    uint32_t host;
}Address;

typedef struct {
    uint16_t strnum;
    Address sourceaddr;
    Address destaddr;
    short sourceport;
    short destport;
}HeaderSymbol;

typedef
struct struct_app_ebr_header_data
{
    char type; // type of message
    char range; // saved
    char codetype; // type of the coding
    char codenumber; // number of the coded times
    short pacsum; // total sum of the packets
    short strnum; // no. of the packet_stream
    short pacnum; // no. of the packet
    Address sourceaddr; // source address
    Address destaddr; // destination address
    short sourceport; // source port
    short destport; // destination port
    int length; // length of each payload
    char check[4]; // saved
}EbrHeader;

typedef struct {
    EbrHeader header;
    char payload[MARVEL kMaxPacketLength];
    GFType coef[RLNC kMaxPartNum];
}EbrHeaderMsg;

EbrHeaderMsg* NewEbrHeaderMsg(char type, char range, char code_type, char code_number,
                        short pac_sum, short str_num, short pac_num,
                        Address source_addr, Address dest_addr, short source_port, short dest_port,
                        int length, char* check, char* payload, GFType* coef);

void init_addr();

EbrHeaderMsg* CopyEbrHeaderMsg(EbrHeaderMsg* header);

#endif //MARVELCODING_HEADER_H
