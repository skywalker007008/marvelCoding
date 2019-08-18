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
#include "../marvel_constant.h"

static struct sockaddr_in broadcast_addr;

EbrHeader* NewEbrHeader(char type, char range, char code_type, char code_number,
                        short pac_sum, short str_num, short pac_num,
                        Address source_addr, Address dest_addr, short source_port, short dest_port,
                        int length, char* check, char* payload, GFType* coef) {
    EbrHeader* header = (EbrHeader*)malloc(sizeof(EbrHeader));
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
    header -> check = check;
    header -> payload = (char*)malloc(length * sizeof(char));
    memcpy(header -> payload, payload, length);
    header -> coef = coef;
    return header;
}

void init_broadcast_addr() {
    broadcast_addr.sin_port = kDefaultPort;
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_addr.s_addr = inet_addr("255.255.255.255");
}

EbrHeader* CopyEbrHeader(EbrHeader* header) {
    EbrHeader* header_new = (EbrHeader*)malloc(sizeof(EbrHeader));
    header_new -> type = header -> type;
    header_new -> range = header -> range;
    header_new -> codetype = header -> codetype;
    header_new -> codenumber = header -> codenumber;
    header_new -> pacsum = header -> pacsum;
    header_new -> strnum = header -> strnum;
    header_new -> pacnum = header -> pacnum;
    header_new -> sourceaddr = header -> sourceaddr;
    header_new -> destaddr = header -> destaddr;
    header_new -> sourceport = header -> sourceport;
    header_new -> destport = header -> destport;
    header_new -> length = header -> length;
    header_new -> check = header -> check;
    header -> payload = (char*)malloc(header_new -> length * sizeof(char));
    header -> coef = (GFType*)malloc(header_new -> pacsum * sizeof(GFType));
    return header_new;
}
