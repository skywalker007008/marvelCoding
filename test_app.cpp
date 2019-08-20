/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include "test_app.h"
#include "api_app.h"
#include "marvel_log.h"

App::App(uint32_t host, const STRING& name)
        : host_ (host), name_(name) {
    STRING file_name = "./src/" + name + ".txt";
    stream_ = new OFSTREAM(file_name, std::ios::out);
    client_ = MARVEL_API LogInClient(this, kDefaultPort);
    server_ = MARVEL_API LogInServer(this, kDefaultPort);
    // pool.commit(start);
    // pool.commit(MARVEL StartClient, client_);
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

ssize_t App::SendMessage(uint32_t dest_host, uint16_t dest_port, char *msg) {
    ssize_t send_bytes;
    Address host;
    host.host = dest_host;
    try {
        send_bytes = MARVEL_API SendMessageToServer(*client_, host, dest_port, msg);
    } catch (MARVEL_EXCEPTION exp) {
        HandleException(exp);
    }

    log("MessageSent!\t" + TO_STRING(send_bytes) + "bytes");
    return send_bytes;
}

ssize_t App::RecvMessage(char* msg, uint32_t* host, uint16_t* port) {
    ssize_t recv_bytes;
    Address temp_host;

    try {
        recv_bytes = MARVEL_API RecvMessageFromServer(*server_, &temp_host, port, msg);
    } catch (MARVEL_EXCEPTION exp) {
        HandleException(exp);
    }

    log("MessageRecv!\t" + TO_STRING(recv_bytes) + "bytes");
    log("Host: " + TO_STRING(temp_host.host) + "\tPort: " + TO_STRING(*port));
    log("Message:");
    log(msg);
    *host = temp_host.host;
}

void App::RecvMessage(char* msg, EbrHeaderMsg* header_msg) {
    *stream_ << "Recv Message:\t" << (header_msg -> header).pacsum * (header_msg -> header).length << "bytes\n";
    *stream_ << "Source addr:\t" << (header_msg -> header).sourceaddr.host << "\n";
    *stream_ << "Source port:\t" << (header_msg -> header).sourceport << "\n";
    *stream_ << "Message:\n";
    *stream_ << msg;
}
void App::log(STRING log_msg) {
    *stream_ << "[" << MARVEL_LOG pass_time() << "]\n";
    *stream_ << log_msg << std::endl;
}

void App::log(char* log_msg) {
    *stream_ << "[" << MARVEL_LOG pass_time() << "]\n";
    *stream_ << log_msg << std::endl;
}

OFSTREAM* App::get_stream() {
    return stream_;
}

uint32_t App::get_host() {
    return host_;
}
uint16_t App::get_port() {
    return 58371;
}

void App::shutdown() {
    MARVEL_API LogOut(*client_);
    MARVEL_API LogOut(*server_);
    stream_ -> close();
}

void App::start() {

}

static void MARVEL StartClient(MARVEL_CLIENT* client) {
    client->start();
}

static void MARVEL StartServer(MARVEL_SERVER* server) {
    server->start();
}

