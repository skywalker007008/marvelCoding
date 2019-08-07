/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include "matrix.h"

static SqMat<uint8_t, MAX_PART_NUM> init_mat;

void CODEC init() {
    for (int i = 0; i < MAX_PART_NUM, i++) {
        for (int j = 0; j < MAX_PART_NUM, j++) {
            if (i <= j) {
                init_mat(i, j) = 1;
            } else {
                init_mat(i, j) = 0;
            }
        }
    }
}

template <typename Type, size_t M>
SqMat<Type, M> CODEC GetMatrix(u_int8_t M, u_int8_t iter_time, bool is_inv) {
    SqMat<Type, M> mat = init_mat.slice(0, 0);
    SqMat<Type, M> temp_mat(mat);
    for (int i = 0; i < iter_time; i++) {
        mat *= temp_mat;
    }
    if (is_inv) {
        return inv(mat);
    } else {
        return mat;
    }
}

template <typename Type, size_t M>
SqMat<Type, M> CODEC GetSquareMatrix(bool is_inv) {
    // TODO: Use RLNC algorithm to get a matrix
}

template <typename Type, size_t M>
Vec<Type, M> CODEC mul(SqMat<Type, M>& mat, Vec<Type, M>& msg, int mat_size, int vec_size,
                    uint16_t iter_time) {
    // TODO: Multiply the Matrix and the vector msg
    return mat * msg;
}

template <typename Type, size_t M>
Vec<Type, M> CODEC div(SqMat<Type, M>& mat, Vec<Type, M>& msg, int mat_size, int vec_size,
               uint16_t iter_time) {
    // TODO: Divide the vector msg and the inv(mat)
    SqMat<Type, M> inv_mat(MATRIX inv(mat));
    return mul(inv_mat, msg, mat_size, vec_size, iter_time);
}





