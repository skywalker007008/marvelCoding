/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include <arpa/inet.h>
#include "debug.h"
#include "marvel_log.h"

void log_send_message(EbrHeaderMsg* msg) {
    struct in_addr host;
    host.s_addr = (msg ->header).destaddr.host;
    int length = (msg->header).length;
    printf("[%x] Send Message %d bytes\n", MARVEL_LOG pass_time(), length);
    printf("Dest Address: %s  Dest Port: %d\n", inet_ntoa(host), (msg->header).destport);
    printf("StrNum : %x CodeNumber : %x PacSum: %x PacNum : %x\n",
           (msg->header).strnum, (msg->header).codenumber, (msg->header).pacsum, (msg->header).pacnum);
    char* payload = (char*)malloc(length);
    memset(payload, 0, length);
    memcpy(payload, msg->payload, length);
    printf("Message Content:\n");

    for (int i = 0; i < length / 32; i++) {
        for (int j = 0; j < 32; j++) {
            printf("%x ", (uint8_t)payload[i * 32 + j]);
        }
        printf("\n");
    }
    printf("\n");
    free(payload);
}

void log_recv_message(EbrHeaderMsg* msg) {
    struct in_addr host;
    host.s_addr = (msg ->header).sourceaddr.host;
    int length = (msg->header).length;
    printf("[%x] Recv Message %d bytes\n", MARVEL_LOG pass_time(), length);
    printf("Source Address: %s  Source Port: %d\n", inet_ntoa(host), (msg->header).sourceport);
    printf("StrNum : %x CodeNumber : %x PacSum: %x PacNum : %x\n",
           (msg->header).strnum, (msg->header).codenumber, (msg->header).pacsum, (msg->header).pacnum);
    char* payload = (char*)malloc(length);
    memset(payload, 0, length);
    memcpy(payload, msg->payload, length);
    printf("Message Content:\n");

    for (int i = 0; i < length / 32; i++) {
        for (int j = 0; j < 32; j++) {
            printf("%x ", (uint8_t)payload[i * 32 + j]);
        }
        printf("\n");
    }
    printf("\n");
    free(payload);
}

void log_send_init_message(uint32_t host, uint16_t port, char* msg, int length, uint8_t strnum) {
    struct in_addr host_;
    host_.s_addr = host;
    printf("[%x] Send Init Message %d bytes\n", MARVEL_LOG pass_time(), length);
    printf("Dest Address: %s  Dest Port: %d\n", inet_ntoa(host_), port);
    printf("StrNum : %x\n", strnum);
    char* payload = (char*)malloc(length);

    memcpy(payload, msg, length);
    printf("Message Content:\n");

    for (int i = 0; i < length / 32; i++) {
        for (int j = 0; j < 32; j++) {
            printf("%x ", (uint8_t)payload[i * 32 + j]);
        }
        printf("\n");
    }
    printf("\n");
    free(payload);
}

void log_recv_final_message(EbrHeaderMsg* header_msg, char* msg, int length) {
    struct in_addr host;
    host.s_addr = (header_msg ->header).sourceaddr.host;
    printf("[%x] Recv Final Message %d bytes\n", MARVEL_LOG pass_time(), length);
    printf("Source Address: %s  Source Port: %d\n", inet_ntoa(host), (header_msg->header).sourceport);
    printf("StrNum : %x\n", (header_msg->header).strnum);
    char* payload = (char*)malloc(length);

    memcpy(payload, msg, length);
    printf("Message Content:\n");

    for (int i = 0; i < length / 32; i++) {
        for (int j = 0; j < 32; j++) {
            printf("%x ", (uint8_t)payload[i * 32 + j]);
        }
        printf("\n");
    }
    printf("\n");
    free(payload);
}

void log_transfer_message(EbrHeaderMsg* msg) {
    struct in_addr host;
    int length = (msg->header).length;
    printf("[%x] Transfer Message %d bytes\n", MARVEL_LOG pass_time(), length);
    host.s_addr = (msg ->header).sourceaddr.host;
    printf("Source Address: %s  Source Port: %d\n", inet_ntoa(host), (msg->header).sourceport);
    host.s_addr = (msg ->header).destaddr.host;
    printf("Dest Address: %s  Dest Port: %d\n", inet_ntoa(host), (msg->header).destport);
    printf("StrNum : %x CodeNumber : %x PacSum: %x PacNum : %x\n",
           (msg->header).strnum, (msg->header).codenumber, (msg->header).pacsum, (msg->header).pacnum);
    char* payload = (char*)malloc(length);
    memset(payload, 0, length);
    memcpy(payload, msg->payload, length);
    printf("Message Content:\n");

    for (int i = 0; i < length / 32; i++) {
        for (int j = 0; j < 32; j++) {
            printf("%x ", (uint8_t)payload[i * 32 + j]);
        }
        printf("\n");
    }
    printf("\n");
    free(payload);
}