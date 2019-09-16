/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 *
 * This file defines the ops and attrs of the Client
 */

#ifndef MARVELCODING_MARVELCLIENT_H
#define MARVELCODING_MARVELCLIENT_H
#include <sys/socket.h>
#include <cstdint> // useless
#include <string>
#include <fstream>
#include "../include/marvel_constant.h"
#include "marvel_app.h"
#include "../codec/codec.h"
#include "../codec/header.h"

// constexpr int CLIENT_SIZE = sizeof(marvel::MarvelClient);

namespace marvel {
    class MarvelApp;
    class MarvelClient {
    public:

        /*!
         * Constructor default
         * @param app link to which app
         * @param host the host of this client
         * @param port the port of this client
         */
        MarvelClient(MarvelApp* app, uint32_t host, uint16_t port);

        /*!
         * Default deconstrutor
         */
        ~MarvelClient();

        /*!
         * Send a message through certain sock
         * @param sock sock to send message
         * @param msg char* format
         * @param serv_addr where to send
         * @return Total send bytes
         */
        ssize_t sendMessage(int sock, EbrHeaderMsg* header_msg);

        /*!
         * API function for APP to send message
         * @param host which host to send
         * @param port which port to send
         * @param num the No. of the message
         * @param msg char* format
         * @return Total send bytes
         */
        ssize_t SendProcess(uint32_t host, uint16_t port, char* msg, int packet_size);

        /*!
         * Init a default codec library
         */
        void BuildCodec();

        /*!
         * Get the output stream ptr
         * @return stream ptr
         */
        OFSTREAM* GetStream();

        /*!
         * Shutdown this client
         */
        void shutdown();

        /*!
         * Start the client
         */
        void start();

        void AddCache(char* msg, GFType** coef, uint8_t strnum, Address dest_addr,
                      uint16_t dest_port, uint16_t packet_size, uint8_t packet_sum);

        void SendResendRequest(EbrResendMsg* msg);

        void SendResendRequestThread(EbrResendMsg* msg);

        void SendResendMsg(EbrHeaderMsg* msg);

        void SendResendMsgThread(EbrHeaderMsg* msg);

        void AddCache(ClientCacheHeaderMsg* header_msg);
        void FindCache(EbrResendMsg* request, ClientCacheHeaderMsg* header);
        void RemoveCache();

    private:
        /*! ptr of the codec_lib */
        CODEC* codec_;
        /*! app linked to */
        MarvelApp* app_;
        /*! host of this client */
        uint32_t host_;
        Address addr_;
        /*! port of this client */
        uint16_t port_;
        uint8_t id_;

        pthread_mutex_t mutex_remove_;
        pthread_mutex_t mutex_resend_;
        pthread_mutex_t mutex_request_;

#ifdef MARVELCODING_QUEUE_H
        struct ClientCacheList client_cache_list_;
        int cache_num_;
#endif
    };

    static void StartClient(MarvelClient* client);


}

#endif //MARVELCODING_MARVELCLIENT_H