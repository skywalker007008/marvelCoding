/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include "marvel_exception.h"
#include "marvel_constant.h"

MARVEL_ERR SocketCreateFailedException() {}

STRING MARVEL_ERR SocketCreateFailedException::print() {
    return "Socket Create Failed!";
}

MARVEL_ERR SocketConnectFailedException(struct sockaddr_in sockaddr)
        : host_(sockaddr.sin_addr.s_addr), port_(sockaddr.sin_port) {}

STRING MARVEL_ERR SocketConnectFailedException::print() {
    return ("Socket Connect Failed!\nHost: " + TO_STRING(host_)
            + "Port: " + TO_STRING(port_));
}

MARVEL_ERR SocketBindFailedException(struct sockaddr_in sockaddr)
        : host_(sockaddr.sin_addr.s_addr), port_(sockaddr.sin_port) {
}

STRING MARVEL_ERR SocketBindFailedException::print() {
    return ("Socket Bind Failed!\nHost: " + TO_STRING(host_)
            + "Port: " + TO_STRING(port_));
}

MARVEL_ERR SocketAcceptFailedException(struct sockaddr_in sockaddr)
        : host_(sockaddr.sin_addr.s_addr), port_(sockaddr.sin_port) {
}

STRING MARVEL_ERR SocketAcceptFailedException::print() {
    return ("Socket Accept Failed!\nHost: " + TO_STRING(host_)
            + "Port: " + TO_STRING(port_));
}

MARVEL_ERR SocketListenFailedException(struct sockaddr_in sockaddr)
        : host_(sockaddr.sin_addr.s_addr), port_(sockaddr.sin_port) {
}

STRING MARVEL_ERR SocketListenFailedException::print() {
    return ("Socket Accept Failed!\nHost: " + TO_STRING(host_)
            + "Port: " + TO_STRING(port_));
}

MARVEL_ERR MessageSendFailedException(int pos, int size)
        : pos_(pos), size_(size) {}

STRING MARVEL_ERR MessageSendFailedException::print() {
    return ("Message Send Failed!\nFailed Position: " + TO_STRING(pos_)
           + " Failed Size: " + TO_STRING(size_));
}

MARVEL_ERR MessageRecvFailedException(int pos, int size)
        : pos_(pos), size_(size) {}

STRING MARVEL_ERR MessageRecvFailedException::print() {
    return ("Message Recv Failed!\nFailed Position: " + TO_STRING(pos_)
            + " Failed Size: " + TO_STRING(size_));
}





