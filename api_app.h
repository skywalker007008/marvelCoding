/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#ifndef MARVELCODING_API_APP_H
#define MARVELCODING_API_APP_H

#include "marvel_client.h"
#include "marvel_server.h"

namespace marvel::api {
    // Log app to client & server
    template <typename APP> marvel::MarvelClient LogInClient(APP* app);
    template <typename APP> marvel::MarvelServer LogInServer(APP* app);
    // app to log out
    void LogOut(marvel::MarvelClient* client);
    void LogOut(marvel::MarvelServer* client);
    // For a client App to send Message
    int SendMessageToServer(marvel::MarvelClient* client,
                            uint32_t host, uint16_t port, const char* msg);
    // For a server to inform App of message received.
    void RecvMessageFromClient(marvel::MarvelClient* client,
                              uint32_t host, uint16_t port, const char* msg);
}

#endif //MARVELCODING_API_APP_H
