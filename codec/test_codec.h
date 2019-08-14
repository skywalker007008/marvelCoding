/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/rlnc-lib
 * 
 */

#ifndef RLNC_TEST_CODEC_H
#define RLNC_TEST_CODEC_H

#include "codec.h"

namespace rlnc {
    /*!
     * test_print
     * @param msg msg to print
     */
    void print(char* msg);

    void print_mul();

    void print_mat(GFType** mat, int vec_size);

    void test_encode_decode();

    void test_link();
}

#endif //RLNC_TEST_CODEC_H
