/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include <cstring>
#include <thread>
#include "api_app.h"

#include "marvel_exception.h"
#include "codec/header.h"

MARVEL_CLIENT* MARVEL_API LogInClient(MARVEL_APP* app, uint16_t port) {
    MARVEL_CLIENT* client = new MARVEL_CLIENT(app, app -> get_host(), port);
    // client.start();
    return client;
}

MARVEL_SERVER* MARVEL_API LogInServer(MARVEL_APP* app, uint16_t port) {
    MARVEL_SERVER* server = new MARVEL_SERVER(app, app -> get_host(), port);
    // std::thread t(&MARVEL_SERVER::start, server);
    return server;
}

void MARVEL_API LogOut(MARVEL_CLIENT client) {
    client.shutdown();
    // memset(client, 0, CLIENT_SIZE);
}

void MARVEL_API LogOut(MARVEL_SERVER server) {
    server.shutdown();
    // memset(server, 0, CLIENT_SIZE);
}

ssize_t MARVEL_API SendMessageToServer(MARVEL_CLIENT client,
                             Address host, uint16_t port, char* msg) {
    ssize_t send_bytes = 0;
    try {
        send_bytes = client.SendProcess(host.host, port, msg, kDefaultPacketSize);
    } catch (MARVEL_ERR MarvelException exp) {
        throw exp;
    }
    return send_bytes;
}

ssize_t MARVEL_API RecvMessageFromServer(MARVEL_SERVER server,
                               Address* host, uint16_t* port, char* msg) {
    // client -> app_ -> print();
    ssize_t recv_bytes = 0;
    try {
        recv_bytes = server.RecvProcess(msg, host, port);
    } catch (MARVEL_ERR MarvelException exp){
        throw exp;
    }
    return recv_bytes;
}