#include <cstdint>

#include "codec.h"

/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/rlnc-lib
 * 
 */
GFType** std_coef;

CODEC::Codec(int vec_size, int packet_size) :
        _vec_size(vec_size), _recv_num(0), _packet_size(packet_size),
        _is_enough(false), _is_full(false) {
    _coef_mat = (GFType**)malloc(_vec_size * sizeof(GFType*));
    _cache_coef_mat = (GFType**)malloc(kMaxBufSize * sizeof(GFType*));
    _raw_msg = (char*)malloc(_vec_size * _packet_size * sizeof(char*));
    _encode_msg = (char*)malloc(_vec_size * _packet_size * sizeof(char*));
    _decode_msg = (char*)malloc(_vec_size * _packet_size * sizeof(char*));
    _cache_msg = (char*)malloc(kMaxBufSize * _packet_size * sizeof(char*));
    for (int i = 0; i < vec_size; i++) {
        _coef_mat[i] = (GFType*)malloc(vec_size * sizeof(GFType));
        _cache_coef_mat[i] = (GFType*)malloc(vec_size * sizeof(GFType));
    }
    for (int i = vec_size; i < kMaxBufSize; i++) {
        _cache_coef_mat[i] = (GFType*)malloc(vec_size * sizeof(GFType));
    }
}

CODEC::~Codec() {
    free(_raw_msg);
    free(_encode_msg);
    free(_decode_msg);
    free(_cache_msg);
    for (int i = 0; i < _vec_size; i++) {
        free(_coef_mat[i]);
        free(_cache_coef_mat[i]);
    }
    for (int i = _vec_size; i < kMaxBufSize; i++) {
        free(_cache_coef_mat[i]);
    }
    free(_coef_mat);
    free(_cache_coef_mat);
}

bool CODEC::is_enough() {
    return _is_enough;
}

bool CODEC::is_full() {
    return _is_enough;
}

void CODEC::RecvMessage(char* msg, GFType* coef) {
    // TODO: Recv the msg and add the coef to the cache
    memcpy((_cache_msg + _recv_num * _packet_size), msg, _packet_size * sizeof(char));
    memcpy(_cache_coef_mat[_recv_num], coef, _vec_size * sizeof(GFType));
    _recv_num++;
    if (_recv_num == kMaxBufSize) {
        _is_full = true;
    }
}

bool CODEC::LinkMsg() {
    if (_recv_num < _vec_size) {
        return false;
    }
    // TODO: Reform the msg with previous _vec_size rows is full-trace
    GFType** orig_mat = (GFType**)malloc(_recv_num * sizeof(GFType*));
    // TODO: Copy the cache_mat
    for (int i = 0; i < _recv_num; i++) {
        orig_mat[i] = (GFType*) malloc(_vec_size * sizeof(GFType));
        memcpy(orig_mat[i], _cache_coef_mat[i], _vec_size * sizeof(GFType));
    }
    // TODO: Turn it into a lower-triangle-matrix
    GFType temp;
    GFType shift_temp;
    char* msg_temp = (char*) malloc(_packet_size * sizeof(char));

    for (int i = 0; i < _vec_size; i++) {
        // TODO: Turn the [i][i] into 1(change both orig and dest)
        temp = orig_mat[i][i];
        // if [i][i] == 0, needs shift
        if (temp == 0) {
            // TODO: Find a row j while [j][i] != 0
            for (int j = i; j < _recv_num; j++) {
                if (orig_mat[j][i] != 0) {
                    // TODO: Shift the 2 lines
                    for (int k = 0; k < _vec_size; k++) {
                        shift_temp = orig_mat[i][k];
                        orig_mat[i][k] = orig_mat[j][k];
                        orig_mat[j][k] = shift_temp;
                        shift_temp = _cache_coef_mat[i][k];
                        _cache_coef_mat[i][k] = _cache_coef_mat[j][k];
                        _cache_coef_mat[j][k] = shift_temp;
                    }
                    memcpy(msg_temp, (_cache_msg + i * _packet_size), _packet_size * sizeof(char));
                    memcpy((_cache_msg + i * _packet_size), (_cache_msg + j * _packet_size), _packet_size * sizeof(char));
                    memcpy((_cache_msg + j * _packet_size), msg_temp, _packet_size * sizeof(char));
                    break;
                }
                // TODO: if not found, return false
                if (j == _recv_num - 1) {
                    _recv_num--;
                    return false;
                }
            }
            temp = orig_mat[i][i];
        }
        for (int j = 0; j < _vec_size; j++) {
            orig_mat[i][j] = gf_div(orig_mat[i][j], temp);
        }
        // TODO: Turn the column[i] into 1,0,0,0......
        for (int t = i + 1; t < _recv_num; t++) {
            temp = orig_mat[t][i];
            for (int j = 0; j < _vec_size; j++) {
                orig_mat[t][j] = gf_sub(orig_mat[t][j], gf_mul(temp, orig_mat[i][j]));
            }
        }
    }
    // TODO: Copy the cache into the place
    for (int i = 0; i < _vec_size; i++) {
        memcpy(_coef_mat[i], _cache_coef_mat[i], _vec_size * sizeof(GFType));
    }
    memcpy(_raw_msg, _cache_msg, _vec_size * _packet_size * sizeof(char));
    _is_enough = true;
    return true;
}

