/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include <fstream>
#include <netinet/in.h>
#include "MarvelLog.h"

clock_t base;

using namespace marvel;

void log::client::SendMessage(MarvelClient* client, uint32_t host, uint16_t port, char *msg,
                              int send_bytes, int total_bytes) {
    std::ofstream stream = client -> GetStream();
    stream << "[" << clock() - base << "]\n";
    stream << "Socket Host: " << host << "\tPort: " << port << "\n";
    stream << "Message Sent:\t" << msg << "\n";
    stream << "Sent Bytes: " << send_bytes << "\tTotal Bytes: " << total_bytes << "\n";
}

void log::client::SocketConnected(MarvelClient* client, struct sockaddr_in* sockaddr) {
    std::ofstream stream = client -> GetStream();
    stream << "[" << clock() - base << "]\n";
    stream << "Socket Host: " << sockaddr -> sin_addr.s_addr
           << "\tPort: " << sockaddr -> sin_port << "\n";
    stream << "Socket Connected" << "\n";
}