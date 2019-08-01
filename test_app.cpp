/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include "test_app.h"
#include "api_app.h"
#include "marvel_log.h"

using namespace std;

App::App(uint32_t host, const string &name)
        : host_ (host), name_(name){
    string file_name = name + ".txt";
    stream_.open(file_name, ios::out);
    client_ = marvel::api::LogInClient(this);
    server_ = marvel::api::LogInServer(this);
}

/* App::~App() {
    marvel::api::LogOut(&client_);
    marvel::api::LogOut(&server_);
    stream_.close();
}
 */

template <class ERR> void App::HandleException(ERR err) {
    log(err.print());
}

template <class ERR> void App::SendMessage(uint32_t dest_host, uint16_t dest_port, const char *msg) {
    int send_bytes;
    try {
        send_bytes = marvel::api::SendMessageToServer(client_,dest_host, dest_port, msg);
    } catch (ERR exp) {
        HandleException(exp);
    }

    log("MessageSent!\t" + to_string(send_bytes) + "bytes");
}

void App::log(std::string log_msg) {
    stream_ << "[" << marvel::log::pass_time() << "]\n";
    stream_ << log_msg << endl;
}

ofstream App::get_stream() {
    return stream_;
}

uint32_t App::get_host() {
    return host_;
}
uint16_t App::get_port() {
    return 60;
}

void App::shutdown() {
    marvel::api::LogOut(&client_);
    marvel::api::LogOut(&server_);
    stream_.close();
}

