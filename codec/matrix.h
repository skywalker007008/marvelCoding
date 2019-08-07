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
#define Vec matrix::Vector

// typedef matrix::SquareMatrix Sqmat;
extern SqMat<uint8_t, MAX_PART_NUM> init_mat;

namespace codec {

    /*!
     * Init the init_mat
     */
    void init();

    /*!
     * Get a coefficient matrix with certain size and iter_time
     * @tparam Type data type of the matrix
     * @tparam M data size
     * @param size equal to M
     * @param iter_time the iter_time of the wanted matrix
     * @param is_inv if we want the inv matrix
     * @return the matrix required
     */
    template <typename Type, size_t M>
    SqMat<Type, M> GetMatrix(u_int8_t M, u_int8_t iter_time, bool is_inv);

    /*!
     *
     * @return The matrix generated.
     */
    template <typename Type, size_t M>
    SqMat<Type, M> GetSquareMatrix(bool is_init);

    /*!
     * Encode msg with the mat
     * @param mat Used for Mult
     * @param msg Used for Mult
     * @param mat_size Sizeof the mat
     * @param vec_size Sizeof the vector of msg
     * @param iter_time Times of the iterate
     * @param msg_out Ptr to receive calculated msg
     * @return the result
     */
    template <typename Type, size_t M>
    Vec<Type, M> mul(SqMat<Type, M>& mat, Vec<Type, M>& msg, int mat_size, int vec_size,
             uint16_t iter_time);

    /*!
     * Dncode msg with the mat
     * @param mat Used for Mult
     * @param msg Used for Mult
     * @param mat_size Sizeof the mat
     * @param vec_size Sizeof the vector of msg
     * @param iter_time Times of the iterate
     * @param msg_out Ptr to receive calculated msg
     * @return the result
     */
    template <typename Type, size_t M>
    Vec<Type, M> div(SqMat<Type, M>& mat, Vec<Type, M>& msg, int mat_size, int vec_size,
             uint16_t iter_time);


}

#endif //MARVELCODING_MATRIX_H
