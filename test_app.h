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

template <class Client, class Server> class App {
public:
    App(uint32_t host, const std::string& name);
    // ~App();
    // Send message to a destination
    void SendMessage(uint32_t dest_host, uint16_t dest_port, const char* msg);
    // Handle the message
    template <class ERR> void HandleException(ERR err);
    // close this APP
    void shutdown();
    //
    std::ofstream get_stream();
    uint32_t get_host();
    uint16_t get_port();

private:
    std::string name_;
    std::ofstream stream_;
    uint32_t host_;
    Client client_;
    Server server_;
    void log(std::string log_msg);
};

#endif //MARVELCODING_TEST_APP_H
