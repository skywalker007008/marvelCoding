/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#ifndef MARVELCODING_CODEC_H
#define MARVELCODING_CODEC_H

#include <cstdint>

#define MAX_PART_NUM 12
#define ROUND(a,b) (a) + (b) - ((a) % (b))

#define CODEC codec::
#define CODEC_LIB codec::Codec
typedef int Matrix[MAX_PART_NUM][MAX_PART_NUM];

static int matrix_coef[MAX_PART_NUM][MAX_PART_NUM];


namespace codec {
    // a library used to encode and decode
    class Codec {
    public:
        Codec();
        ~Codec();
        // if this codec is available
        bool is_init();
        // Add msg into uncode_msg with each part with a sizeof msg_size;
        void set_msg(char* msg, int msg_size);
        // Set the encoding-iterate time
        // Default: time <= 16
        void set_time(uint16_t time);
        // Encode a msg
        // Result saved in msg_out
        // Return the sizeof matrix
        int encode(char* msg_out);
        // Decode a msg
        // Result saved in msg_out
        // Return the sizeof matrix
        int decode(char* msg_out);
        //
    private:
        // the time of the iterates(max of 16
        uint16_t _iter_time;
        // size of the matrix
        int _mat_size;
        // size of the vector of message
        int _vec_size;
        // the uncoded message;
        char* _uncode_msg;
        // the cache of the encoded_msg;
        char* _encode_msg;
    };
}

#endif //MARVELCODING_CODEC_H
