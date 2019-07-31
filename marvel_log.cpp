/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include <fstream>
#include <netinet/in.h>
#include "marvel_log.h"

clock_t base;

using namespace marvel;

void log::client::SendMessage(MarvelClient* client, struct sockaddr_in* sockaddr,
                              char *msg, int send_bytes, int total_bytes) {
    std::ofstream stream = client -> GetStream();
    stream << "[" << clock() - base << "]\n";
    stream << "Socket Host: " << sockaddr -> sin_addr.s_addr
           << "\tPort: " << sockaddr -> sin_port << "\n";
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

void log::server::RecvMessage(MarvelServer *server, struct sockaddr_in* sockaddr,
                              char *msg, int recv_bytes, int total_bytes) {
    std::ofstream stream = server -> GetStream();
    stream << "[" << clock() - base << "]\n";
    stream << "Socket Host: " << sockaddr -> sin_addr.s_addr
           << "\tPort: " << sockaddr -> sin_port << "\n";
    stream << "Message Recv:\t" << msg << "\n";
    stream << "Sent Bytes: " << recv_bytes << "\tTotal Bytes: " << total_bytes << "\n";
}

void log::server::SocketAccepted(MarvelServer* server, struct sockaddr_in* sockaddr,
                                 struct sockaddr_in* clntaddr) {
    std::ofstream stream = server -> GetStream();
    stream << "[" << clock() - base << "]\n";
    stream << "Socket Host: " << sockaddr -> sin_addr.s_addr
           << "\tPort: " << sockaddr -> sin_port << "\n";
    stream << "Socket Accepted" << "\n";
    stream << "Socket Host: " << clntaddr -> sin_addr.s_addr
           << "\tPort: " << clntaddr -> sin_port << "\n";
}