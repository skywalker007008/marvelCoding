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

#define HEADER_SIZE (sizeof(EbrHeader))
#define MSG_SIZE ((MARVEL kMaxPacketLength) * sizeof(char))
#define COEF_SIZE ((RLNC kMaxPartNum) * sizeof(GFType))
#define HEADER_MSG_SIZE (HEADER_SIZE + MSG_SIZE + COEF_SIZE)

extern struct sockaddr_in broadcast_addr;

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

    // char* payload; // payload
    // GFType* coef; // coef of the payload
}EbrHeader;

typedef struct {
    EbrHeader header;
    char payload[MARVEL kMaxPacketLength];
    GFType coef[RLNC kMaxPartNum];
}EbrHeaderMsg;


/*typedef
struct struct_app_ebr_client_str {
    Address localAddr;
    Address remoteAddr;
    D_Clocktype interval;
    clocktype sessionStart;
    clocktype sessionFinish;
    clocktype sessionLastSent;
    clocktype endTime;
    BOOL sessionIsClosed;
    UInt32 itemsToSend;
    UInt32 itemssum;
    UInt32 itemSize;
    UInt32 NO;
    short sourcePort;
    Int32 seqNo;
    D_UInt32 tos;
    int uniqueId;
    Int32 mdpUniqueId;
    BOOL isMdpEnabled;
    STRING *applicationName;
    STAT_AppStatistics *stats;

    // Dynamic Address
    NodeId destNodeId; // destination node id for this app session
    Int16 clientInterfaceIndex; // client interface index for this app
    // session
    Int16 destInterfaceIndex; // destination interface index for this app
    // session
    // dns
    std::string *serverUrl;
}AppDataEbrClient;

typedef
struct struct_app_ebr_server_str
{
    Address localAddr;
    Address remoteAddr;
    short sourcePort;
    clocktype sessionStart;
    clocktype sessionFinish;
    clocktype sessionLastReceived;
    BOOL sessionIsClosed;
    Int32 seqNo;
    int uniqueId ;

    //    clocktype lastInterArrivalInterval;
    //    clocktype lastPacketReceptionTime;

#ifdef ADDON_DB
    Int32 sessionId;
    Int32 sessionInitiator;
    Int32 hopCount;
    SequenceNumber *seqNumCache;
#endif // ADDON_DB


#ifdef ADDON_BOEINGFCS
    // The number of packets received from a different partition
    UInt32 numPktsRecvdDP;

    // Total end to end delay between nodes on different partitions
    clocktype totalEndToEndDelayDP;

    // Total jitter between nodes on different partitions
    clocktype totalJitterDP;

    // Used for calculating jitter for packets on different partitions
    clocktype lastPacketReceptionTimeDP;
    clocktype lastInterArrivalIntervalDP;

    // Whether or not to perform sequence number check on incoming data packets
    BOOL useSeqNoCheck;
#endif *//* ADDON_BOEINGFCS *//*
    STAT_AppStatistics* stats;
}
        AppDataEbrServer;

*//*typedef struct {
    EbrHeader ebr_header;
    char* message;
} EbrMsg;*/

EbrHeaderMsg* NewEbrHeaderMsg(char type, char range, char code_type, char code_number,
                        short pac_sum, short str_num, short pac_num,
                        Address source_addr, Address dest_addr, short source_port, short dest_port,
                        int length, char* check, char* payload, GFType* coef);

void init_broadcast_addr();

EbrHeaderMsg* CopyEbrHeaderMsg(EbrHeaderMsg* header);

#endif //MARVELCODING_HEADER_H
