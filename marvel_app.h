/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#ifndef MARVELCODING_MARVEL_APP_H
#define MARVELCODING_MARVEL_APP_H

#include <cstdint>
#include "marvel_constant.h"
#include "marvel_exception.h"
#include "marvel_client.h"
#include "marvel_server.h"

#include "codec/header.h"

namespace marvel {
     class MarvelApp {
     public:
        // MarvelApp();
        // virtual ~MarvelApp();
        virtual ssize_t SendMessage(uint32_t dest_host, uint16_t dest_port, char* msg) {}
        // Handle the message
        virtual void HandleException(MARVEL_EXCEPTION err) {}
        // close this APP
        virtual void shutdown() {}
        //
        virtual OFSTREAM* get_stream() {}
        virtual uint32_t get_host() {}
        virtual uint16_t get_port() {}
         virtual void log(STRING log_msg) {}
         virtual void RecvMessage(char* msg, EbrHeaderMsg* header_msg) {}
    };
}
#endif //MARVELCODING_MARVEL_APP_H
