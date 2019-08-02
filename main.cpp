#include "test_app.h"
#include "marvel_log.h"

constexpr uint32_t kDefaultClientHost = 255;
constexpr uint32_t kDefaultServerHost = 0;
constexpr uint16_t kDefaultPort = 58371;


int main() {
    base = clock();
    OFSTREAM* stream1 = new OFSTREAM("./src/client.txt", std::ios::out);
    *stream1 << "client.txt created\n";
    OFSTREAM* stream2 = new OFSTREAM("./src/server.txt", std::ios::out);
    *stream2 << "server.txt created\n";
    App app_client(kDefaultClientHost, "client", stream1);
    App app_server(kDefaultServerHost, "server", stream2);
    // app_client.start();
    // app_server.start();
    app_client.SendMessage(kDefaultServerHost, kDefaultPort, "Greet to you you idiot!");
    app_client.SendMessage(kDefaultServerHost, kDefaultPort, "I am your father!");
    app_client.shutdown();
    app_server.shutdown();
    std::cout << "Finished Test" << std::endl;
}