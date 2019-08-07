/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include <iostream>
#include "test_codec.h"

void test_codec() {
    CODEC init();
    char message[64] = "This is the silly test file, now is the implement.0123456789012";
    char message_out[64];
    CODEC_LIB<int8_t> codec;
    std::cout << "-------Test Started!-------\n";

    std::cout << "Original Message\n";
    print(message);
    codec.set_uncode_msg(message, 8);
    // SqMat<int8_t, 8> mat = CODEC GetMatrix(8, 1, false);
    // codec.set_matrix(&mat);
    codec.set_time(1);
    message_out = CODEC vec2msg<uint64_t, 8>(codec.encode<u_int64_t, 8>());
    std::cout << "Encode Message\n";
    print(message_out);
    message_out = CODEC vec2msg<uint64_t, 8>(codec.decode());
    std::cout << "Decode Message\n";
    print(message_out);

    std::cout << "-------Test Finished!-------\n";

}

void print(char* data) {
    int size = strlen(data);
    uint64_t *data64 = (uint64_t *)data;
    for (int i = 0; i < (size / 8); i++) {
        std::cout << *(data64 + i) << std::endl;
    }
}