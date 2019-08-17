// Copyright (c) 2001-2013, SCALABLE Network Technologies, Inc.  All Rights Reserved.
//                          600 Corporate Pointe
//                          Suite 1200
//                          Culver City, CA 90230
//                          info@scalable-networks.com
//
// This source code is licensed, not sold, and is subject to a written
// license agreement.  Among other things, no portion of this source
// code may be copied, transmitted, disclosed, displayed, distributed,
// translated, used as the basis for a derivative work, or used, in
// whole or in part, for any program or purpose other than its intended
// use in compliance with the license agreement as part of the QualNet
// software.  This source code and certain of the algorithms contained
// within it are confidential trade secrets of Scalable Network
// Technologies, Inc. and may not be used as the basis for any other
// software, hardware, product or service.

#ifndef _EBR_APP_H_
#define _EBR_APP_H_

#include "types.h"
#include "dynamic.h"
#include "stats_app.h"
#include "application.h"
#include <stdint.h>
#include "gf.h"
#include "codec.h"

#define splitnum 2
#define maxcodenum 2
#define maxrange 2

#ifdef ADDON_DB
class SequenceNumber;
#endif // ADDON_DB
/*
 * Header size defined to be consistent accross 32/64 bit platforms
 */
#define EBR_HEADER_SIZE sizeof(EbrHeader)
/*
 * Data item structure used by ebr.
 */
typedef
struct struct_app_ebr_data
{
    short sourcePort;
    char type;
    BOOL isMdpEnabled;
    Int32 seqNo;
    clocktype txTime;
#ifdef ADDON_BOEINGFCS
    Int32 txPartitionId;
#endif // ADDON_BOEINGFCS

#if defined(ADVANCED_WIRELESS_LIB) || defined(UMTS_LIB) || defined(MUOS_LIB)
    Int32 pktSize;
    clocktype interval;
#endif // ADVANCED_WIRELESS_LIB || UMTS_LIB || MUOS_LIB
}
EbrData;

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

/* Structure containing ebr client information. */

typedef
struct struct_app_ebr_client_str
{
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
    int  uniqueId;
    Int32 mdpUniqueId;
    BOOL isMdpEnabled;
    std::string* applicationName;
#ifdef ADDON_DB
    Int32 sessionId;
    Int32 receiverId;
#endif // ADDON_DB

#ifdef ADDON_NGCNMS
    Message* lastTimer;
#endif
    STAT_AppStatistics* stats;

    // Dynamic Address
    NodeId destNodeId; // destination node id for this app session 
    Int16 clientInterfaceIndex; // client interface index for this app 
                                // session
    Int16 destInterfaceIndex; // destination interface index for this app
                              // session
    // dns
    std::string* serverUrl;
}
AppDataEbrClient;

/* Structure containing ebr related information. */

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


/*
 * NAME:        AppLayerEbrClient.
 * PURPOSE:     Models the behaviour of EbrClient Client on receiving the
 *              message encapsulated in msg.
 * PARAMETERS:  nodePtr - pointer to the node which received the message.
 *              msg - message received by the layer
 * RETURN:      none.
 */
void
AppLayerEbrClient(Node *nodePtr, Message *msg);

/*
 * NAME:        AppEbrClientInit.
 * PURPOSE:     Initialize a EbrClient session.
 * PARAMETERS:  nodePtr - pointer to the node,
 *              serverAddr - address of the server,
 *              itemsToSend - number of items to send,
 *              itemSize - size of each packet,
 *              interval - interval of packet transmission rate.
 *              startTime - time until the session starts.
 *              endTime - time until the session end.
 *              tos - the contents for the type of service field.
 *              srcString - source name
 *              destString - destination name
 *              isRsvpTeEnabled - whether RSVP-TE is enabled
 *              appName - application alias name
 *              isMdpEnabled - true if MDP is enabled by user.
 *              isProfileNameSet - true if profile name is provided by user.
 *              profileName - mdp profile name.
 *              uniqueId - mdp session unique id.
 *              nodeInput - nodeInput for config file.
 * RETURN:      none.
 */
void
AppEbrClientInit(
    Node *node,
    Address clientAddr,
    Address serverAddr,
    Int32 itemsToSend,
    Int32 itemSize,
    clocktype interval,
    unsigned tos,
    const char* srcString,
    const char* destString,
    BOOL isRsvpTeEnabled,
    char* appName,
    BOOL isMdpEnabled = FALSE,
    BOOL isProfileNameSet = FALSE,
    char* profileName = NULL,
    Int32 uniqueId = -1,
    const NodeInput* nodeInput = NULL,
    clocktype startTime=0,
    clocktype endTime=120);


