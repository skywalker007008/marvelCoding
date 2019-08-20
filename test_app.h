/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#ifndef MARVELCODING_TEST_APP_H
#define MARVELCODING_TEST_APP_H

#include <stdint.h>
#include <string>
#include <fstream>
#include "marvel_exception.h"
#include "marvel_client.h"
#include "marvel_server.h"
#include "marvel_app.h"
#include "threadpool.h"

class App : public MARVEL_APP {
public:
    // inherit from MarvelApp
    App(uint32_t host, const STRING& name);
    // ~App();
    // Send message to a destination
    ssize_t SendMessage(uint32_t dest_host, uint16_t dest_port, char* msg) override;
    // Handle the message
    void HandleException(MARVEL_EXCEPTION err) override;
    // close this APP
    void shutdown() override;
    //
    OFSTREAM* get_stream() override ;
    uint32_t get_host() override;
    uint16_t get_port() override;
    void RecvMessage(char* msg, EbrHeaderMsg* header_msg) override ;
    ssize_t App::RecvMessage(char* msg, uint32_t* host, uint16_t* port);
    void start();
    void log(STRING log_msg);
    void log(char* log_msg);

private:
    STRING name_;
    OFSTREAM* stream_;
    uint32_t host_;
    MARVEL_CLIENT* client_;
    MARVEL_SERVER* server_;
};

#endif //MARVELCODING_TEST_APP_H
