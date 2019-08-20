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
        MARVEL_CLIENT* LogInClient(MARVEL_APP *app, uint16_t port);

        MARVEL_SERVER* LogInServer(MARVEL_APP *app, uint16_t port);

        // app to log out
        void LogOut(MARVEL_CLIENT client);

        void LogOut(MARVEL_SERVER server);

        // For a client App to send Message
        ssize_t SendMessageToServer(MARVEL_CLIENT client,
                                Address host, uint16_t port, char *msg);

        // For a server to inform App of message received.
        ssize_t RecvMessageFromServer(MARVEL_SERVER server,
                                                 Address* host, uint16_t* port, char* msg);
    }
}

#endif //MARVELCODING_API_APP_H
