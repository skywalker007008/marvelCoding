/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include "marvel_exception.h"

MARVEL_ERR MarvelException::MarvelException() {}

MARVEL_ERR SocketCreateFailedException::SocketCreateFailedException() {}

STRING MARVEL_ERR SocketCreateFailedException::print() {
    return "Socket Create Failed!";
}

MARVEL_ERR SocketConnectFailedException::SocketConnectFailedException
        (struct sockaddr_in sockaddr) : host_(sockaddr.sin_addr.s_addr), port_(sockaddr.sin_port) {}

STRING MARVEL_ERR SocketConnectFailedException::print() {
    return ("Socket Connect Failed!\nHost: " + TO_STRING(host_)
            + "Port: " + TO_STRING(port_));
}

MARVEL_ERR SocketBindFailedException::SocketBindFailedException
        (struct sockaddr_in sockaddr) : host_(sockaddr.sin_addr.s_addr), port_(sockaddr.sin_port) {
}

STRING MARVEL_ERR SocketBindFailedException::print() {
    return ("Socket Bind Failed!\nHost: " + TO_STRING(host_)
            + "Port: " + TO_STRING(port_));
}

MARVEL_ERR SocketAcceptFailedException::SocketAcceptFailedException
        (struct sockaddr_in sockaddr) : host_(sockaddr.sin_addr.s_addr), port_(sockaddr.sin_port) {
}

STRING MARVEL_ERR SocketAcceptFailedException::print() {
    return ("Socket Accept Failed!\nHost: " + TO_STRING(host_)
            + "Port: " + TO_STRING(port_));
}

MARVEL_ERR SocketListenFailedException::SocketListenFailedException
        (struct sockaddr_in sockaddr) : host_(sockaddr.sin_addr.s_addr), port_(sockaddr.sin_port) {
}

STRING MARVEL_ERR SocketListenFailedException::print() {
    return ("Socket Accept Failed!\nHost: " + TO_STRING(host_)
            + "Port: " + TO_STRING(port_));
}

MARVEL_ERR MessageOversizedException::MessageOversizedException() {}

STRING MARVEL_ERR MessageOversizedException::print() {
    return "MessageOversized!";
}

MARVEL_ERR MessageSendFailedException::MessageSendFailedException
        (int pos, int size) : pos_(pos), size_(size) {}

STRING MARVEL_ERR MessageSendFailedException::print() {
    return ("Message Send Failed!\nFailed Position: " + TO_STRING(pos_)
           + " Failed Size: " + TO_STRING(size_));
}

MARVEL_ERR MessageRecvFailedException::MessageRecvFailedException
        (int pos, int size) : pos_(pos), size_(size) {}

STRING MARVEL_ERR MessageRecvFailedException::print() {
    return ("Message Recv Failed!\nFailed Position: " + TO_STRING(pos_)
            + " Failed Size: " + TO_STRING(size_));
}

MARVEL_ERR AppCacheFullException::AppCacheFullException() {}

STRING MARVEL_ERR AppCacheFullException::print() {
    return "Cache Full!\n";
}





