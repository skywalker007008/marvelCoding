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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "api.h"
#include "partition.h"
#include "app_util.h"
#include "app_ebr.h"
#include "ipv6.h"
#include "transport_udp.h" 


#ifdef ADDON_DB
#include "db.h"
#include "dbapi.h"
#endif // ADDON_DB

#ifdef LTE_LIB
#ifdef LTE_LIB_LOG
#include "log_lte.h"
#endif
#endif


// Pseudo traffic sender layer
#include "app_trafficSender.h"
#include "RS-FEC.h"
#define FEC

struct Record{
    int codenumber;
    int pacnum;
    BOOL isfirst;
};
static struct Record record[100][100];
static int strnum=0;
static int streamcodenum[10]={0};
EbrHeader store[10][maxcodenum+1][splitnum+1];

void print(char* msg) {
    int len = strlen(msg);
    for (int i = 0; i < len; i++) {
        printf("%c",(uint8_t)msg[i]);      
    }
	std::cout << "\n";
}

void printx(char* msg) {
    int len = strlen(msg);
    for (int i = 0; i < len / 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%x ", (uint8_t)msg[i * 8 + j]);
        }
        std::cout << "\n";
    }
}

void print_mul() {
    for (int i = 0; i < gFieldSize; i++) {
        for (int j = 0; j < gFieldSize; j++) {
            printf("%x ", table_mul[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    for (int i = 0; i < gFieldSize; i++) {
        for (int j = 0; j < gFieldSize; j++) {
            printf("%x ", table_div[i][j]);
        }
        printf("\n");
    }

    printf("%x", gf_mul(3,0xE));

}

void print_mat(GFType** mat, int vec_size) {
    printf("matrix::\n");
    for (int i = 0; i < vec_size; i++) {
        for (int j = 0; j < vec_size; j++) {
            printf("%x ", mat[i][j]);
        }
        printf("\n");
    }
}
// /**
// FUNCTION   :: AppLayerEbrInitTrace
// LAYER      :: APPLCATION
// PURPOSE    :: Ebr initialization  for tracing
// PARAMETERS ::
// + node : Node* : Pointer to node
// + nodeInput    : const NodeInput* : Pointer to NodeInput
// RETURN ::  void : NULL
// **/


void AppLayerEbrInitTrace(Node* node, const NodeInput* nodeInput)
{
    char buf[MAX_STRING_LENGTH];
    BOOL retVal;
    BOOL traceAll = TRACE_IsTraceAll(node);
    BOOL trace = FALSE;
    static BOOL writeMap = TRUE;

    IO_ReadString(
        node->nodeId,
        ANY_ADDRESS,
        nodeInput,
        "TRACE-EBR",
        &retVal,
        buf);

    if (retVal)
    {
        if (strcmp(buf, "YES") == 0)
        {
            trace = TRUE;
        }
        else if (strcmp(buf, "NO") == 0)
        {
            trace = FALSE;
        }
        else
        {
            ERROR_ReportError(
                "TRACE-EBR should be either \"YES\" or \"NO\".\n");
        }
    }
    else
    {
        if (traceAll || node->traceData->layer[TRACE_APPLICATION_LAYER])
        {
            trace = TRUE;
        }
    }

    if (trace)
    {
            TRACE_EnableTraceXML(node, TRACE_EBR,
                "EBR",  AppLayerEbrPrintTraceXML, writeMap);
    }
    else
    {
            TRACE_DisableTraceXML(node, TRACE_EBR,
                "EBR", writeMap);
    }
    writeMap = FALSE;
}


//**
// FUNCTION   :: AppLayerEbrPrintTraceXML
// LAYER      :: NETWORK
// PURPOSE    :: Print packet trace information in XML format
// PARAMETERS ::
// + node     : Node*    : Pointer to node
// + msg      : Message* : Pointer to packet to print headers from
// RETURN     ::  void   : NULL
// **/

void AppLayerEbrPrintTraceXML(Node* node, Message* msg)
{
    char buf[MAX_STRING_LENGTH];
    char clockStr[MAX_STRING_LENGTH];

    EbrData* data = (EbrData*) MESSAGE_ReturnPacket(msg);

    if (data == NULL) {
        return;
       }

    TIME_PrintClockInSecond(data->txTime, clockStr);

    sprintf(buf, "<ebr>%u %c %u %d %s</ebr>",
            data->sourcePort,
            data->type,
            data->seqNo,
                 (msg->packetSize + msg->virtualPayloadSize),
            clockStr);
    TRACE_WriteToBufferXML(node, buf);
}

void 
printaddrString(
    NodeAddress ipAddress)
{
    printf( "%u.%u.%u.%u\n",
            (ipAddress & 0xff000000) >> 24,
            (ipAddress & 0xff0000) >> 16,
            (ipAddress & 0xff00) >> 8,
            ipAddress & 0xff);
}


/*
 * NOTE: EBR does not attempt to reorder packets.  Any out of order
 *       packets will be dropped.
 */

/*
 * NAME:        AppLayerEbrClient.
 * PURPOSE:     Models the behaviour of EbrClient Client on receiving the
 *              message encapsulated in msg.
 * PARAMETERS:  node - pointer to the node which received the message.
 *              msg - message received by the layer
 * RETURN:      none.
 */
void
AppLayerEbrClient(Node *node, Message *msg)
{
    char buf[MAX_STRING_LENGTH];
    char error[MAX_STRING_LENGTH];
    AppDataEbrClient *clientPtr;

#ifdef DEBUG
    TIME_PrintClockInSecond(node->getNodeTime(), buf);
    printf("EBR Client: Node %ld got a message at %sS\n",
           node->nodeId, buf);
#endif /* DEBUG */

    switch (msg->eventType)
    {
        case MSG_APP_TimerExpired:
        {
            AppTimer *timer;

            timer = (AppTimer *) MESSAGE_ReturnInfo(msg);

#ifdef DEBUG
            printf("EBR Client: Node %ld at %s got timer %d\n",
                   node->nodeId, buf, timer->type);
#endif /* DEBUG */

            clientPtr = AppEbrClientGetEbrClient(node, timer->sourcePort);

            if (clientPtr == NULL)
            {
                sprintf(error, "EBR Client: Node %d cannot find ebr"
                    " client\n", node->nodeId);

                ERROR_ReportError(error);
            }

            switch (timer->type)
            {
                case APP_TIMER_SEND_PKT:
                {
                    EbrData data;
                    char ebrData[EBR_HEADER_SIZE];

                    memset(&data, 0, sizeof(data));

                    ERROR_Assert(sizeof(data) <= EBR_HEADER_SIZE,
                         "EbrData size cant be greater than EBR_HEADER_SIZE");

#ifdef DEBUG
                    printf("EBR Client: Node %u has %u items left to"
                        " send\n", node->nodeId, clientPtr->itemsToSend);
#endif /* DEBUG */

                    if (clientPtr->itemsToSend > 1)
                    {
                        data.type = 'd';
                    }
                    else
                    {
                        data.type = 'c';
                        clientPtr->sessionIsClosed = TRUE;
                        clientPtr->sessionFinish = node->getNodeTime();
                        if (node->appData.appStats && clientPtr->stats)
                        { 
                            clientPtr->stats->SessionFinish(node);
                        }
                    }
                    data.sourcePort = clientPtr->sourcePort;
                    data.txTime = node->getNodeTime();
                    data.seqNo = clientPtr->seqNo++;
                    data.isMdpEnabled = clientPtr->isMdpEnabled;
#if defined(ADVANCED_WIRELESS_LIB) || defined(UMTS_LIB) || defined(MUOS_LIB)
                    data.pktSize = clientPtr->itemSize;
                    data.interval = clientPtr->interval;
#endif // ADVANCED_WIRELESS_LIB || UMTS_LIB || MUOS_LIB

#ifdef DEBUG
                    {
                        char clockStr[MAX_STRING_LENGTH];
                        char addrStr[MAX_STRING_LENGTH];

                        TIME_PrintClockInSecond(node->getNodeTime(), clockStr, node);
                        IO_ConvertIpAddressToString(
                            &clientPtr->remoteAddr, addrStr);

                        printf("EBR Client: node %ld sending data packet"
                               " at time %sS to EBR server %s\n",
                               node->nodeId, clockStr, addrStr);
                        printf("    size of payload is %d\n",
                               clientPtr->itemSize);
                    }
#endif /* DEBUG */
                    // Note: An overloaded Function
                    memset(ebrData, 0, EBR_HEADER_SIZE);
                    memcpy(ebrData, (char *) &data, sizeof(data));
#ifdef ADDON_DB
                    StatsDBAppEventParam appParam;
                    appParam.m_SessionInitiator = node->nodeId;
                    appParam.m_ReceiverId = clientPtr->receiverId;
                    appParam.SetAppType("EBR");
                    appParam.SetFragNum(0);

                    if (!clientPtr->applicationName->empty())
                    {
                        appParam.SetAppName(
                            clientPtr->applicationName->c_str());
                    }
                    // dns
                    if (clientPtr->remoteAddr.networkType != NETWORK_INVALID)
                    {
                        appParam.SetReceiverAddr(&clientPtr->remoteAddr);
                    }
                    appParam.SetPriority(clientPtr->tos);
                    appParam.SetSessionId(clientPtr->sessionId);
                    appParam.SetMsgSize(clientPtr->itemSize);
                    appParam.m_TotalMsgSize = clientPtr->itemSize;
                    appParam.m_fragEnabled = FALSE;
#endif // ADDON_DB

                    // Dynamic address
                    // Create and send a UDP msg with header and virtual
                    // payload.
                    // if the client has not yet acquired a valid
                    // address then the application packets should not be
                    // generated
                    // check whether client and server are in valid address
                    // state
                    // if this is session start then packets will not be sent
                    // if in invalid address state and session will be closed
                    // ; if the session has already started and address
                    // becomes invalid during application session then
                    // packets will get generated but will be dropped at
                    //  network layer
                    if (AppEbrClientGetSessionAddressState(node, clientPtr)
                            == ADDRESS_FOUND)
                    {
					//set ebr headder
						EbrHeader ebrheader;
						memset(&ebrheader, 0, sizeof(ebrheader));
						ebrheader.type = 'd';
						ebrheader.range = 0;
						ebrheader.codetype = 0;
						ebrheader.codenumber = 1;
						ebrheader.pacsum = splitnum;
						ebrheader.strnum = strnum-clientPtr->itemsToSend+1;
						ebrheader.pacnum = 0;

						ebrheader.length = clientPtr->itemSize;
						ebrheader.sourceaddr=clientPtr->localAddr;
						ebrheader.destaddr=clientPtr->remoteAddr;
						ebrheader.sourceport=clientPtr->sourcePort;
						ebrheader.destport=clientPtr->sourcePort;
						ebrheader.check=0;
						char *rpayload = (char*)malloc(ebrheader.length*sizeof(char));
						char *payload = (char*)malloc(ebrheader.length*sizeof(char));
						for(int i=0;i<ebrheader.length-1;i++){
							rpayload[i] = 'a';
						}
						rpayload[ebrheader.length-1]=0;
						printf("the payload origin is: ");
						printx(rpayload);
						printf("\n");
						int len = ebrheader.length/splitnum;
						RLNC init(8);
						CODEC codec(splitnum, len);
						for(int tb=0;tb<splitnum;tb++){
						codec.RecvMessage(rpayload + len*tb, std_coef[tb]);
						}
						GFType** rand1;
						rand1 = codec.encode();
						// rand1 = gf_newcoef(rand1, std_coef, 2, 2);
						// RLNC print_mat(rand1, 2);
						char* payload1 = (char*)malloc(ebrheader.length * sizeof(char));
						codec.get_encode_message(payload1);
						std::cout << "Encode Finished\n";
						printx(payload1);
						printf("\n");
						print_mat(rand1, 2);
						printf("\n");
						/*char *decode_buf = (char*)malloc(ebrheader.length*sizeof(char));
						CODEC codec3(2, len);
						codec3.RecvMessage(payload1, rand1[0]);
						codec3.RecvMessage(payload1+len, rand1[1]);
						if (codec3.LinkMsg()) {
							std::cout << "Link Finished\n";
							codec3.decode();
							codec3.get_decode_message(decode_buf);
							std::cout << "Decode Finished\n";
							printx(decode_buf);
						}printf("\n");*/
for(int o = 0;o<splitnum;o++){
						printf("%d\n",len);
						char *pay = (char*)malloc(len*sizeof(char));
						memset(pay,0,len*sizeof(char));
						memcpy(pay,payload1+o*len,len*sizeof(char));
						printx(pay);
						printf("\n");
						if(o==splitnum-1){
							ebrheader.type='c';
						}
						ebrheader.payload = pay;
						ebrheader.coef = rand1[o];
						ebrheader.pacnum += 1;
						ebrheader.length = len;
					printf("node %d is sending the message",node->nodeId);
					printf("ebrheader.type =%c\n",ebrheader.type);
					printf("ebrheader.range =%d\n",ebrheader.range);
					printf("ebrheader.codetype =%d\n",ebrheader.codetype);
					printf("ebrheader.pacsum =%d\n",ebrheader.pacsum);
					printf("ebrheader.strnum =%d\n",ebrheader.strnum);
					printf("ebrheader.pacnum =%d\n",ebrheader.pacnum);
					printf("ebrheader.length =%d\n",ebrheader.length);
					printf("ebrheader.sourceaddr =%d\n",ebrheader.sourceaddr.interfaceAddr.ipv4);
					printf("ebrheader.destaddr =%d\n",ebrheader.destaddr.interfaceAddr.ipv4);
					printf("ebrheader.sourceport =%d\n",ebrheader.sourceport);
					printf("ebrheader.destport =%d\n",ebrheader.destport);
					for (int i = 0; i < node->numberInterfaces; i++)
					{
					 NodeAddress destAddress;
					 Address temp = clientPtr->remoteAddr;
					 //temp.interfaceAddr.ipv4 = ANY_DEST;
					 if (NetworkIpIsWiredNetwork (node, i))			 
						 temp.interfaceAddr.ipv4 = NetworkIpGetInterfaceBroadcastAddress(node, i);			 
					 else temp.interfaceAddr.ipv4 = ANY_DEST;
					 printf("send to port%d ,and addr %d \n",i);
					 printaddrString(temp.interfaceAddr.ipv4);
                        Message* sentMsg = APP_UdpCreateMessage(
                                                node,
                                                clientPtr->localAddr,
                                                (short) clientPtr->sourcePort,
                                                temp,
                                                (short) APP_EBR_SERVER,
                                                TRACE_EBR,
                                                clientPtr->tos);

						
						ebrheader.payload = pay;
#ifdef FEC				

						const int msglen = 128;  
						const uint8_t ECC_LENGTH = 10;		
						char message_frame[msglen];
						char repaired[msglen];    
						char encoded[msglen + ECC_LENGTH];
						RS::ReedSolomon<msglen, ECC_LENGTH> rs;
						ebrheader.check = (char*)malloc(ECC_LENGTH);
						memset(message_frame, 0, sizeof(message_frame));        // Clear the array
						for (int i = 0; i < msglen; i++) { message_frame[i] = pay[i]; } // Fill with the message

						rs.Encode(message_frame, encoded);
						int j = 0;
						for (int i = msglen; i < msglen + ECC_LENGTH; i++) {ebrheader.check[j++] = encoded[i]; 
						//printf("%c",ebrheader.check[j - 1]); 
						}

						/*printf("Original: "); for (int i = 0; i < sizeof(message_frame); i++) { printf("%c", message_frame[i]); }    printf("\n");
						printf("Encoded:  ");        for (int i = 0; i < sizeof(encoded); i++) { printf("%c", encoded[i]); }    printf("\n");
						for (int i = 20; i < 25; i++) { encoded[i] = '-'; } //Let's steal some byte from 20 to 25.
						printf("Corrupted: ");      for (int i = 0; i < sizeof(encoded); i++) { printf("%c", encoded[i]); }    printf("\n");
						int j = 0;
						for (int i = msglen; i < msglen + ECC_LENGTH; i++) {ebrheader.check[j++] = encoded[i]; printf("%c",ebrheader.check[j - 1]); }
						printf("\n");
						for (int i = 0; i < msglen; i++) { encoded[i] = pay[i]; }
						j = 0;
						for (int i = msglen; i < msglen + ECC_LENGTH; i++) { encoded[i] =ebrheader.check[j++];}
						printf("Encoded:  ");        for (int i = 0; i < sizeof(encoded); i++) { printf("%c", encoded[i]); }    printf("\n");
						rs.Decode(encoded, repaired);
						printf("Repaired: "); for (int i = 0; i < sizeof(repaired); i++) { printf("%c", repaired[i]); }    printf("\n");*/

#endif

						memset(ebrData, 0, sizeof(ebrheader));
						memcpy(ebrData, (char *) &ebrheader, sizeof(ebrheader));
						APP_AddHeader(
                                node,
                                sentMsg,
                                ebrData,
                                sizeof(ebrheader));

                        int payloadSize =
                                clientPtr->itemSize - EBR_HEADER_SIZE;
                        APP_AddVirtualPayload(
                                node,
                                sentMsg,
                                payloadSize);
						
                        // Add the virtual payload size as info
                        APP_AddInfo(
                                node,
                                sentMsg,
                                (char*) &payloadSize,
                                sizeof(int),
                                INFO_TYPE_DataSize);
						/*
                        APP_AddHeader(
                                node,
                                sentMsg,
                                ebrData,
                                EBR_HEADER_SIZE);

                        int payloadSize =
                                clientPtr->itemSize - EBR_HEADER_SIZE;
                        APP_AddVirtualPayload(
                                node,
                                sentMsg,
                                payloadSize);

                        // Add the virtual payload size as info
                        APP_AddInfo(
                                node,
                                sentMsg,
                                (char*) &payloadSize,
                                sizeof(int),
                                INFO_TYPE_DataSize);
								*/


                        if (clientPtr->isMdpEnabled)
                        {
                            APP_UdpSetMdpEnabled(sentMsg,
                                                     clientPtr->mdpUniqueId);
                        }

                        // dns
                        AppUdpPacketSendingInfo packetSendingInfo;
#ifdef ADDON_DB
                        packetSendingInfo.appParam = appParam;
#endif
                        packetSendingInfo.itemSize = clientPtr->itemSize;
                        packetSendingInfo.stats = clientPtr->stats;
                        packetSendingInfo.fragNo = NO_UDP_FRAG;
                        packetSendingInfo.fragSize = 0;


						cout<<APP_GetMsgType(sentMsg)<<endl;

                        node->appData.appTrafficSender->appUdpSend(
                                                node,
                                                sentMsg,
                                                *clientPtr->serverUrl,
                                                clientPtr->localAddr,
                                                APP_EBR_CLIENT,
                                                (short)clientPtr->sourcePort,
                                                packetSendingInfo);
                        clientPtr->sessionLastSent = node->getNodeTime();
					}
}

                        if (clientPtr->itemsToSend > 0)
                        {
                            clientPtr->itemsToSend--;
                        }

                        if (clientPtr->sessionIsClosed == FALSE)
                        {
                            AppEbrClientScheduleNextPkt(node, clientPtr);
                        }
                    }
					
                    else
                    {
                        clientPtr->sessionStart = 0;
                        clientPtr->sessionIsClosed = TRUE;
                    }

                    break;
                }

                default:
                {
#ifndef EXATA
                    assert(FALSE);
#endif
                }
            }

            break;
        }
        default:
           TIME_PrintClockInSecond(node->getNodeTime(), buf, node);
           sprintf(error, "EBR Client: at time %sS, node %d "
                   "received message of unknown type"
                   " %d\n", buf, node->nodeId, msg->eventType);
#ifndef EXATA
           ERROR_ReportError(error);
#endif
    }

    MESSAGE_Free(node, msg);
}

#ifdef ADDON_NGCNMS
// reset source address of application so that we
// can route packets correctly.
void
AppEbrClientReInit(Node* node, Address sourceAddr)
{
    AppInfo *appList = node->appData.appPtr;
    AppDataEbrClient *ebrClient;
    int i;

    for (; appList != NULL; appList = appList->appNext)
    {
        if (appList->appType == APP_EBR_CLIENT)
        {
            ebrClient = (AppDataEbrClient *) appList->appDetail;

            for (i=0; i< node->numberInterfaces; i++)
            {
                // currently only support ipv4. May be better way to compare this.
                if (ebrClient->localAddr.interfaceAddr.ipv4 == node->iface[i].oldIpAddress)
                {
                    memcpy(&(ebrClient->localAddr), &sourceAddr, sizeof(Address));
                }
            }
        }
    }

}
#endif

/*
 * NAME:        AppEbrClientInit.
 * PURPOSE:     Initialize a EbrClient session.
 * PARAMETERS:  node - pointer to the node,
 *              serverAddr - address of the server,
 *              itemsToSend - number of items to send,
 *              itemSize - size of each packet,
 *              interval - interval of packet transmission rate.
 *              startTime - time until the session starts,
 *              endTime - time until the session ends,
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
    BOOL isMdpEnabled,
    BOOL isProfileNameSet,
    char* profileName,
    Int32 uniqueId,
    const NodeInput* nodeInput,
    clocktype startTime,
    clocktype endTime)
{
    char error[MAX_STRING_LENGTH];
    AppTimer *timer;
    AppDataEbrClient *clientPtr;
    Message *timerMsg;
    int minSize;
    startTime -= getSimStartTime(node);
    endTime -= getSimStartTime(node);
	strnum=itemsToSend;

    // Validate the given tos for this application.
    if (/*tos < 0 || */tos > 255)
    {
        sprintf(error, "EBR Client: Node %d should have tos value "
            "within the range 0 to 255.\n",
            node->nodeId);
        ERROR_ReportError(error);
    }

#ifdef ADDON_NGCNMS
    clocktype origStart = startTime;

    // for restarts (node and interface).
    if (NODE_IsDisabled(node))
    {
        clocktype currTime = node->getNodeTime();
        if (endTime <= currTime)
        {
            // application already finished.
            return;
        }
        else if (startTime >= currTime)
        {
            // application hasnt started yet
            startTime -= currTime;
        }
        else
        {
            // application has already started,
            // so pick up where we left off.

            // try to determine the number of items already sent.
            if (itemsToSend != 0)
            {
                clocktype diffTime = currTime - startTime;
                int itemsSent = diffTime / interval;

                if (itemsToSend < itemsSent)
                {
                    return;
                }

                itemsToSend -= itemsSent;
            }

            // start right away.
            startTime = 0;

        }
    }
#endif

    clientPtr = AppEbrClientNewEbrClient(node,
                                         clientAddr,
                                         serverAddr,
                                         itemsToSend,
                                         itemSize,
                                         interval,
#ifndef ADDON_NGCNMS
                                         startTime,
#else
                                         origStart,
#endif
                                         endTime,
                                         (TosType) tos,
                                         appName);

    if (clientPtr == NULL)
    {
        sprintf(error,
                "EBR Client: Node %d cannot allocate memory for "
                    "new client\n", node->nodeId);

        ERROR_ReportError(error);
    }
    // dns
    // Skipped if the server network type is not valid
    // it should happen only when the server address is given by a URL
    // statistics will be initialized when url is resolved by dns
    if (serverAddr.networkType != NETWORK_INVALID && node->appData.appStats)
    {
        std::string customName;
        if (clientPtr->applicationName->empty())
        {
            customName = "EBR Client";
        }
        else
        {
            customName = *clientPtr->applicationName;
        }
        clientPtr->stats = new STAT_AppStatistics(
                                      node,
                                      "ebr",
                                      STAT_Unicast,
                                      STAT_AppSender,
                                      customName.c_str());
        clientPtr->stats->Initialize(node,
                                     clientAddr,
                                     serverAddr,
                                     (STAT_SessionIdType)clientPtr->uniqueId,
                                     clientPtr->uniqueId);

        clientPtr->stats->setTos(tos);
        // Create application hop by hop flow filter button
        if (GUI_IsAppHopByHopFlowEnabled())
        {
            GUI_CreateAppHopByHopFlowFilter(clientPtr->stats->GetSessionId(),
                node->nodeId, srcString, destString, "EBR");
        }
    }
    // client pointer initialization with respect to mdp
    clientPtr->isMdpEnabled = isMdpEnabled;
    clientPtr->mdpUniqueId = uniqueId;

    if (isMdpEnabled)
    {
        // MDP Layer Init
        MdpLayerInit(node,
                     clientAddr,
                     serverAddr,
                     clientPtr->sourcePort,
                     APP_EBR_CLIENT,
                     isProfileNameSet,
                     profileName,
                     uniqueId,
                     nodeInput,
                     -1,
                     TRUE);
    }

    if (node->transportData.rsvpProtocol && isRsvpTeEnabled)
    {
        // Note: RSVP is enabled for Ipv4 networks only.
        Message *rsvpRegisterMsg;
        AppToRsvpSend *info;

        rsvpRegisterMsg = MESSAGE_Alloc(node,
                                  TRANSPORT_LAYER,
                                  TransportProtocol_RSVP,
                                  MSG_TRANSPORT_RSVP_InitApp);

        MESSAGE_InfoAlloc(node,
                           rsvpRegisterMsg,
                           sizeof(AppToRsvpSend));

        info = (AppToRsvpSend *) MESSAGE_ReturnInfo(rsvpRegisterMsg);

        info->sourceAddr = GetIPv4Address(clientAddr);
        info->destAddr = GetIPv4Address(serverAddr);

        info->upcallFunctionPtr = NULL;

        MESSAGE_Send(node, rsvpRegisterMsg, startTime);
    }


    timerMsg = MESSAGE_Alloc(node,
                              APP_LAYER,
                              APP_EBR_CLIENT,
                              MSG_APP_TimerExpired);

    MESSAGE_InfoAlloc(node, timerMsg, sizeof(AppTimer));

    timer = (AppTimer *)MESSAGE_ReturnInfo(timerMsg);

    timer->sourcePort = clientPtr->sourcePort;
    timer->type = APP_TIMER_SEND_PKT;

#ifdef DEBUG
    {
        char clockStr[MAX_STRING_LENGTH];
        TIME_PrintClockInSecond(startTime, clockStr, node);
        printf("EBR Client: Node %ld starting client at %sS\n",
               node->nodeId, clockStr);
    }
#endif /* DEBUG */

    MESSAGE_Send(node, timerMsg, startTime);

#ifdef ADDON_NGCNMS

    clientPtr->lastTimer = timerMsg;

#endif
    // Dynamic Address
    // update the client with url info if destination configured as URL
    if (serverAddr.networkType == NETWORK_INVALID && destString)
    {
        // set the dns info in client pointer if server url
        // is present
        // set the server url if it is not localhost
        if (node->appData.appTrafficSender->ifUrlLocalHost(destString)
                                                                    == FALSE)
        {
            node->appData.appTrafficSender->appClientSetDnsInfo(
                                        node,
                                        destString,
                                        clientPtr->serverUrl);
        }
    }
    // Update the EBR clientPtr with address information
    AppEbrClientAddAddressInformation(node, clientPtr);
}

/*
 * NAME:        AppEbrClientPrintStats.
 * PURPOSE:     Prints statistics of a EbrClient session.
 * PARAMETERS:  node - pointer to the this node.
 *              clientPtr - pointer to the ebr client data structure.
 * RETURN:      none.
 */
void AppEbrClientPrintStats(Node *node, AppDataEbrClient *clientPtr) {

    char addrStr[MAX_STRING_LENGTH];
    char sessionStatusStr[MAX_STRING_LENGTH];

    char buf[MAX_STRING_LENGTH];

    if (clientPtr->sessionIsClosed == FALSE &&
        clientPtr->remoteAddr.networkType != NETWORK_INVALID) {
        clientPtr->sessionFinish = node->getNodeTime();
        sprintf(sessionStatusStr, "Not closed");
        if (!clientPtr->stats->IsSessionFinished(STAT_Unicast))
        {
            clientPtr->stats->SessionFinish(node);
        }
    }
    else {
        sprintf(sessionStatusStr, "Closed");
    }


    if (clientPtr->remoteAddr.networkType == NETWORK_ATM)
    {
        const LogicalSubnet* dstLogicalSubnet =
            AtmGetLogicalSubnetFromNodeId(
            node,
            clientPtr->remoteAddr.interfaceAddr.atm.ESI_pt1,
            0);
        IO_ConvertIpAddressToString(
            dstLogicalSubnet->ipAddress,
            addrStr);
    }
    else
    {
        // dns
        if (clientPtr->remoteAddr.networkType != NETWORK_INVALID)
        {
            IO_ConvertIpAddressToString(&clientPtr->remoteAddr, addrStr);
        }
        else
        {
            strcpy(addrStr, (char*)clientPtr->serverUrl->c_str());
        }

    }

    sprintf(buf, "Server Address = %s", addrStr);
    IO_PrintStat(
        node,
        "Application",
        "EBR Client",
        ANY_DEST,
        clientPtr->sourcePort,
        buf);


    sprintf(buf, "Session Status = %s", sessionStatusStr);
    IO_PrintStat(
        node,
        "Application",
        "EBR Client",
        ANY_DEST,
        clientPtr->sourcePort,
        buf);

}

/*
 * NAME:        AppEbrClientFinalize.
 * PURPOSE:     Collect statistics of a EbrClient session.
 * PARAMETERS:  node - pointer to the node.
 *              appInfo - pointer to the application info data structure.
 * RETURN:      none.
 */
void
AppEbrClientFinalize(Node *node, AppInfo* appInfo)
{
    AppDataEbrClient *clientPtr = (AppDataEbrClient*)appInfo->appDetail;

    if (node->appData.appStats == TRUE)
    {
        AppEbrClientPrintStats(node, clientPtr);

        // Print stats in .stat file using Stat APIs
        if (clientPtr->stats)
        {
            clientPtr->stats->Print(
                node,
                "Application",
                "EBR Client",
                ANY_ADDRESS,
                clientPtr->sourcePort);
        }
    }
    if (clientPtr->serverUrl)
    {
        delete (clientPtr->serverUrl);
        clientPtr->serverUrl = NULL;
    }
}

/*
 * NAME:        AppEbrClientGetEbrClient.
 * PURPOSE:     search for a ebr client data structure.
 * PARAMETERS:  node - pointer to the node.
 *              sourcePort - source port of the ebr client.
 * RETURN:      the pointer to the ebr client data structure,
 *              NULL if nothing found.
 */
AppDataEbrClient *
AppEbrClientGetEbrClient(Node *node, short sourcePort)
{
    AppInfo *appList = node->appData.appPtr;
    AppDataEbrClient *ebrClient;

    for (; appList != NULL; appList = appList->appNext)
    {
        if (appList->appType == APP_EBR_CLIENT)
        {
            ebrClient = (AppDataEbrClient *) appList->appDetail;

            if (ebrClient->sourcePort == sourcePort)
            {
                return ebrClient;
            }
        }
    }

    return NULL;
}

/*
 * NAME:        AppEbrClientNewEbrClient.
 * PURPOSE:     create a new ebr client data structure, place it
 *              at the beginning of the application list.
 * PARAMETERS:  node - pointer to the node.
 *              remoteAddr - remote address.
 *              itemsToSend - number of ebr items to send in simulation.
 *              itemSize - size of each packet.
 *              interval - time between two packets.
 *              startTime - when the node will start sending.
 * RETURN:      the pointer to the created ebr client data structure,
 *              NULL if no data structure allocated.
 */
AppDataEbrClient *
AppEbrClientNewEbrClient(Node *node,
                         Address localAddr,
                         Address remoteAddr,
                         Int32 itemsToSend,
                         Int32 itemSize,
                         clocktype interval,
                         clocktype startTime,
                         clocktype endTime,
                         TosType tos,
                         char* appName)
{
    AppDataEbrClient *ebrClient;

    ebrClient = (AppDataEbrClient *)
                MEM_malloc(sizeof(AppDataEbrClient));
    memset(ebrClient, 0, sizeof(AppDataEbrClient));

    /*
     * fill in ebr info.
     */
    memcpy(&(ebrClient->localAddr), &localAddr, sizeof(Address));
    memcpy(&(ebrClient->remoteAddr), &remoteAddr, sizeof(Address));
    ebrClient->interval = interval;
#ifndef ADDON_NGCNMS
    ebrClient->sessionStart = node->getNodeTime() + startTime;
#else
    if (!NODE_IsDisabled(node))
    {
        ebrClient->sessionStart = node->getNodeTime() + startTime;
    }
    else
    {
        // start time was already figured out in caller function.
        ebrClient->sessionStart = startTime;
    }
#endif
    ebrClient->sessionIsClosed = FALSE;
    ebrClient->sessionLastSent = node->getNodeTime();
    ebrClient->sessionFinish = node->getNodeTime();
    ebrClient->endTime = endTime;
    ebrClient->itemsToSend = itemsToSend;
	ebrClient->itemssum = itemsToSend;
    ebrClient->itemSize = itemSize;
    ebrClient->sourcePort = node->appData.nextPortNum++;
    ebrClient->seqNo = 0;
    ebrClient->tos = tos;
    ebrClient->uniqueId = node->appData.uniqueId++;
    //client pointer initialization with respect to mdp
    ebrClient->isMdpEnabled = FALSE;
    ebrClient->mdpUniqueId = -1;  //invalid value
    ebrClient->stats = NULL;
    // dns
    ebrClient->serverUrl = new std::string();
    ebrClient->serverUrl->clear();
    ebrClient->destNodeId = ANY_NODEID;
    ebrClient->clientInterfaceIndex = ANY_INTERFACE;
    ebrClient->destInterfaceIndex = ANY_INTERFACE;
    if (appName)
    {
        ebrClient->applicationName = new std::string(appName);
    }
    else
    {
        ebrClient->applicationName = new std::string();
    }
#ifdef ADDON_DB
    ebrClient->sessionId = ebrClient->uniqueId;

    // dns
    // Skipped if the server network type is not valid
    // it should happen only when the server is given by a URL
    // receiverId will be initialized when url is resolved
    if (ebrClient->remoteAddr.networkType != NETWORK_INVALID)
    {
        if (Address_IsAnyAddress(&(ebrClient->remoteAddr)) ||
            Address_IsMulticastAddress(&ebrClient->remoteAddr))
        {
            ebrClient->receiverId = 0;
        }
        else
        {
            ebrClient->receiverId =
                MAPPING_GetNodeIdFromInterfaceAddress(node, remoteAddr);
        }
    }
#endif // ADDON_DB

    // Add EBR variables to hierarchy
    std::string path;
    D_Hierarchy *h = &node->partitionData->dynamicHierarchy;

    if (h->CreateApplicationPath(
            node,                   // node
            "ebrClient",            // protocol name
            ebrClient->sourcePort,  // port
            "interval",             // object name
            path))                  // path (output)
    {
        h->AddObject(
            path,
            new D_ClocktypeObj(&ebrClient->interval));
    }

// The HUMAN_IN_THE_LOOP_DEMO is part of a gui user-defined command
// demo.
// The type of service value for this EBR application is added to
// the dynamic hierarchy so that the user-defined-command can change
// it during simulation.
#ifdef HUMAN_IN_THE_LOOP_DEMO
    if (h->CreateApplicationPath(
            node,
            "ebrClient",
            ebrClient->sourcePort,
            "tos",                  // object name
            path))                  // path (output)
    {
        h->AddObject(
            path,
            new D_UInt32Obj(&ebrClient->tos));
    }
#endif

#ifdef DEBUG
    {
        char clockStr[MAX_STRING_LENGTH];
        char localAddrStr[MAX_STRING_LENGTH];
        char remoteAddrStr[MAX_STRING_LENGTH];

        IO_ConvertIpAddressToString(&ebrClient->localAddr, localAddrStr);
        IO_ConvertIpAddressToString(&ebrClient->remoteAddr, remoteAddrStr);

        printf("EBR Client: Node %u created new ebr client structure\n",
               node->nodeId);
        printf("    localAddr = %s\n", localAddrStr);
        printf("    remoteAddr = %s\n", remoteAddrStr);
        TIME_PrintClockInSecond(ebrClient->interval, clockStr);
        printf("    interval = %s\n", clockStr);
        TIME_PrintClockInSecond(ebrClient->sessionStart, clockStr, node);
        printf("    sessionStart = %sS\n", clockStr);
        printf("    itemsToSend = %u\n", ebrClient->itemsToSend);
        printf("    itemSize = %u\n", ebrClient->itemSize);
        printf("    sourcePort = %ld\n", ebrClient->sourcePort);
        printf("    seqNo = %ld\n", ebrClient->seqNo);
    }
#endif /* DEBUG */

    APP_RegisterNewApp(node, APP_EBR_CLIENT, ebrClient);

    return ebrClient;
}

/*
 * NAME:        AppEbrClientScheduleNextPkt.
 * PURPOSE:     schedule the next packet the client will send.  If next packet
 *              won't arrive until the finish deadline, schedule a close.
 * PARAMETERS:  node - pointer to the node,
 *              clientPtr - pointer to the ebr client data structure.
 * RETRUN:      none.
 */

void printheadder(Node *node,EbrHeader data){
					printf("node %d is recieve the message",node->nodeId);
					printf("recieve the package from %d\n,and the real dest is %d\n",NetworkIpGetInterfaceAddress(node,0),data.destaddr.interfaceAddr.ipv4);
					printf("ebrheader.type =%c\n",data.type);
					printf("ebrheader.range =%d\n",data.range);
					printf("ebrheader.codenumber =%d\n",data.codenumber);
					printf("ebrheader.pacsum =%d\n",data.pacsum);
					printf("ebrheader.strnum =%d\n",data.strnum);
					printf("ebrheader.pacnum =%d\n",data.pacnum);
					printf("ebrheader.length =%d\n",data.length);
					printf("ebrheader.sourceaddr =%d\n",data.sourceaddr.interfaceAddr.ipv4);
					printf("ebrheader.destaddr =%d\n",data.destaddr.interfaceAddr.ipv4);
					printf("ebrheader.sourceport =%d\n",data.sourceport);
					printf("ebrheader.destport =%d\n",data.destport);
}
void //inline//
AppEbrClientScheduleNextPkt(Node *node, AppDataEbrClient *clientPtr)
{
    AppTimer *timer;
    Message *timerMsg;

    timerMsg = MESSAGE_Alloc(node,
                              APP_LAYER,
                              APP_EBR_CLIENT,
                              MSG_APP_TimerExpired);

    MESSAGE_InfoAlloc(node, timerMsg, sizeof(AppTimer));

    timer = (AppTimer *)MESSAGE_ReturnInfo(timerMsg);
    timer->sourcePort = clientPtr->sourcePort;
    timer->type = APP_TIMER_SEND_PKT;

#ifdef DEBUG
    {
        char clockStr[24];
        printf("EBR Client: Node %u scheduling next data packet\n",
               node->nodeId);
        printf("    timer type is %d\n", timer->type);
        TIME_PrintClockInSecond(clientPtr->interval, clockStr);
        printf("    interval is %sS\n", clockStr);
    }
#endif /* DEBUG */

#ifdef ADDON_NGCNMS

    clientPtr->lastTimer = timerMsg;

#endif

    MESSAGE_Send(node, timerMsg, clientPtr->interval);
}

void Resend(int strnum){
	;
}

void SendToSource(int strnum,Node *node,EbrHeader data){
	return;
						EbrHeader ebrheader;
						ebrheader.type = 'c';
						char ebrData[sizeof(ebrheader)];
						Message* sentMsg = APP_UdpCreateMessage(
                                                node,
												data.destaddr,
												data.destport,
												data.sourceaddr,
                                                (short) APP_EBR_SERVER,
                                                TRACE_EBR);
						APP_AddHeader(
                                node,
                                sentMsg,
                                (char *) &ebrheader,
                                sizeof(ebrheader));
                        AppUdpPacketSendingInfo packetSendingInfo;
								packetSendingInfo.itemSize = 0;
								packetSendingInfo.stats = FALSE;
								packetSendingInfo.fragNo = NO_UDP_FRAG;
								packetSendingInfo.fragSize = 0;
								std::string* serverUrl = new std::string();
								serverUrl->clear();
								node->appData.appTrafficSender->appUdpSend(
														node,
														sentMsg,
														*serverUrl,
														data.sourceaddr,
														APP_EBR_CLIENT,
														data.sourceport,
														packetSendingInfo);
							return;
}
/*
 * NAME:        AppLayerEbrServer.

 * PURPOSE:     Models the behaviour of EbrServer Server on receiving the
 *              message encapsulated in msg.
 * PARAMETERS:  node - pointer to the node which received the message.
 *              msg - message received by the layer
 * RETURN:      none.
 */
void
AppLayerEbrServer(Node *node, Message *msg)
{
    char error[MAX_STRING_LENGTH];
    AppDataEbrServer *serverPtr;
	cout<<APP_GetMsgType(msg)<<endl;

    switch (msg->eventType)
    {
        case MSG_APP_FromTransport:
        {
            UdpToAppRecv *info;
            //EbrData data;
			EbrHeader data;
#ifdef ADDON_DB
            AppMsgStatus msgStatus = APP_MSG_OLD;
#endif // ADDON_DB

			printf("node %d is runing !\n",node->nodeId);
            info = (UdpToAppRecv *) MESSAGE_ReturnInfo(msg);
            memcpy(&data, MESSAGE_ReturnPacket(msg), sizeof(data));
            // trace recd pkt
            ActionData acnData;
            acnData.actionType = RECV;
            acnData.actionComment = NO_COMMENT;
            TRACE_PrintTrace(node,
                             msg,
                             TRACE_APPLICATION_LAYER,
                             PACKET_IN,
                             &acnData);

#ifdef DEBUG
            {
                char clockStr[MAX_STRING_LENGTH];
                char addrStr[MAX_STRING_LENGTH];
                TIME_PrintClockInSecond(data.txTime, clockStr, node);
                IO_ConvertIpAddressToString(&info->sourceAddr, addrStr);

                printf("EBR Server %ld: packet transmitted at %sS\n",
                       node->nodeId, clockStr);
                TIME_PrintClockInSecond(node->getNodeTime(), clockStr, node);
                printf("    received at %sS\n", clockStr);
                printf("    client is %s\n", addrStr);
                printf("    connection Id is %d\n", data.sourcePort);
                printf("    seqNo is %d\n", data.seqNo);
            }
#endif /* DEBUG */

            serverPtr = AppEbrServerGetEbrServer(node,
                                                 info->sourceAddr,
												 data.sourceport);

            /* New connection, so create new ebr server to handle client. */
            if (serverPtr == NULL)
            {		
					//回到了起点，空操作返回
				if(NetworkIpGetInterfaceAddress(node,0)==data.sourceaddr.interfaceAddr.ipv4){
					if(data.type=='r'){
						//重传
						Resend(data.strnum);
					}
					return;
				}
				if(NetworkIpGetInterfaceAddress(node,0)!=data.destaddr.interfaceAddr.ipv4){
					//不接收接收过的和下一代的
					if(!record[node->nodeId][data.strnum].isfirst){
						if(record[node->nodeId][data.strnum].codenumber != data.codenumber ||
							(record[node->nodeId][data.strnum].pacnum > data.pacnum)){
								printf("cant 2\n");
								printf("record[node->nodeId][data.strnum].codenumber = %d\ndata.codenumber = %d\n record[node->nodeId][data.strnum].pacnum = %d\n data.pacnum = %d\n",
									record[node->nodeId][data.strnum].codenumber,data.codenumber,record[node->nodeId][data.strnum].pacnum,data.pacnum);
							return;
						}
					}else{
						record[node->nodeId][data.strnum].isfirst = false;
						record[node->nodeId][data.strnum].codenumber = data.codenumber;
					}
					//设置node中记录的信息
						record[node->nodeId][data.strnum].pacnum = data.pacnum;
				}
					
				//return;
				printf("check if is center\n");
				//kai shi zhuan fa  , zhongjianjiejian
				if(NetworkIpGetInterfaceAddress(node,0)!=data.destaddr.interfaceAddr.ipv4){
					printf("it is center\n");
					//#define splitnum 2
					//#define maxcodenum 4
					//#define maxrange 5
					//return;
					printf("real payload is = \n");
					printx(data.payload);
					printf("\n");
						EbrHeader ebrheader;
						ebrheader.type = data.type;
						ebrheader.range = data.range;
						ebrheader.codetype = data.codetype;
						ebrheader.codenumber = data.codenumber;
						ebrheader.pacsum = splitnum;
						ebrheader.strnum = data.strnum;
						ebrheader.pacnum = 0;

						ebrheader.length = data.length/splitnum;
						ebrheader.sourceaddr=data.sourceaddr;
						ebrheader.destaddr=data.destaddr;
						ebrheader.sourceport=data.sourceport;
						ebrheader.destport=data.destport;
						ebrheader.check=data.check;
						printf("node is save message info:\n");
						printheadder(node,data);
					if(ebrheader.range == maxrange){
						printf("node %d cant send to dest",node->nodeId);
						return;
					}
					else if(ebrheader.codenumber == maxcodenum){
						printf("node %d is sending as center",node->nodeId);
						data.range = data.range+1;		
						printf("maxcode reach node is sending message info:\n");
						printheadder(node,data);
						 memcpy(MESSAGE_ReturnPacket(msg),&data, sizeof(data));
						 for (int i = 0; i < node->numberInterfaces; i++)
							{
							 Address temp = data.destaddr;
							 //temp.interfaceAddr.ipv4 = ANY_DEST;
							 if (NetworkIpIsWiredNetwork (node, i))			 
								 temp.interfaceAddr.ipv4 = NetworkIpGetInterfaceBroadcastAddress(node, i);			 
							 else temp.interfaceAddr.ipv4 = ANY_DEST;
						printf("send to port%d ,and addr%d\n",i);
						 printaddrString(temp.interfaceAddr.ipv4);
						 printaddrString(GetDefaultIPv4InterfaceAddress(node));
						 Address current = data.destaddr;
						 current.interfaceAddr.ipv4 = GetDefaultIPv4InterfaceAddress(node);
						Message* sentMsg = APP_UdpCreateMessage(
                                                node,
                                                current,
                                                i,
                                                temp,
                                                (short) APP_EBR_SERVER,
                                                TRACE_EBR);
						char ebrData[sizeof(data)];
						memset(ebrData, 0, sizeof(data));
						memcpy(ebrData, (char *) &data, sizeof(data));
								APP_AddHeader(
                                node,
                                sentMsg,
                                ebrData,
                                sizeof(data));

                        int payloadSize =
							data.length;
                        APP_AddVirtualPayload(
                                node,
                                sentMsg,
                                payloadSize);

                        // Add the virtual payload size as info
                        APP_AddInfo(
                                node,
                                sentMsg,
                                (char*) &payloadSize,
                                sizeof(int),
                                INFO_TYPE_DataSize);
						printf("node %d is sending as center\n1\n",node->nodeId);
								AppUdpPacketSendingInfo packetSendingInfo;
								packetSendingInfo.itemSize = sizeof(MESSAGE_ReturnPacket(msg));
								packetSendingInfo.stats = FALSE;
								packetSendingInfo.fragNo = NO_UDP_FRAG;
								packetSendingInfo.fragSize = 0;
								std::string* serverUrl = new std::string();
								serverUrl->clear();
								node->appData.appTrafficSender->appUdpSend(
														node,
														sentMsg,
														*serverUrl,
														current,
														APP_EBR_CLIENT,
														data.sourceport,
														packetSendingInfo);
							}
						return;
					}
					else{
						printf("node %d is sending as center coding----ing\n ",node->nodeId);
						printf("node is sending message info:\n");
						for(int v=1;v<=1;v++){
							EbrHeader tem;
							/*tem.type = 'd';
							if(data.type == 'c' && v == splitnum){
								tem.type = data.type;
							}	*/
							tem.type = data.type;
							tem.range = data.range+1;
							tem.codetype = data.codetype;
							tem.codenumber = data.codenumber+1;
							//tem.pacsum = data.pacsum*2;
							tem.pacsum = data.pacnum;
							tem.strnum = data.strnum;
							//tem.pacnum = (data.pacnum-1)*splitnum+v;
							tem.pacnum = data.pacnum;
							//tem.length = data.length/splitnum;
							tem.length = data.length;
							tem.sourceaddr=data.sourceaddr;
							tem.destaddr=data.destaddr;
							tem.sourceport=data.sourceport;
							tem.destport=data.destport;
							tem.check=data.check;
							/*tem.payload = (char*)malloc(tem.length);
							for(int pi=0;pi<tem.length;pi++){
								tem.payload[pi] = data.payload[(v-1)*tem.length+pi];
							}
							printf("codenumber %d split %d info is :\n",tem.codenumber,v);*/
							    CODEC codec2(splitnum, data.length);
								codec2.RecvMessage(data.payload, data.coef);
								tem.coef = codec2.encode()[0];
								tem.payload = (char*)malloc(data.length*sizeof(char));
								codec2.get_encode_message(tem.payload);
							printheadder(node,tem);
							for (int i = 0; i < node->numberInterfaces; i++)
							{
								 Address temp = data.destaddr;
								 if (NetworkIpIsWiredNetwork (node, i))			 
									temp.interfaceAddr.ipv4 = NetworkIpGetInterfaceBroadcastAddress(node, i);			 
								 else 
								 	temp.interfaceAddr.ipv4 = ANY_DEST;
								 printf("send to port%d ,and addr%d\n",i);
								 printaddrString(temp.interfaceAddr.ipv4);
								 printaddrString(GetDefaultIPv4InterfaceAddress(node));
								 Address current = data.destaddr;
								 current.interfaceAddr.ipv4 = GetDefaultIPv4InterfaceAddress(node);	
								 Message* sentMsg = APP_UdpCreateMessage(
	                                                node,
	                                                current,
	                                                i,
	                                                temp,
	                                                (short) APP_EBR_SERVER,
	                                                TRACE_EBR);
								char ebrData[sizeof(tem)];
								memset(ebrData, 0, sizeof(tem));
								memcpy(ebrData, (char *) &tem, sizeof(tem));
								APP_AddHeader(
	                                node,
	                                sentMsg,
	                                ebrData,
	                                sizeof(tem));
								int payloadSize =
								tem.length;
		                        APP_AddVirtualPayload(
		                                node,
		                                sentMsg,
		                                payloadSize);

		                        APP_AddInfo(
		                                node,
		                                sentMsg,
		                                (char*) &payloadSize,
		                                sizeof(int),
		                                INFO_TYPE_DataSize);
		                        AppUdpPacketSendingInfo packetSendingInfo;
									packetSendingInfo.itemSize = sizeof(MESSAGE_ReturnPacket(msg));
									packetSendingInfo.stats = FALSE;
									packetSendingInfo.fragNo = NO_UDP_FRAG;
									packetSendingInfo.fragSize = 0;
									std::string* serverUrl = new std::string();
									serverUrl->clear();
									node->appData.appTrafficSender->appUdpSend(
															node,
															sentMsg,
															*serverUrl,
															current,
															APP_EBR_CLIENT,
															tem.sourceport,
															packetSendingInfo);
							}
						}
						
						return;
					}
				}
				if(NetworkIpGetInterfaceAddress(node,0)==data.destaddr.interfaceAddr.ipv4){
					printf("arrive to the end!\n");
					if (data.type == 'd')
					{
						store[data.strnum][data.codenumber][data.pacnum] = data;
						printf("stream %d store codenum %d spiltnum %d\n",data.strnum,data.codenumber,data.pacnum);
						if(data.codenumber>streamcodenum[data.strnum]){
							streamcodenum[data.strnum] = data.codenumber;
						}
					}
					else if (data.type == 'c')
					{
						store[data.strnum][data.codenumber][data.pacnum] = data;
						printf("stream %d store codenum %d spiltnum %d\n",data.strnum,data.codenumber,data.pacnum);
						if(data.codenumber>streamcodenum[data.strnum]){
							streamcodenum[data.strnum] = data.codenumber;
						}
						if(streamcodenum[data.strnum]!=0){
							for(int s=1;s<= data.pacsum;s++){
								printf("stream %d have codenum %d spiltnum %d\n",store[data.strnum][data.codenumber][s].strnum,store[data.strnum][data.codenumber][s].codenumber,store[data.strnum][data.codenumber][s].pacnum);
							}
						}else{
							printf("stream %d have codenum %d spiltnum %d\n",store[data.strnum][data.codenumber][1].strnum,store[data.strnum][data.codenumber][1].codenumber,store[data.strnum][data.codenumber][1].pacnum);
						}
						//如果不满秩，发回源节点报文，标注流信息，重新发送指定流
						if(data.pacnum!=data.pacsum){
							SendToSource(data.strnum,node,data);
						}else{
						CODEC codec3(splitnum, data.length);
						for(int s=1;s<= data.pacsum;s++){
							codec3.RecvMessage(store[data.strnum][data.codenumber][s].payload, store[data.strnum][data.codenumber][s].coef);
							printx(store[data.strnum][data.codenumber][s].payload);
						}
						char* decode_buf = (char*)malloc(data.pacsum * data.length*sizeof(char));
						if (codec3.LinkMsg()) {
							std::cout << "Link Finished\n";
							codec3.decode();
							codec3.get_decode_message(decode_buf);
							std::cout << "Decode Finished\n";
							printx(decode_buf);
						}
						}
					}
				}
            }

            break;
        }

        default:
        {
            char buf[MAX_STRING_LENGTH];

            TIME_PrintClockInSecond(node->getNodeTime(), buf, node);
            sprintf(error, "EBR Server: At time %sS, node %d received "
                    "message of unknown type "
                    "%d\n", buf, node->nodeId, msg->eventType);
            ERROR_ReportError(error);
        }
    }

    MESSAGE_Free(node, msg);
}

/*
 * NAME:        AppEbrServerInit.
 * PURPOSE:     listen on EbrServer server port.
 * PARAMETERS:  node - pointer to the node.
 * RETURN:      none.
 */


void
AppEbrServerInit(Node *node)
{
    APP_InserInPortTable(node, APP_EBR_SERVER, APP_EBR_SERVER);
	for(int i=1;i<=strnum;i++){
		record[node->nodeId][i].isfirst=true;
	}
}

/*
 * NAME:        AppEbrServerPrintStats.
 * PURPOSE:     Prints statistics of a EbrServer session.
 * PARAMETERS:  node - pointer to this node.
 *              serverPtr - pointer to the ebr server data structure.
 * RETURN:      none.
 */
void
AppEbrServerPrintStats(Node *node, AppDataEbrServer *serverPtr) {

    char addrStr[MAX_STRING_LENGTH];
    char sessionStatusStr[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];

    if (serverPtr->sessionIsClosed == FALSE) {
        serverPtr->sessionFinish = node->getNodeTime();
        sprintf(sessionStatusStr, "Not closed");
        if (!serverPtr->stats->IsSessionFinished(STAT_Unicast))
        {
            serverPtr->stats->SessionFinish(node);
        }
    }
    else {
        sprintf(sessionStatusStr, "Closed");
    }

    IO_ConvertIpAddressToString(&serverPtr->remoteAddr, addrStr);

    sprintf(buf, "Client address = %s", addrStr);
    IO_PrintStat(
        node,
        "Application",
        "EBR Server",
        ANY_DEST,
        serverPtr->sourcePort,
        buf);


    sprintf(buf, "Session Status = %s", sessionStatusStr);
    IO_PrintStat(
        node,
        "Application",
        "EBR Server",
        ANY_DEST,
        serverPtr->sourcePort,
        buf);


#ifdef LTE_LIB
#ifdef LTE_LIB_LOG
#ifdef LTE_LIB_VALIDATION_LOG
    lte::LteLog::InfoFormat(node,-1,
            "APP",
            "%s,%s,%llu",
            "AppThroughput",
            addrStr,
            throughput);
#endif
#endif
#endif
}

/*
 * NAME:        AppEbrServerFinalize.
 * PURPOSE:     Collect statistics of a EbrServer session.
 * PARAMETERS:  node - pointer to the node.
 *              appInfo - pointer to the application info data structure.
 * RETURN:      none.
 */
void
AppEbrServerFinalize(Node *node, AppInfo* appInfo)
{
    AppDataEbrServer *serverPtr = (AppDataEbrServer*)appInfo->appDetail;

    if (node->appData.appStats == TRUE)
    {
        AppEbrServerPrintStats(node, serverPtr);
        // Print stats in .stat file using Stat APIs
        if (serverPtr->stats)
        {
            serverPtr->stats->Print(node,
                                    "Application",
                                    "EBR Server",
                                    ANY_ADDRESS,
                                    serverPtr->sourcePort);
        }
    }
#ifdef ADDON_DB
    if (serverPtr->seqNumCache != NULL)
    {
        delete serverPtr->seqNumCache;
        serverPtr->seqNumCache = NULL;
    }
#endif // ADDON_DB
}

/*
 * NAME:        AppEbrServerGetEbrServer.
 * PURPOSE:     search for a ebr server data structure.
 * PARAMETERS:  node - ebr server.
 *              remoteAddr - ebr client sending the data.
 *              sourcePort - sourcePort of ebr client/server pair.
 * RETURN:      the pointer to the ebr server data structure,
 *              NULL if nothing found.
 */
AppDataEbrServer * //inline//
AppEbrServerGetEbrServer(
    Node *node, Address remoteAddr, short sourcePort)
{
    AppInfo *appList = node->appData.appPtr;
    AppDataEbrServer *ebrServer;

    for (; appList != NULL; appList = appList->appNext)
    {
        if (appList->appType == APP_EBR_SERVER)
        {
            ebrServer = (AppDataEbrServer *) appList->appDetail;

            if ((ebrServer->sourcePort == sourcePort) &&
                IO_CheckIsSameAddress(
                    ebrServer->remoteAddr, remoteAddr))
            {
                return ebrServer;
            }
       }
   }

    return NULL;
}

/*
 * NAME:        AppEbrServerNewEbrServer.
 * PURPOSE:     create a new ebr server data structure, place it
 *              at the beginning of the application list.
 * PARAMETERS:  node - pointer to the node.
 *              localAddr - local address.
 *              remoteAddr - remote address.
 *              sourcePort - sourcePort of ebr client/server pair.
 * RETRUN:      the pointer to the created ebr server data structure,
 *              NULL if no data structure allocated.
 */
AppDataEbrServer * //inline//
AppEbrServerNewEbrServer(Node *node,
                         Address localAddr,
                         Address remoteAddr,
                         short sourcePort)
{
    AppDataEbrServer *ebrServer;

    ebrServer = (AppDataEbrServer *)
                MEM_malloc(sizeof(AppDataEbrServer));
    memset(ebrServer, 0, sizeof(AppDataEbrServer));
    /*
     * Fill in ebr info.
     */
    memcpy(&(ebrServer->localAddr), &localAddr, sizeof(Address));
    memcpy(&(ebrServer->remoteAddr), &remoteAddr, sizeof(Address));
    ebrServer->sourcePort = sourcePort;
    ebrServer->sessionStart = node->getNodeTime();
    ebrServer->sessionFinish = node->getNodeTime();
    ebrServer->sessionIsClosed = FALSE;
    ebrServer->seqNo = 0;
    ebrServer->uniqueId = node->appData.uniqueId++;

    //    ebrServer->lastInterArrivalInterval = 0;
    //    ebrServer->lastPacketReceptionTime = 0;
    ebrServer->stats = NULL;
#ifdef ADDON_DB
    ebrServer->sessionId = -1;
    ebrServer->sessionInitiator = 0;
    ebrServer->hopCount = 0;
    ebrServer->seqNumCache = NULL;
#endif // ADDON_DB


#ifdef ADDON_BOEINGFCS
    BOOL retVal = FALSE;

    ebrServer->useSeqNoCheck = TRUE;

    IO_ReadBool(
        node->nodeId,
        ANY_ADDRESS,
        node->partitionData->nodeInput,
        "APP-EBR-USE-SEQUENCE-NUMBER-CHECK",
        &retVal,
        &ebrServer->useSeqNoCheck);

#endif

    APP_RegisterNewApp(node, APP_EBR_SERVER, ebrServer);

    return ebrServer;
}

#ifdef ADDON_NGCNMS

void AppEbrReset(Node* node)
{

    if (node->disabled)
    {
        AppInfo *appList = node->appData.appPtr;
        AppDataEbrClient *ebrClient;

        for (; appList != NULL; appList = appList->appNext)
        {
            if (appList->appType == APP_EBR_CLIENT)
            {
                ebrClient = (AppDataEbrClient *) appList->appDetail;
                MESSAGE_CancelSelfMsg(node, ebrClient->lastTimer);
            }
        }
    }
}

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
                                  AppDataEbrClient* clientPtr)
{
    // Store the client and destination interface index such that we can get
    // the correct address when the application starts
    NodeId destNodeId = MAPPING_GetNodeIdFromInterfaceAddress(
                                                node,
                                                clientPtr->remoteAddr);
    if (destNodeId > 0)
    {
        clientPtr->destNodeId = destNodeId;
        clientPtr->destInterfaceIndex =
            (Int16)MAPPING_GetInterfaceIdForDestAddress(
                                                node,
                                                destNodeId,
                                                clientPtr->remoteAddr);
    }
    // Handle loopback address in destination
    if (destNodeId == INVALID_MAPPING)
    {
        if (NetworkIpCheckIfAddressLoopBack(node, clientPtr->remoteAddr))
        {
            clientPtr->destNodeId = node->nodeId;
            clientPtr->destInterfaceIndex = DEFAULT_INTERFACE;
        }
    }
    clientPtr->clientInterfaceIndex =
        (Int16)MAPPING_GetInterfaceIndexFromInterfaceAddress(
                                            node,
                                            clientPtr->localAddr);
}

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
                                   AppDataEbrClient* clientPtr)
{
    // variable to determine the server current address state
    Int32 serverAddresState = 0;
    // variable to determine the client current address state
    Int32 clientAddressState = 0;

    // Get the current client and destination address if the
    // session is starting
    // Address state is checked only at the session start; if this is not
    // starting of session then return FOUND_ADDRESS
    if (clientPtr->seqNo == 1)
    {
        clientAddressState =
            MAPPING_GetInterfaceAddrForNodeIdAndIntfId(
                                    node,
                                    node->nodeId,
                                    clientPtr->clientInterfaceIndex,
                                    clientPtr->localAddr.networkType,
                                    &(clientPtr->localAddr));

        if (NetworkIpCheckIfAddressLoopBack(node, clientPtr->remoteAddr))
        {
            serverAddresState = clientAddressState;
        }
        else if (clientPtr->destNodeId != ANY_NODEID &&
            clientPtr->destInterfaceIndex != ANY_INTERFACE)
        {
            serverAddresState =
                MAPPING_GetInterfaceAddrForNodeIdAndIntfId(
                                    node,
                                    clientPtr->destNodeId,
                                    clientPtr->destInterfaceIndex,
                                    clientPtr->remoteAddr.networkType,
                                    &(clientPtr->remoteAddr));
        }
    }
    // if either client or server are not in valid address state
    // then mapping will return INVALID_MAPPING
    if (clientAddressState == INVALID_MAPPING ||
        serverAddresState == INVALID_MAPPING)
    {
        return ADDRESS_INVALID;
    }
    return ADDRESS_FOUND;
}

