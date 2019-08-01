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

template <typename APP> MARVEL_CLIENT MARVEL_API LogInClient(APP* app) {
    MARVEL_CLIENT<APP> client(app, app -> get_host(), app -> get_port());
    client.start();
    return client;
}

template <typename APP> MARVEL_SERVER MARVEL_API LogInServer(APP* app) {
    MARVEL_SERVER<APP> server(app, app -> get_host(), app -> get_port());
    std::thread t(server.start());
    return server;
}

void MARVEL_API LogOut(MARVEL_CLIENT* client) {
    client -> shutdown();
    memset(client, 0, CLIENT_SIZE);
}

void MARVEL_API LogOut(MARVEL_SERVER* server) {
    server -> shutdown();
    memset(server, 0, CLIENT_SIZE);
}

int MARVEL_API SendMessageToServer(MARVEL_CLIENT* client,
                             uint32_t host, uint16_t port, const char* msg) {
    int send_bytes = 0;
    try {
        send_bytes = client -> SendProcess(host, port, msg);
    } catch (MARVEL_ERR MarvelException exp) {
        throw exp;
    }
    return send_bytes;
}

void MARVEL_API RecvMessageFromClient(MARVEL_CLIENT* client,
                               uint32_t host, uint16_t port, const char* msg) {
    // client -> app_ -> print();
}