/*
 * NAME:        AppEbrClientPrintStats.
 * PURPOSE:     Prints statistics of a EbrClient session.
 * PARAMETERS:  nodePtr - pointer to the this node.
 *              clientPtr - pointer to the ebr client data structure.
 * RETURN:      none.
 */
void
AppEbrClientPrintStats(Node *nodePtr, AppDataEbrClient *clientPtr);

/*
 * NAME:        AppEbrClientFinalize.
 * PURPOSE:     Collect statistics of a EbrClient session.
 * PARAMETERS:  nodePtr - pointer to the node.
 *              appInfo - pointer to the application info data structure.
 * RETURN:      none.
 */
void
AppEbrClientFinalize(Node *nodePtr, AppInfo* appInfo);

/*
 * NAME:        AppEbrClientGetEbrClient.
 * PURPOSE:     search for a ebr client data structure.
 * PARAMETERS:  nodePtr - pointer to the node.
 *              sourcePort - source port of the ebr client.
 * RETURN:      the pointer to the ebr client data structure,
 *              NULL if nothing found.
 */
AppDataEbrClient *
AppEbrClientGetEbrClient(Node *nodePtr, short sourcePort);

/*
 * NAME:        AppEbrClientNewEbrClient.
 * PURPOSE:     create a new ebr client data structure, place it
 *              at the beginning of the application list.
 * PARAMETERS:  nodePtr - pointer to the node.
 *              remoteAddr - remote address.
 *              itemsToSend - number of items to send,
 *              itemSize - size of data packets.
 *              interval - interdeparture time of packets.
 *              startTime - time when session is started.
 *              endTime - time when session is ended.
 * RETURN:      the pointer to the created ebr client data structure,
 *              NULL if no data structure allocated.
 */
AppDataEbrClient *
AppEbrClientNewEbrClient(
    Node *nodePtr,
    Address localAddr,
    Address remoteAddr,
    Int32 itemsToSend,
    Int32 itemSize,
    clocktype interval,
    clocktype startTime,
    clocktype endTime,
    TosType tos,
    char* appName);

/*
 * NAME:        AppEbrClientScheduleNextPkt.
 * PURPOSE:     schedule the next packet the client will send.  If next packet
 *              won't arrive until the finish deadline, schedule a close.
 * PARAMETERS:  nodePtr - pointer to the node,
 *              clientPtr - pointer to the ebr client data structure.
 * RETRUN:      none.
 */
void
AppEbrClientScheduleNextPkt(Node *nodePtr, AppDataEbrClient *clientPtr);


/*
 * NAME:        AppLayerEbrServer.
 * PURPOSE:     Models the behaviour of EbrServer Server on receiving the
 *              message encapsulated in msg.
 * PARAMETERS:  nodePtr - pointer to the node which received the message.
 *              msg - message received by the layer
 * RETURN:      none.
 */
void
AppLayerEbrServer(Node *nodePtr, Message *msg);

/*
 * NAME:        AppEbrServerInit.
 * PURPOSE:     listen on EbrServer server port.
 * PARAMETERS:  nodePtr - pointer to the node.
 * RETURN:      none.
 */
void
AppEbrServerInit(Node *nodePtr);

/*
 * NAME:        AppEbrServerPrintStats.
 * PURPOSE:     Prints statistics of a EbrServer session.
 * PARAMETERS:  nodePtr - pointer to this node.
 *              serverPtr - pointer to the ebr server data structure.
 * RETURN:      none.
 */
void
AppEbrServerPrintStats(Node *nodePtr, AppDataEbrServer *serverPtr);

/*
 * NAME:        AppEbrServerFinalize.
 * PURPOSE:     Collect statistics of a EbrServer session.
 * PARAMETERS:  nodePtr - pointer to the node.
 *              appInfo - pointer to the application info data structure.
 * RETURN:      none.
 */
void
AppEbrServerFinalize(Node *nodePtr, AppInfo* appInfo);

