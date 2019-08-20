#include <arpa/inet.h>
#include "test_app.h"
#include "api_app.h"

/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

void test_local() {
    App server(htonl(INADDR_ANY), "SERVER");
    App client(htonl(inet_addr("192.168,0.1")), "CLIENT");
    char* msg = (char*)malloc(MARVEL kMaxMsgLength * sizeof(char));
    for (int i = 0; i < MARVEL kMaxMsgLength; i++) {
        msg[i] = rand() % 256;
    }
    uint32_t host;
    uint16_t port;
    // std::thread t1(MARVEL StartClient, client_);
    // t1.join();
    // std::thread t2(MARVEL StartServer, server_);
    // t2.join();

    std::thread t1(&App::SendMessage, &client, htonl(INADDR_ANY), kDefaultPort, msg);
    t1.join();
    printf("Thread1 start.\n");
    std::thread t2(&App::RecvMessage, &server, msg, &host, &port);
    t2.join();
    printf("Thread1 start.\n");

    while (true) {}
}

int main() {
    test_local();
    return 0;
}