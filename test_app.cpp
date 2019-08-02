/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include "test_app.h"
#include "api_app.h"
#include "marvel_log.h"

App::App(uint32_t host, const STRING& name, OFSTREAM& stream)
        : host_ (host), name_(name), stream_(stream) {
    STRING file_name = "./src/" + name + ".txt";
    client_ = MARVEL_API LogInClient(this);
    server_ = MARVEL_API LogInServer(this);
    // pool.commit(start);
    pool.commit(MARVEL StartClient, client_);
    pool.commit(MARVEL StartServer, server_);
    // std::thread t1(MARVEL StartClient, client_);
    // t1.join();
    // std::thread t2(MARVEL StartServer, server_);
    // t2.join();
}

/* App::~App() {
    marvel::api::LogOut(&client_);
    marvel::api::LogOut(&server_);
    stream_.close();
}
 */

void App::HandleException(MARVEL_EXCEPTION err) {
    log(err.print());
}

void App::SendMessage(uint32_t dest_host, uint16_t dest_port, const char *msg) {
    int send_bytes;
    try {
        send_bytes = MARVEL_API SendMessageToServer(*client_,dest_host, dest_port, msg);
    } catch (MARVEL_EXCEPTION exp) {
        HandleException(exp);
    }

    log("MessageSent!\t" + TO_STRING(send_bytes) + "bytes");
}

void App::log(STRING log_msg) {
    stream_ << "[" << MARVEL_LOG pass_time() << "]\n";
    stream_ << log_msg << std::endl;
}

OFSTREAM& App::get_stream() {
    return stream_;
}

uint32_t App::get_host() {
    return host_;
}
uint16_t App::get_port() {
    return 60;
}

void App::shutdown() {
    MARVEL_API LogOut(*client_);
    MARVEL_API LogOut(*server_);
    stream_.close();
}

void App::start() {

}

static void MARVEL StartClient(MARVEL_CLIENT* client) {
    client->start();
}

static void MARVEL StartServer(MARVEL_SERVER* server) {
    server->start();
}

