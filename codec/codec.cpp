/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include <malloc.h>
#include <cstring>
#include "codec.h"

CODEC_LIB::Codec()
        : _uncode_msg(nullptr), _encode_msg(nullptr), _mat_size(-1), _vec_size(0) {}

CODEC_LIB::~Codec() {
    free(_uncode_msg);
    free(_encode_msg);
}

bool CODEC_LIB::is_init() {
    if (_uncode_msg == nullptr || _encode_msg == nullptr || _mat_size < 0) {
        return false;
    }
    return true;
}

void CODEC_LIB::set_msg(char* msg, int msg_size) {
    int size = sizeof(msg);
    int length = strlen(msg);
    int round_size = ROUND(size, msg_size);
    _mat_size = round_size / msg_size;
    _vec_size = msg_size;
    _uncode_msg = (char*)malloc(round_size);
    memcpy(_uncode_msg, msg, msg_size);
    memset(_uncode_msg + msg_size, 0, round_size - msg_size);
}

void CODEC_LIB::set_time(uint16_t time) {
    _iter_time = time;
}

int CODEC_LIB::encode(char* msg_out) {
    Matrix* mat;
    CODEC GetMatrix(mat);
    CODEC mul(mat, _uncode_msg, _mat_size, _vec_size, _iter_time, msg_out);
    return _mat_size;
}

int CODEC_LIB::decode(char* msg_out) {
    Matrix* mat;
    CODEC GetDivMatrix(mat);
    CODEC div(mat, _uncode_msg, _mat_size, _vec_size, _iter_time, msg_out);
}

