/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include "test_app.h"
#include "../src/api/api_app.h"
#include "../src/include/marvel_log.h"
#include "../src/exception/marvel_exception.h"
#include "../src/codec/header.h"
#include <unistd.h>

App::App(uint32_t host, const STRING& name)
        : host_ (host), name_(name) {
    STRING file_name = "./log/" + name + ".txt";
    stream_ = new OFSTREAM(file_name, std::ios::out);
    client_ = MARVEL_API LogInClient(this, kDefaultPort);
    server_ = MARVEL_API LogInServer(this, kDefaultPort);
    // pool.commit(start);
    // pool.commit(MARVEL StartClient, client_);
    // pool.commit(MARVEL StartServer, server_);
    // std::thread t1(MARVEL StartClient, client_);
    // t1.join();
    // std::thread t2(MARVEL StartServer, server_);
    // t2.join();
#ifdef MARVELCODING_QUEUE_H
    TAILQ_INIT(&client_cache_list_);
    cache_num_++;
#endif
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
        send_bytes = MARVEL_API SendMessageToServer(client_, host, dest_port, msg);
    } catch (MARVEL_EXCEPTION exp) {
        HandleException(exp);
    }

    log("MessageSent!\t" + TO_STRING(send_bytes) + "bytes");
    log_char(msg);
    return send_bytes;
}

ssize_t App::RecvMessage(char* msg, uint32_t* host, uint16_t* port) {
    ssize_t recv_bytes;
    Address temp_host;

    try {
        recv_bytes = MARVEL_API RecvMessageFromServer(server_, &temp_host, port, msg);
    } catch (MARVEL_EXCEPTION exp) {
        HandleException(exp);
    }

    log("MessageRecv!\t" + TO_STRING(recv_bytes) + "bytes");
    log("Host: " + TO_STRING(temp_host.host) + "\tPort: " + TO_STRING(*port));
    log("Message:");
    log_char(msg);
    *host = temp_host.host;
    return recv_bytes;
}

void App::RecvMessageDebug(char* msg, EbrHeaderMsg* header_msg) {
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

void App::log_char(char* log_msg) {
    *stream_ << "[" << MARVEL_LOG pass_time() << "]\n";
    for (int i = 0; i < strlen(log_msg) / 32; i++) {
        for (int j = 0; j < 32; j++) {
            *stream_ << (short)log_msg[i * 32 + j] << " ";
        }
        *stream_ << "\n";
    }
    *stream_ << "\n";
}

OFSTREAM* App::get_stream() {
    return stream_;
}

uint32_t App::get_host() {
    return host_;
}
uint16_t App::get_port() {
    return kDefaultPort;
}

void App::shutdown() {
    MARVEL_API LogOut(*client_);
    MARVEL_API LogOut(*server_);
    stream_ -> close();
}

void App::start() {

}

void App::AddCache(ClientCacheHeaderMsg* header_msg) {
    if (cache_num_ >= 32) {
        throw new MARVEL_ERR AppCacheFullException();
    }
    TAILQ_INSERT_TAIL(&client_cache_list_, header_msg, cache_link);
    std::thread t(&App::RemoveCache, this);
}

void App::FindCache(EbrResendMsg* request, ClientCacheHeaderMsg* header) {
    ClientCacheHeaderMsg* header_msg;
    TAILQ_FOREACH(header_msg, &client_cache_list_, cache_link) {
        if (MatchCacheHeader(header_msg, request)) {
            CopyCacheHeaderMsg(header_msg, header);
            return;
        }
    }
    free(header);
    header = nullptr;
}

void App::RemoveCache() {
    // MACRO
    // Time Exceed then remove this cache
    sleep(3000);
    ClientCacheHeaderMsg* header = TAILQ_FIRST(&client_cache_list_);
    TAILQ_FIRST(&client_cache_list_) = TAILQ_NEXT(header, cache_link);
    TAILQ_REMOVE(&client_cache_list_, header, cache_link);
}

void App::SendResendRequest(EbrResendMsg* msg) {
    client_ -> SendResendRequest(msg);
}

static void MARVEL StartClient(MARVEL_CLIENT* client) {
    client->start();
}

static void MARVEL StartServer(MARVEL_SERVER* server) {
    server->start();
}

