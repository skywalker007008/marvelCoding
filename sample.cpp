/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */
#include "test_app.h"
#include "marvel_log.h"

constexpr uint32_t kDefaultClientHost = 255;
constexpr uint32_t kDefaultServerHost = 0;
constexpr uint16_t kDefaultPort = 996;

void sample() {
    base = clock();
    App app_client(kDefaultClientHost, "client");
    App app_server(kDefaultServerHost, "server");
    app_client.SendMessage(kDefaultServerHost, kDefaultPort, "Greet to you you idiot!");
    app_client.SendMessage(kDefaultServerHost, kDefaultPort, "I am your father!");
    app_client.shutdown();
    app_server.shutdown();
    std::cout << "Finished Test" << std::endl;
}