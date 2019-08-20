/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#ifndef MARVELCODING_MARVELCONSTANT_H
#define MARVELCODING_MARVELCONSTANT_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include "threadpool.h"

#define MARVEL marvel::
#define MARVEL_LOG marvel::log::
#define MARVEL_ERR marvel::err::
#define MARVEL_API marvel::api::

#define MARVEL_CLIENT marvel::MarvelClient
#define MARVEL_SERVER marvel::MarvelServer
#define MARVEL_APP marvel::MarvelApp
#define MARVEL_EXCEPTION marvel::err::MarvelException

#define STRING std::string
#define OFSTREAM std::ofstream
#define POOL std::threadpool

#define DEBUG debug::

// #define TO_STRING(x) std::to_string(x)
#define TO_STRING std::to_string

constexpr int INIT_SERVER_ID = 1;


#define ADDRIN_SIZE sizeof(struct sockaddr_in)
#define ADDR_SIZE sizeof(struct sockaddr)
#define ROUND(a,b) (((a) % (b) == 0) ? (a) : ((a) + (b) - ((a) % (b))))
constexpr int MAX_POOL_NUM = 40;
constexpr int kDefaultPacketSize = 256 * sizeof(char);

static POOL pool{MAX_POOL_NUM};

namespace marvel {
    constexpr int PER_TRANS_SIZE = 32;
    constexpr int MAX_CONNECT_NUM = 2;
    constexpr int MAX_RETRY_TIME = 2;
    constexpr int kMaxPacketLength = 256;
    constexpr int kMaxCacheSize = 16;
    constexpr int kMaxMsgLength = 512;


}



#endif //MARVELCODING_MARVELCONSTANT_H


