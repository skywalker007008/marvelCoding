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

constexpr int INIT_SERVER_ID = 1;


constexpr int ADDRIN_SIZE = sizeof(struct sockaddr_in);
constexpr int ADDR_SIZE = sizeof(struct sockaddr);

namespace marvel {
    constexpr int MAX_BUF_SIZE = 1024;
    constexpr int PER_TRANS_SIZE = 32;
    constexpr int MAX_CONNECT_NUM = 2;
    constexpr int MAX_RETRY_TIME = 2;
}

#endif //MARVELCODING_MARVELCONSTANT_H


