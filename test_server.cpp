/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include <arpa/inet.h>
#include "test_app.h"
#include "api_app.h"

int main(int argc, char* argv[]) {
    RLNC init(8);
    init_addr();
    App server(inet_addr(argv[1]), argv[2]);
    char* msg = (char*)malloc(MARVEL kMaxMsgLength * sizeof(char));
    char a;

    uint32_t host;
    uint16_t port;

    // server.RecvMessage(msg, &host, &port);

    // std::thread t2(&App::RecvMessage, &server, msg, &host, &port);

    while (true) {
        server.RecvMessage(msg, &host, &port);
        std::cout << "Input a:";
        std::cin >> a;
        if (a == 'E') {
            break;
        }
    }
    return 0;
}