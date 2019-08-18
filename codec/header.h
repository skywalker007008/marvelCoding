/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#ifndef MARVELCODING_HEADER_H
#define MARVELCODING_HEADER_H

#include "gf.h"
#include "../marvel_constant.h"

extern struct sockaddr_in broadcast_addr;

constexpr uint16_t kDefaultPort = 666;

typedef struct {
    uint32_t host;
}Address;

typedef struct {
    uint8_t codenumber;
    Address sourceaddr;
    Address destaddr;
    short sourceport;
    short destport;
}HeaderSymbol;

typedef
struct struct_app_ebr_header_data
{
    char type;
    char range;
    char codetype;
    char codenumber;
    short pacsum;
    short strnum;
    short pacnum;
    Address sourceaddr;
    Address destaddr;
    short sourceport;
    short destport;
    int length;
    char* check;
    char* payload;
    GFType* coef;
}EbrHeader;

typedef
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
#endif /* ADDON_BOEINGFCS */
    STAT_AppStatistics* stats;
}
        AppDataEbrServer;

/*typedef struct {
    EbrHeader ebr_header;
    char* message;
} EbrMsg;*/

EbrHeader* NewEbrHeader(char type, char range, char code_type, char code_number,
                        short pac_sum, short str_num, short pac_num,
                        Address source_addr, Address dest_addr, short source_port, short dest_port,
                        int length, char* check, char* payload, GFType* coef);

void init_broadcast_addr();

EbrHeader* CopyEbrHeader(EbrHeader* header);

#endif //MARVELCODING_HEADER_H