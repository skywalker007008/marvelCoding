/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/rlnc-lib
 * 
 */

#ifndef RLNC_CODEC_H
#define RLNC_CODEC_H

#include <malloc.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "gf.h"

#define RLNC rlnc::
#define CODEC rlnc::Codec

extern GFType** std_coef;

namespace rlnc {
    /*!
     * The coding library
     */
    class Codec {
    public:
        /*!
         * Init the codec according to the vector size and packet size
         * @param vec_size the size of the coding number of message unit
         * @param packet_size the size of the message unit
         */
        Codec(int vec_size, int packet_size);

        /*!
         * Default deconstructor: free space
         */
        ~Codec();

        /*!
         * If the cache is full(should be never)
         * @return _is_full
         */
        bool is_full();

        /*!
         * If the cache is enough
         * @return _is_enough
         */
        bool is_enough();

        /*!
         * Recv the message unit and the encoding coef on it
         * Put them into the cache_buf
         * @param msg the message_ptr
         * @param coef the encoding coef of this message
         */
        void RecvMessage(char* msg, GFType* coef);

        /*!
         * Reform the msg and the coef
         * In order to make the trace bigger or equal
         * @return if the trace is full
         */
        bool LinkMsg();

        /*!
         * Encode the cached msg
         * @return the final encoded msg
         */

        GFType** encode();

        /*!
         * Decode the cached msg
         * @requires the cached coef should be full-trace(_is_enough = true)
         */
        void decode();

        /*!
         * Get the encoded message
         * @param buf ptr to receive the encoded_message
         */
        void get_encode_message(char* buf);

        /*!
         * Get the decoded message
         * @param buf ptr to receive the decoded_message
         */
        void get_decode_message(char* buf);

    private:
        /*! The size of the message-unit */
        int _packet_size;
        /*! Symbol whether the cache is full */
        bool _is_full;
        /*! Symbol whether the cache_coef is enough to decode(full-trace) */
        bool _is_enough;
        // int _msg_num;
        /*! Number of the valid msg received */
        int _recv_num;
        /*! Size of the coding number of the msg */
        int _vec_size;
        /*! Ptr where msg is ready to encode or decode */
        char* _raw_msg;
        /*! Ptr that stores the encoded_msg */
        char* _encode_msg;
        /*! Ptr that stores the decoded_msg */
        char* _decode_msg;
        /*! Ptr that stores the cached_msg */
        char* _cache_msg;
        /*! Ptr that stores the used_coef */
        GFType** _coef_mat;
        /*! Ptr that stores the cached_coef */
        GFType** _cache_coef_mat;
    };

    /*! The max size of the buf */
    constexpr int kMaxBufSize = 20;
    /*! The max number of the msg unit */
    constexpr int kMaxPartNum = 16;

    /*!
     * Init the std_coef
     */
    void coef_init();

    void init(unsigned int m);
}

#endif //RLNC_CODEC_H
