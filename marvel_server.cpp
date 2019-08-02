/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include "marvel_server.h"
#include "marvel_exception.h"
#include <unistd.h>
#include "marvel_log.h"
#include "marvel_socket.h"

MARVEL_SERVER::MarvelServer(
        MARVEL_APP app, uint32_t host, uint16_t port)
        :host_(host), port_(port), app_(app) {
}

MARVEL_SERVER::~MarvelServer() {
}

void MARVEL_SERVER::RecvProcess() { //
    int serv_socket;
    int clnt_socket;



    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[PER_TRANS_SIZE + 1]; // One Time Buffer
    int length;
    int recv_bytes = 0;

    try {
        // create a socket
        serv_socket = NewSocket(app_.get_stream());
        // add details
        PackSockaddr(&serv_addr, AF_INET, host_, port_);
        // bind the socket
        BindSocket(app_.get_stream(), serv_socket, &serv_addr);
        // now listen
        ListenSocket(app_.get_stream(), serv_socket, &serv_addr);
    } catch (MARVEL_ERR MarvelException exp) {
        throw exp;
    }

    // now accept and recv
    try {
        RecvMessage(serv_socket, &serv_addr);
    } catch (MARVEL_ERR SocketAcceptFailedException exp) {
        throw exp; // ???
    } catch (MARVEL_ERR MessageRecvFailedException exp) {
        throw exp; // ???
    }

}

void MARVEL_SERVER::RecvMessage(int serv_socket, struct sockaddr_in* serv_addr) {
    int id = INIT_SERVER_ID;
    int clnt_socket;
    socklen_t clnt_addr_size = ADDR_SIZE;
    struct sockaddr_in clnt_addr;
    char message[PER_TRANS_SIZE + 1]; // One Time Buffer
    int length;
    int recv_bytes = 0;
    for(int i = 0; i < MAX_CONNECT_NUM; i++) {
        recv_bytes = 0;
        try {
            clnt_socket = AcceptSocket(GetStream(), serv_socket,
                                       serv_addr, &clnt_addr, &clnt_addr_size);
        } catch (MARVEL_ERR SocketAcceptFailedException exp) {
            throw exp;
        }
        MARVEL_LOG SocketAccepted(GetStream(), serv_addr, &clnt_addr);

        while ((length = recv(clnt_socket, message, PER_TRANS_SIZE, 0)) != 0) {
            if (length < 0) {
                // err::errMsg(err::RECV_FAILED);
            }
            recv_bytes += length;
            MARVEL_LOG RecvMessage(GetStream(), serv_addr, message, length, recv_bytes);
            memset(message, 0, PER_TRANS_SIZE);
        }

        id++;
        close(clnt_socket);
        // shutdown(clnt_socket, SHUT_RDWR);

    }
    close(serv_socket);
    // shutdown(serv_socket, SHUT_RDWR);
}

OFSTREAM& MARVEL_SERVER::GetStream() {
    return app_.get_stream();
}

void MARVEL_SERVER::shutdown() {

}

void MARVEL_SERVER::start() {
    try {
        RecvProcess();
    } catch (MARVEL_ERR MarvelException exp) {
        app_.HandleException(exp);
    }
}

