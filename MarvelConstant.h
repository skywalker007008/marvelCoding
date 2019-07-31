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

constexpr std::ofstream err_file("error.txt", std::ios::out);
constexpr std::ofstream log_file("log.txt", std::ios::out);
constexpr std::ofstream server_file("server.txt", std::ios::out);
constexpr std::ofstream client_file("client.txt", std::ios::out);

namespace marvel {
    constexpr int MAX_BUF_SIZE = 1024;
    constexpr int PER_TRANS_SIZE = 32;
}

#endif //MARVELCODING_MARVELCONSTANT_H


