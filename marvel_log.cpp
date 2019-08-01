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

void MARVEL_LOG SendMessage(MARVEL_CLIENT* client, struct sockaddr_in* sockaddr,
                              char *msg, int send_bytes, int total_bytes) {
    OFSTREAM stream = client -> GetStream();
    stream << "[" << pass_time() << "]\n";
    stream << "Socket Host: " << sockaddr -> sin_addr.s_addr
           << "\tPort: " << sockaddr -> sin_port << "\n";
    stream << "Message Sent:\t" << msg << "\n";
    stream << "Sent Bytes: " << send_bytes << "\tTotal Bytes: " << total_bytes << "\n";
}

void MARVEL_LOG SocketConnected(MARVEL_CLIENT* client, struct sockaddr_in* sockaddr) {
    OFSTREAM stream = client -> GetStream();
    stream << "[" << pass_time() << "]\n";
    stream << "Socket Host: " << sockaddr -> sin_addr.s_addr
           << "\tPort: " << sockaddr -> sin_port << "\n";
    stream << "Socket Connected" << "\n";
}

void MARVEL_LOG RecvMessage(MARVEL_SERVER *server, struct sockaddr_in* sockaddr,
                              char *msg, int recv_bytes, int total_bytes) {
    OFSTREAM stream = server -> GetStream();
    stream << "[" << pass_time() << "]\n";
    stream << "Socket Host: " << sockaddr -> sin_addr.s_addr
           << "\tPort: " << sockaddr -> sin_port << "\n";
    stream << "Message Recv:\t" << msg << "\n";
    stream << "Sent Bytes: " << recv_bytes << "\tTotal Bytes: " << total_bytes << "\n";
}

void MARVEL_LOG SocketAccepted(MARVEL_SERVER* server, struct sockaddr_in* sockaddr,
                                 struct sockaddr_in* clntaddr) {
    OFSTREAM stream = server -> GetStream();
    stream << "[" << pass_time() << "]\n";
    stream << "Socket Host: " << sockaddr -> sin_addr.s_addr
           << "\tPort: " << sockaddr -> sin_port << "\n";
    stream << "Socket Accepted" << "\n";
    stream << "Socket Host: " << clntaddr -> sin_addr.s_addr
           << "\tPort: " << clntaddr -> sin_port << "\n";
}

void MARVEL_LOG log(std::ofstream stream, const std::string& msg) {
    stream << "[" << clock() - base << "]\n";
    stream << msg << "\n";
}

inline clock_t MARVEL_LOG pass_time() {
    return clock() - base;
}