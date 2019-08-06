/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#ifndef MARVELCODING_MATRIX_H
#define MARVELCODING_MATRIX_H

#include <cstdint>
#include "../matrix/math.hpp"

#define MAX_PART_NUM 12
#define ROUND(a,b) (a) + (b) - ((a) % (b))

#define CODEC codec::
#define MATRIX matrix::

#define SqMat matrix::SquareMatrix
#define Mat matrix::Matrix

namespace codec {

    /*!
     *
     * @return The matrix generated.
     */
    template <typename Type, size_t M>
    SqMat<Type, M> GetSquareMatrix();

    /*!
     *
     * @param mat Used for Mult
     * @param msg Used for Mult
     * @param mat_size Sizeof the mat
     * @param vec_size Sizeof the vector of msg
     * @param iter_time Times of the iterate
     * @param msg_out Ptr to receive calculated msg
     */
    template <typename Type, size_t M>
    void mul(SqMat<Type, M>& mat, char* msg, int mat_size, int vec_size,
             uint16_t iter_time, char* msg_out);

    /*!
     *
     * @param mat Used for Mult
     * @param msg Used for Mult
     * @param mat_size Sizeof the mat
     * @param vec_size Sizeof the vector of msg
     * @param iter_time Times of the iterate
     * @param msg_out Ptr to receive calculated msg
     */
    template <typename Type, size_t M>
    void div(SqMat<Type, M>& mat, char* msg, int mat_size, int vec_size,
             uint16_t iter_time, char* msg_out);
}

#endif //MARVELCODING_MATRIX_H
