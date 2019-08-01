/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include <cstring>
#include "api_app.h"
#include "marvel_exception.h"

using namespace marvel;

template <typename APP> MarvelClient api::LogInClient(APP* app) {
    MarvelClient<APP> client(app, app -> GetHost(), app -> GetPort());
    client.start();
    return client;
}

template <typename APP> MarvelServer api::LogInServer(APP* app) {
    MarvelServer<APP> server(app, app -> GetHost(), app -> GetPort());
    server.start();
    return server;
}
void api::LogOut(marvel::MarvelClient* client) {
    client -> shutdown();
    memset(client, 0, CLIENT_SIZE);
}
void api::LogOut(marvel::MarvelServer* server) {
    server -> shutdown();
    memset(server, 0, CLIENT_SIZE);
}
int api::SendMessageToServer(marvel::MarvelClient* client,
                             uint32_t host, uint16_t port, const char* msg) {
    int send_bytes = 0;
    try {
        send_bytes = client -> start(host, port, msg);
    } catch (err::MarvelException exp) {
        throw exp;
    }
    return send_bytes;
}
void api::RecvMessageFromClient(marvel::MarvelClient* client,
                               uint32_t host, uint16_t port, const char* msg) {
    client -> app -> print();
}