/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#ifndef MARVELCODING_HEADER_H
#define MARVELCODING_HEADER_H

#include "codec.h"
#include "../include/marvel_constant.h"
#include <endian.h>
#include "../include/queue.h"
#include "../include/alloc.h"

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
    uint8_t codenumber;
    Address sourceaddr;
    Address destaddr;
    uint16_t sourceport;
    uint16_t destport;
}HeaderSymbol;

typedef struct {
    uint32_t host;
}Address;

typedef
struct struct_app_ebr_header_data
{
#if defined(__BIG_ENDIAN)
    char type:4; // type of message
    char range:4; // No. of the hop
#elif defined(__LITTLE_ENDIAN)
    char range:4; // No. of the hop
    char type:4; // type of message
#endif

#if defined(__BIG_ENDIAN)
    char codetype:4; // type of the coding(actually only one)
    char codenumber:4; // number of the coded times
#elif defined(__LITTLE_ENDIAN)
    char codenumber:4; // number of the coded times
    char codetype:4; // type of the coding(actually only one)
#endif

#if defined(__BIG_ENDIAN)
    uint8_t pacsum:4; // total sum of the packets
    uint8_t pacnum:4; // no. of the packet
#elif defined(__LITTLE_ENDIAN)
    uint8_t pacnum:4; // no. of the packet
    uint8_t pacsum:4; // total sum of the packets
#endif
    uint8_t strnum; // no. of the packet_stream

    Address sourceaddr; // source address
    Address destaddr; // destination address

    uint16_t sourceport; // source port
    uint16_t destport; // destination port

    uint16_t length; // length of each payload
    uint16_t check; // handle error
}EbrHeader;

typedef struct EbrHeaderMsg {
    EbrHeader header;
    char payload[MARVEL kMaxPacketLength];
    GFType coef[RLNC kMaxPartNum];
}EbrHeaderMsg;

#ifdef MARVEL_TCP
typedef struct EbrResendMsg {
    EbrHeader header;
    uint16_t symbol;
};
#endif

// Cache Used

typedef struct {
    uint8_t strnum;
    Address destaddr;
    uint16_t destport;
}ClientCacheHeader;

typedef struct ClientCacheHeaderMsg {
#ifdef MARVELCODING_QUEUE_H
    TAILQ_ENTRY(ClientCacheHeaderMsg) cache_link;
#endif
    ClientCacheHeader* header;
    char* msg;
    GFType** coef;
    uint8_t pacsum;
    uint16_t pacsize;
}ClientCacheHeaderMsg;

typedef struct ClientCacheRequest {
    ClientCacheHeader header;
    uint8_t misscoef[RLNC kMaxPartNum];
}ClientCacheRequest;

typedef struct ServerCacheHeaderMsg {
#ifdef MARVELCODING_QUEUE_H
    TAILQ_ENTRY(ServerCacheHeaderMsg) cache_link;
#endif
    Address sourceaddr;
    uint16_t sourceport;
    uint8_t strnum;
    CODEC* codec;
    uint8_t size;
    uint8_t recvnum;
    bool recv[16];
};

#ifdef MARVELCODING_QUEUE_H

TAILQ_HEAD(ClientCacheList, ClientCacheHeaderMsg);
TAILQ_HEAD(ServerCacheList, ServerCacheHeaderMsg);

#endif

void init_addr();

EbrHeaderMsg* NewEbrHeaderMsg(char type, char range, char code_type, char code_number,
                        short pac_sum, short str_num, short pac_num,
                        Address source_addr, Address dest_addr, short source_port, short dest_port,
                        int length, char* check, char* payload, GFType* coef);

EbrHeaderMsg* CopyEbrHeaderMsg(EbrHeaderMsg* header);

#ifdef MARVEL_TCP
EbrResendMsg* NewEbrResendMsg(ServerCacheHeaderMsg* cache_msg);

uint16_t ResendValue(bool recv[]);
#endif

void NewClientCacheHeader(ClientCacheHeader* header, uint8_t strnum, Address destaddr, uint16_t destport);

void NewClientCacheHeaderMsg(ClientCacheHeaderMsg* header_msg, uint8_t strnum, uint8_t pacsum, uint16_t pacsize,
                          char* msg, GFType** coef, Address destaddr, uint16_t destport);

void CopyCacheHeaderMsg(ClientCacheHeaderMsg* src_header, ClientCacheHeaderMsg* dest_header);

void NewClientCacheRequest(ClientCacheRequest* request, uint8_t strnum,
                           Address destaddr, uint16_t destport,
                           uint8_t* miss_packet, uint8_t pacsum);

bool MatchCacheHeader(ClientCacheHeaderMsg* header_client, ClientCacheRequest* header_request);

void NewServerCacheMsg(Address sourceaddr, uint16_t sourceport, uint8_t strnum,
                       CODEC* codec, uint8_t size, ServerCacheHeaderMsg* header);

void NewServerCacheMsg(EbrHeaderMsg* header_msg, ServerCacheHeaderMsg* header);

bool MatchServerCacheMsg(ServerCacheHeaderMsg* header, EbrHeaderMsg* msg);

#endif //MARVELCODING_HEADER_H