// dns
//---------------------------------------------------------------------------
// NAME      : AppEbrUrlSessionStartCallback
// PURPOSE   : Process Received DNS info.
// PARAMETERS::
// + node       : Node*          : pointer to the node
// + serverAddr : Address*       : server address
// + sourcePort : unsigned short : source port
// + uniqueId   : Int32          : connection id; not used for EBR
// + interfaceId : Int16         : interface index,
// + serverUrl   : std::string   : server URL
// + packetSendingInfo : AppUdpPacketSendingInfo : information required to
//                                                 send buffered application
//                                                 packets in case of UDP
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
                    AppUdpPacketSendingInfo* packetSendingInfo)
{
    EbrData data;
    char ebrData[EBR_HEADER_SIZE];
    AppDataEbrClient* clientPtr = AppEbrClientGetEbrClient(node, sourcePort);

    if (clientPtr == NULL)
    {
        return (FALSE);
    }
    if (clientPtr->remoteAddr.networkType != NETWORK_INVALID)
    {
#ifdef DEBUG
            printf("URL already resolved so ignore this response\n");
#endif
        return (FALSE);
    }
    if (serverAddr->networkType == NETWORK_IPV4)
    {
        if (clientPtr->localAddr.networkType != NETWORK_IPV4)
        {
            ERROR_ReportErrorArgs("EBR Client: Node %d cannot find IPv4"
                           " ebr client for ipv4 dest\n", node->nodeId);
        }
    }
    else if (serverAddr->networkType == NETWORK_IPV6)
    {
        if (clientPtr->localAddr.networkType != NETWORK_IPV6)
        {
            ERROR_ReportErrorArgs("EBR Client: Node %d cannot find IPv6"
                           " ebr client for ipv6 dest\n", node->nodeId);
        }
    }

    // Update client RemoteAddress field and session Start
    memcpy(&clientPtr->remoteAddr, serverAddr, sizeof(Address));

    // update the client address information
    AppEbrClientAddAddressInformation(node,
                                      clientPtr);

#ifdef ADDON_DB

    // initialize receiverid

    if (clientPtr->remoteAddr.networkType != NETWORK_INVALID)
    {
        if (Address_IsAnyAddress(&(clientPtr->remoteAddr)) ||
            Address_IsMulticastAddress(&clientPtr->remoteAddr))
        {
            clientPtr->receiverId = 0;
        }
        else
        {
            clientPtr->receiverId =
                MAPPING_GetNodeIdFromInterfaceAddress(node, *serverAddr);
        }
    }
#endif // ADDON_DB

    if ((serverAddr->networkType != NETWORK_INVALID)
        && (node->appData.appStats))
    {
        std::string customName;
        if (clientPtr->applicationName->empty())
        {
            customName = "EBR Client";
        }
        else
        {
            customName = *clientPtr->applicationName;
        }
        clientPtr->stats = new STAT_AppStatistics(
                                      node,
                                      "ebr",
                                      STAT_Unicast,
                                      STAT_AppSender,
                                      customName.c_str());
        clientPtr->stats->Initialize(node,
                                     clientPtr->localAddr,
                                     clientPtr->remoteAddr,
                                     (STAT_SessionIdType)clientPtr->uniqueId,
                                     clientPtr->uniqueId);

        clientPtr->stats->setTos(clientPtr->tos);
    }

    // First check if end time had already reached..
    if ((clientPtr->endTime != 0) &&
        ((clocktype)node->getNodeTime() > clientPtr->endTime))
    {
        if (node->appData.appStats)
        {
            clientPtr->stats->SessionFinish(node);
        }
        return (FALSE);
    }
#ifdef ADDON_DB
    StatsDBAppEventParam appParam;
    appParam.m_SessionInitiator = node->nodeId;
    appParam.m_ReceiverId = clientPtr->receiverId;
    appParam.SetAppType("EBR");
    appParam.SetFragNum(0);

    if (!clientPtr->applicationName->empty())
    {
        appParam.SetAppName(
            clientPtr->applicationName->c_str());
    }
    appParam.SetReceiverAddr(&clientPtr->remoteAddr);
    appParam.SetPriority(clientPtr->tos);
    appParam.SetSessionId(clientPtr->sessionId);
    appParam.SetMsgSize(clientPtr->itemSize);
    appParam.m_TotalMsgSize = clientPtr->itemSize;
    appParam.m_fragEnabled = FALSE;
#endif // ADDON_DB
    clientPtr->sessionStart = node->getNodeTime();
    // send the messages
    // Dynamic address
    // Create and send a UDP msg with header and virtual payload.
    // if the client has not yet acquired a valid address then the
    // application packets should not be generated check whether client
    // and server are in valid address state if this is session start
    // then packets will not be sent if in invalid address state and
    // session will be closed; if the session has already started and
    // address becomes invalid during application session then
    // packets will get generated but will be dropped at network layer
    if (AppEbrClientGetSessionAddressState(node, clientPtr)
                                                        == ADDRESS_FOUND)
    {
#ifdef ADDON_DB
        memcpy(
            &packetSendingInfo->appParam,
            &appParam,
            sizeof(StatsDBAppEventParam));
#else
        memset(
            &packetSendingInfo->appParam,
            NULL,
            sizeof(StatsDBAppEventParam));
#endif
        packetSendingInfo->stats = clientPtr->stats;
        return (TRUE);
    }
    else

    {
        clientPtr->sessionStart = 0;
        clientPtr->sessionIsClosed = TRUE;
    }
    return (FALSE);
}

