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

template<typename Type, size_t M>
void CODEC_LIB::set_matrix(SqMat<Type, M> &square_matrix) {
    _square_matrix.set(square_matrix, M, M);
}

template <typename Type, size_t M>
int CODEC_LIB::encode(char* msg_out) {
    SqMat mat;
    mat.set(CODEC GetSquareMatrix<Type, M>(), 0, 0);
    _square_matrix *= mat;
    CODEC mul(_square_matrix, _uncode_msg, _mat_size, _vec_size, _iter_time, msg_out);
    return _mat_size;
}

int CODEC_LIB::decode(char* msg_out) {
    // SqMat* mat;
    // CODEC GetDivMatrix(mat, _iter_time);
    CODEC div(_square_matrix, _uncode_msg, _mat_size, _vec_size, _iter_time, msg_out);
    return _mat_size;
}

Vec CODEC msg2vec(char* msg) {
    int size = ROUND(strlen(msg), sizeof(uint64_t));
    int num = size / sizeof(uint64_t);
    uint64_t* ptr = (uint64_t*)msg;

    Vec<uint64_t, num> vec(ptr);
    return vec;
}

char* CODEC vec2msg(Vec& vec, int size) {
    uint64_t* msg_64 = vec2u64(vec, size);
    return (char*)msg_64;
}

uint64_t* CODEC vec2u64(Vec& vec, int size) {
    uint64_t data[size];
    vec.copyTo(data);
    return data;
}

