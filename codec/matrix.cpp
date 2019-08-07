/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include "matrix.h"

template <typename Type, size_t M>
SqMat<Type, M> CODEC GetSquareMatrix() {
    // TODO: Use RLNC algorithm to get a matrix
}

template <typename Type, size_t M>
void CODEC mul(SqMat<Type, M>& mat, Vec<Type, M>& msg, int mat_size, int vec_size,
                    uint16_t iter_time, Vec<Type, M>& msg_out) {
    // TODO: Multiply the Matrix and the vector msg
    msg_out.set(mat * msg, 0, 0);
}

template <typename Type, size_t M>
void CODEC div(SqMat<Type, M>& mat, Vec<Type, M>& msg, int mat_size, int vec_size,
               uint16_t iter_time, Vec<Type, M>& msg_out) {
    // TODO: Divide the vector msg and the inv(mat)
    SqMat<Type, M> inv_mat(MATRIX inv(mat));
    mul(inv_mat, msg, mat_size, vec_size, iter_time, msg_out);
}



