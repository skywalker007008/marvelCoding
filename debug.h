/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#ifndef MARVELCODING_DEBUG_H
#define MARVELCODING_DEBUG_H

#include "codec/header.h"

void log_send_message(EbrHeaderMsg* msg);

void log_recv_message(EbrHeaderMsg* msg);

void log_recv_final_message(EbrHeaderMsg* header_msg, char* msg, int length);

void log_transfer_message(EbrHeaderMsg* msg);

#endif //MARVELCODING_DEBUG_H
