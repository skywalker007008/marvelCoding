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

namespace marvel {
    namespace api {
        // Log app to client & server
        MARVEL_CLIENT LogInClient(MARVEL_APP *app);

        MARVEL_SERVER LogInServer(MARVEL_APP *app);

        // app to log out
        void LogOut(MARVEL_CLIENT client);

        void LogOut(MARVEL_SERVER server);

        // For a client App to send Message
        int SendMessageToServer(MARVEL_CLIENT client,
                                uint32_t host, uint16_t port, const char *msg);

        // For a server to inform App of message received.
        void RecvMessageFromClient(MARVEL_CLIENT client,
                                   uint32_t host, uint16_t port, const char *msg);
    }
}

#endif //MARVELCODING_API_APP_H