/*
 * NAME:        AppEbrServerGetEbrServer.
 * PURPOSE:     search for a ebr server data structure.
 * PARAMETERS:  nodePtr - ebr server.
 *              sourcePort - source port of ebr client/server pair.
 *              remoteAddr - ebr client sending the data.
 * RETURN:      the pointer to the ebr server data structure,
 *              NULL if nothing found.
 */
AppDataEbrServer *
AppEbrServerGetEbrServer(
    Node *nodePtr,
    Address remoteAddr,
    short sourcePort);

/*
 * NAME:        AppEbrServerNewEbrServer.
 * PURPOSE:     create a new ebr server data structure, place it
 *              at the beginning of the application list.
 * PARAMETERS:  nodePtr - pointer to the node.
 *              localAddr - local address.
 *              remoteAddr - remote address.
 *              sourcePort - source port of ebr client/server pair.
 * RETRUN:      the pointer to the created ebr server data structure,
 *              NULL if no data structure allocated.
 */
AppDataEbrServer *
AppEbrServerNewEbrServer(
    Node *nodePtr,
    Address localAddr,
    Address remoteAddr,
    short sourcePort);

 /*
 * NAME       :: AppLayerEbrPrintTraceXML
 * PURPOSE    :: Print packet trace information in XML format
 * PARAMETERS :: node     : Node*    : Pointer to node
 *                msg      : Message* : Pointer to packet to print headers
 * RETURN     ::  void   : NULL
*/

void AppLayerEbrPrintTraceXML(Node* node, Message* msg);

/*
 * FUNCTION   :: AppLayerEbrInitTrace
 * PURPOSE    :: Ebr initialization  for tracing
 * PARAMETERS :: node : Node* : Pointer to node
 *               nodeInput    : const NodeInput* : Pointer to NodeInput
 * RETURN ::  void : NULL
*/
void AppLayerEbrInitTrace(Node* node, const NodeInput* nodeInput);

#ifdef ADDON_NGCNMS
void
AppEbrClientReInit(Node* node, Address sourceAddr);

void AppEbrReset(Node* node);

#endif

// Dynamic address
//---------------------------------------------------------------------------
// FUNCTION             :AppEbrClientAddAddressInformation.
// PURPOSE              :store client interface index, destination 
//                       interface index destination node id to get the 
//                       correct address when appplication starts
// PARAMETERS:
// + node               : Node* : pointer to the node
// + clientPtr          : AppDataEbrClient* : pointer to EBR client data
// RETRUN               : void
//---------------------------------------------------------------------------
void
AppEbrClientAddAddressInformation(Node* node,
                                  AppDataEbrClient* clientPtr);

//---------------------------------------------------------------------------
// FUNCTION             :AppEbrClientGetSessionAddressState.
// PURPOSE              :get the current address sate of client and server 
// PARAMETERS:
// + node               : Node* : pointer to the node
// + clientPtr          : AppDataEbrClient* : pointer to EBR client data
// RETRUN:     
// addressState         : AppAddressState :
//                        ADDRESS_FOUND : if both client and server
//                                        are having valid address
//                        ADDRESS_INVALID : if either of them are in 
//                                          invalid address state
//---------------------------------------------------------------------------
AppAddressState
AppEbrClientGetSessionAddressState(Node* node,
                                   AppDataEbrClient* clientPtr);

//---------------------------------------------------------------------------
// NAME      : AppEbrUrlSessionStartCallback
// PURPOSE   : Process Received DNS info.
// PARAMETERS::
// + node       : Node*          : pointer to the node
// + serverAddr : Address*       : server address
// + sourcePort : unsigned short : source port
// + uniqueId   : Int32          : connection id; not used for EBR
// + interfaceId: Int16          : interface index,
// + serverUrl  : std::string    : server URL
// + packetSendingInfo : AppEtpPacketSendingInfo : information required to 
//                                                 send buffered application 
//                                                 packets in case of ETP 
//                                                 based applications
// RETURN       :
// bool         : TRUE if application packet can be sent; FALSE otherwise
//                TCP based application will always return FALSE as 
//                this callback will initiate TCP Open request and not send 
//                packet

//---------------------------------------------------------------------------
bool
AppEbrUrlSessionStartCallback(
                    Node* node,
                    Address* serverAddr,
                    UInt16 sourcePort,
                    Int32 uniqueId,
                    Int16 interfaceId,
                    std::string serverUrl,
                    struct AppEtpPacketSendingInfo* packetSendingInfo);
#endif /* _EBR_APP_H_ */

