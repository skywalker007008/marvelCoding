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
    template <typename APP> marvel::MarvelClient LogInClient(APP* app);
    template <typename APP> marvel::MarvelServer LogInServer(APP* app);
    void LogOut(marvel::MarvelClient* client);
    void LogOut(marvel::MarvelServer* client);
    int SendMessageToServer(marvel::MarvelClient* client,
                            uint32_t host, uint16_t port, const char* msg);
    int RecvMessageFromClient(marvel::MarvelClient* client,
                              uint32_t host, uint16_t port, const char* msg);
}

#endif //MARVELCODING_API_APP_H
