/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include <malloc.h>
#include <cstring>
#include "codec.h"

class Codec;

template <typename Type>
CODEC_LIB<Type>::Codec()
        : _mat_size(-1), _vec_size(0) {}

template <typename Type>
CODEC_LIB<Type>::~Codec() {
    // free(&_uncode_msg);
    // free(&_encode_msg);
    // free(_square_matrix);
}

template <typename Type>
bool CODEC_LIB<Type>::is_init() {
    if (_mat_size < 0) {
        return false;
    }
    return true;
}

template<typename Type, size_t M>
void CODEC_LIB<Type>::set_uncode_msg(char* msg, int msg_size) {
    int size = sizeof(msg);
    int length = strlen(msg);
    int round_size = ROUND(size, msg_size);
    _mat_size = round_size / msg_size;
    _vec_size = msg_size;
    _uncode_msg = msg;

    // memcpy(_uncode_msg, msg, msg_size);
    // memset(_uncode_msg + msg_size, 0, round_size - msg_size);
}
template <typename Type>
void CODEC_LIB<Type>::set_time(uint8_t time) {
    _iter_time = time;
}

/*template<typename Type, size_t M>
void CODEC_LIB::set_matrix(SqMat<Type, M>* square_matrix) {
    _square_matrix = square_matrix;
}*/

template <typename Type, typename msgType, size_t M>
Vec<msgType, M> CODEC_LIB<Type>::encode() {
    // SqMat mat;
    // mat.set(CODEC GetSquareMatrix<Type, M>(), 0, 0);
    SqMat<Type, M> mat = CODEC GetMatrix<Type, M>(_iter_time, false);
    Vec<msgType, M> msg = CODEC msg2vec<msgType, M>(_uncode_msg);
    Vec<msgType, M> msg_out = CODEC mul<msgType, M>(mat, msg, _mat_size, _vec_size, _iter_time);
    return msg_out;
}

template <typename Type, typename msgType, size_t M>
Vec<msgType, M> CODEC_LIB<Type>::decode() {
    // SqMat* mat;
    // CODEC GetDivMatrix(mat, _iter_time);
    SqMat<Type, M> mat = CODEC GetMatrix<Type, M>(_iter_time, true);
    Vec<msgType, M> msg = CODEC msg2vec<msgType, M>(_encode_msg);
    Vec<msgType, M> msg_out = CODEC div<msgType, M>(mat, msg, _mat_size, _vec_size, _iter_time);
    return msg_out;
}

template <typename Type, size_t M>
Vec<Type, M> CODEC msg2vec(char* msg) {
    // int size = ROUND(strlen(msg), sizeof(Type));
    // int num = size / sizeof(Type);
    Type* ptr = (Type*)msg;
    Vec<Type, M> vec(ptr);
    return vec;
}

template <typename Type, size_t M>
char* CODEC vec2msg(Vec<Type, M>& vec) {
    Type* msg_64 = CODEC vec2type<Type, M> (vec);
    return (char*)msg_64;
}

template <typename Type, size_t M>
Type* CODEC vec2type(Vec<Type, M>& vec) {
    Type data[M];
    vec.copyTo(data);
    return data;
}

