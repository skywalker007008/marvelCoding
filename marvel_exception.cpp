/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include "marvel_exception.h"
#include "marvel_constant.h"

using namespace marvel;

err::SocketCreateFailedException() {}

std::string err::SocketCreateFailedException::print() {
    return "Socket Create Failed!";
}

err::SocketConnectFailedException(struct sockaddr_in sockaddr)
        : host_(sockaddr.sin_addr.s_addr), port_(sockaddr.sin_port) {}

std::string err::SocketConnectFailedException::print() {
    return ("Socket Connect Failed!\nHost: " + std::to_string(host_)
            + "Port: " + std::to_string(port_));
}

err::SocketBindFailedException(struct sockaddr_in sockaddr)
        : host_(sockaddr.sin_addr.s_addr), port_(sockaddr.sin_port) {
}

std::string err::SocketBindFailedException::print() {
    return ("Socket Bind Failed!\nHost: " + std::to_string(host_)
            + "Port: " + std::to_string(port_));
}

err::SocketAcceptFailedException(struct sockaddr_in sockaddr)
        : host_(sockaddr.sin_addr.s_addr), port_(sockaddr.sin_port) {
}

std::string err::SocketAcceptFailedException::print() {
    return ("Socket Accept Failed!\nHost: " + std::to_string(host_)
            + "Port: " + std::to_string(port_));
}

err::SocketListenFailedException(struct sockaddr_in sockaddr)
        : host_(sockaddr.sin_addr.s_addr), port_(sockaddr.sin_port) {
}

std::string err::SocketListenFailedException::print() {
    return ("Socket Accept Failed!\nHost: " + std::to_string(host_)
            + "Port: " + std::to_string(port_));
}

err::MessageSendFailedException(int pos, int size)
        : pos_(pos), size_(size) {}

std::string err::MessageSendFailedException::print() {
    return ("Message Send Failed!\nFailed Position: " + std::to_string(pos_)
           + " Failed Size: " + std::to_string(size_));
}

err::MessageRecvFailedException(int pos, int size)
        : pos_(pos), size_(size) {}

std::string err::MessageRecvFailedException::print() {
    return ("Message Recv Failed!\nFailed Position: " + std::to_string(pos_)
            + " Failed Size: " + std::to_string(size_));
}





