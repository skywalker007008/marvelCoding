#include <arpa/inet.h>
#include "test_app.h"
#include "../src/api/api_app.h"

/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

void test_local() {
    RLNC init(8);
    init_addr();
    App server(inet_addr("192.168.0.1"), "SERVER");
    App client(inet_addr("192.168.0.2"), "CLIENT");
    App client2(inet_addr("192.168.0.3"), "CLIENT2");
    char* msg = (char*)malloc(MARVEL kMaxMsgLength * sizeof(char));
    for (int i = 0; i < MARVEL kMaxMsgLength; i++) {
        msg[i] = rand() % 64 + 1;
    }
    char* msg1 = (char*)malloc(MARVEL kMaxMsgLength * sizeof(char));
    uint32_t host;
    uint16_t port;
    // std::thread t1(MARVEL StartClient, client_);
    // t1.join();
    // std::thread t2(MARVEL StartServer, server_);
    // t2.join();

    std::thread t2(&App::RecvMessage, &server, msg1, &host, &port);
    // t2.join();
    printf("Thread2 start.\n");

    std::thread t1(&App::SendMessage, &client, inet_addr("192.168.0.1"), kDefaultPort, msg);
    // t1.join();
    printf("Thread1 start.\n");

    std::thread t3(&App::SendMessage, &client2, inet_addr("192.168.0.1"), kDefaultPort, msg);
    printf("Thread3 start.\n");

    while (true) {
        char a;
        std::cin >> a;
        if (a == 'E') {
            break;
        }
    }

}

int main(int argc, char* argv[]) {
    // test_local();
    RLNC init(8);
    init_addr();
    App client(inet_addr(argv[1]), argv[2]);
    char* msg = (char*)malloc(MARVEL kMaxMsgLength * sizeof(char));
    for (int i = 0; i < MARVEL kMaxMsgLength; i++) {
        msg[i] = rand() % 64 + 1;
    }
    uint32_t host;
    uint16_t port;

    char a;

    while (true) {
        client.SendMessage(inet_addr("192.168.0.1"), kDefaultPort, msg);
        std::cout << "Input a:";
        std::cin >> a;
        if (a == 'E') {
            break;
        }
    }
    return 0;
}