void CODEC::get_encode_message(char* buf) {
    memcpy(buf, _encode_msg, _recv_num * _packet_size * sizeof(char));
}

void CODEC::get_decode_message(char* buf) {
    memcpy(buf, _decode_msg, _vec_size * _packet_size * sizeof(char));
}

GFType** CODEC::encode() {
    // TODO: Generate a matrix
    GFType** rand_list = (GFType**)malloc(_vec_size * sizeof(GFType*));
    GFType rand;
    memset(_encode_msg, 0, _recv_num * _packet_size * sizeof(char));
    memcpy(_raw_msg, _cache_msg, _recv_num * _packet_size * sizeof(char));
    GenerateEncodeMat(_recv_num, rand_list);
    for (int i = 0; i < _recv_num; i++) {
        // rand_list[i] = (GFType*)malloc(_recv_num * sizeof(GFType));
        for (int t = 0; t < _recv_num; t++) {
            rand = rand_list[i][t];
            for (int j = 0; j < _packet_size; j++) {
                _encode_msg[i * _packet_size + j] ^=
                        gf_mul(rand, (uint8_t) _raw_msg[t * _packet_size + j]);
            }
            // rand_list[i][t] = rand;
        }
    }
    rand_list = gf_newcoef(rand_list, _cache_coef_mat, _recv_num, _vec_size);
    return rand_list;
}

void CODEC::decode() {
    GFType** inv_mat = gauss_inv(_coef_mat, _vec_size);
    GFType rand;
    memset(_decode_msg, 0, _vec_size * _packet_size * sizeof(char));
    for (int i = 0; i < _vec_size; i++) {
        for (int t = 0; t < _vec_size; t++) {
            rand = inv_mat[i][t];
            for (int j = 0; j < _packet_size; j++) {
                _decode_msg[i * _packet_size + j] ^=
                        gf_mul(rand, (uint8_t) _raw_msg[t * _packet_size + j]);
            }
        }
    }
}

uint8_t CODEC::get_recv_num() {
    return _recv_num;
}

int CODEC::get_vec_size() {
    return _vec_size;
}

int CODEC::get_packet_size() {
    return _packet_size;
}

void RLNC coef_init() {
    std_coef = (GFType**)malloc(kMaxPartNum * sizeof(GFType*));
    for (int i = 0; i < kMaxPartNum; i++) {
        std_coef[i] = (GFType*)malloc(kMaxPartNum * sizeof(GFType));
        for (int j = 0; j < kMaxPartNum; j++) {
            std_coef[i][j] = ((i == j) ? (GFType)1 : (GFType)0);
        }
    }
}

void RLNC init(unsigned int m) {
    gf_init(m, prim_poly[m]);
    coef_init();
}

void RLNC GenerateEncodeMat(int recv_num, GFType** rand) {
    for (int i = 0; i < recv_num; i++) {
        rand[i] = (GFType *) malloc(recv_num * sizeof(GFType));
    }
    while (true) {
        for (int i = 0; i < recv_num; i++) {
            memset(rand[i], 0, recv_num * sizeof(GFType));
            for (int t = 0; t < recv_num; t++) {
                rand[i][t] = std::rand() % gFieldSize;
            }
        }
        if (is_full(rand, recv_num)) {
            break;
        }
    }
}

bool RLNC is_full(GFType** rand, int vec_size) {
    GFType** orig_mat = (GFType**)malloc(vec_size * sizeof(GFType*));
    // TODO: Copy the cache_mat
    for (int i = 0; i < vec_size; i++) {
        orig_mat[i] = (GFType*) malloc(vec_size * sizeof(GFType));
        memcpy(orig_mat[i], rand[i], vec_size * sizeof(GFType));
    }
    // TODO: Turn it into a lower-triangle-matrix
    GFType temp;
    GFType shift_temp;
    for (int i = 0; i < vec_size; i++) {
        // TODO: Turn the [i][i] into 1(change both orig and dest)
        temp = orig_mat[i][i];
        // if [i][i] == 0, needs shift
        if (temp == 0) {
            // TODO: Find a row j while [j][i] != 0
            for (int j = i; j < vec_size; j++) {
                if (orig_mat[j][i] != 0) {
                    // TODO: Shift the 2 lines
                    for (int k = 0; k < vec_size; k++) {
                        shift_temp = orig_mat[i][k];
                        orig_mat[i][k] = orig_mat[j][k];
                        orig_mat[j][k] = shift_temp;
                    }
                    break;
                }
                // TODO: if not found, return false
                if (j == vec_size - 1) {
                    return false;
                }
            }
            temp = orig_mat[i][i];
        }
        for (int j = 0; j < vec_size; j++) {
            orig_mat[i][j] = gf_div(orig_mat[i][j], temp);
        }
        // TODO: Turn the column[i] into 1,0,0,0......
        for (int t = i + 1; t < vec_size; t++) {
            temp = orig_mat[t][i];
            for (int j = 0; j < vec_size; j++) {
                orig_mat[t][j] = gf_sub(orig_mat[t][j], gf_mul(temp, orig_mat[i][j]));
            }
        }
    }
    return true;
}


