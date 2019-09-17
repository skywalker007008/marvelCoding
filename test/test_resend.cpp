/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include <arpa/inet.h>
#include "test_app.h"
#include "../src/api/api_app.h"

int main(int argc, char* argv[]) {
    // test_local();
    RLNC init(8);
    init_addr();
    App app(inet_addr(argv[1]), argv[2]);
    char* msg = (char*)malloc(MARVEL kMaxMsgLength * sizeof(char));
    for (int i = 0; i < MARVEL kMaxMsgLength; i++) {
        msg[i] = rand() % 64 + 1;
    }

    char a;

    while (true) {
        if (memcmp(argv[3], "send", 4) == 0) {
            app.SendMessage(inet_addr("192.168.0.1"), kDefaultPort, msg);
            std::cout << "Input a:";
            std::cin >> a;
            if (a == 'E') {
                break;
            }
        }
    }
    return 0;
}