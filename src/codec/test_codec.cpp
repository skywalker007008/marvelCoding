/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/rlnc-lib
 * 
 */

#include "test_codec.h"

void RLNC print(char* msg) {
    int len = strlen(msg);
    for (int i = 0; i < len / 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%x ", (uint8_t)msg[i * 8 + j]);
        }
        std::cout << "\n";
    }
}

void RLNC print_mul() {
    for (int i = 0; i < gFieldSize; i++) {
        for (int j = 0; j < gFieldSize; j++) {
            printf("%x ", table_mul[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    for (int i = 0; i < gFieldSize; i++) {
        for (int j = 0; j < gFieldSize; j++) {
            printf("%x ", table_div[i][j]);
        }
        printf("\n");
    }

    printf("%x", gf_mul(3,0xE));

}

void RLNC print_mat(GFType** mat, int vec_size) {
    printf("matrix::\n");
    for (int i = 0; i < vec_size; i++) {
        for (int j = 0; j < vec_size; j++) {
            printf("%x ", mat[i][j]);
        }
        printf("\n");
    }
}

void RLNC test_encode_decode() {
    // TODO: Init
    GFType** rand1;
    GFType** rand2;
    char* encode_buf1 = (char*)malloc(128 * sizeof(char));
    char* encode_buf2 = (char*)malloc(128 * sizeof(char));
    char* encode_buf3 = (char*)malloc(128 * sizeof(char));
    char* decode_buf = (char*)malloc(128 * sizeof(char));

    char packet[128] = "This is the test code. "
            "And encode and decode this message to test the result right or not";
    RLNC print(packet);
    int packet_size = 64;

    CODEC codec(2, packet_size);
    codec.RecvMessage(packet, std_coef[0]);
    codec.RecvMessage(packet + packet_size, std_coef[1]);
    rand1 = codec.encode();
    RLNC print_mat(rand1, 2);
    // rand1 = gf_newcoef(rand1, std_coef, 2, 2);
    // RLNC print_mat(rand1, 2);
    codec.get_encode_message(encode_buf1);
    std::cout << "Encode1 Finished\n";

    CODEC codec2(2, packet_size);
    codec2.RecvMessage(encode_buf1, rand1[0]);
    codec2.RecvMessage(encode_buf1 + packet_size, rand1[1]);
    rand2 = codec2.encode();
    RLNC print_mat(rand2, 2);
    // rand2 = gf_newcoef(rand2, rand1, 2, 2);
    // RLNC print_mat(rand2, 2);
    codec2.get_encode_message(encode_buf2);
    std::cout << "Encode2 Finished\n";

    CODEC codec3(2, packet_size);
    codec3.RecvMessage(encode_buf2, rand2[0]);
    codec3.RecvMessage(encode_buf2 + packet_size, rand2[1]);
    if (codec3.LinkMsg()) {
        std::cout << "Link Finished\n";
        codec3.decode();
        codec3.get_decode_message(decode_buf);
        std::cout << "Decode Finished\n";
        RLNC print(decode_buf);
    }

    GFType a = gf_add(gf_mul(0xbd, 0xf1), gf_mul(0x24, 0x6b));
    GFType b = gf_add(gf_mul(0xbd, 0xd3), gf_mul(0x24, 0xc4));
    GFType c = gf_add(gf_mul(0x79, 0xf1), gf_mul(0xa3, 0x6b));
    GFType d = gf_add(gf_mul(0x79, 0xd3), gf_mul(0xa3, 0xc4));

    GFType e = gf_div(0xde, 0x1a);
    printf("%x %x %x %x\n", a, b, c, d);


    // printf("%x\n", e);

    // RLNC print_mul();
    free(encode_buf1);
    free(encode_buf2);
    free(encode_buf3);
}

void RLNC test_link() {
    char packet1[128] = "This is the test code. "
            "And encode and decode this message to test the result right or not";
    char* packet2 = (char*)malloc(128 * sizeof(char));

    char packet3[128] = "We are glad to see the result is right. "
            "But there are also some annoying bug here.";
    char* buf1 = (char*)malloc(256 * sizeof(char));
    char* buf2 = (char*)malloc(256 * sizeof(char));
    memcpy(packet2, packet1, 128);
    GFType** coef = (GFType**)malloc(2 * sizeof(GFType*));
    coef[0] = (GFType*)malloc(2 * sizeof(GFType));

    CODEC* temp_codec = new CODEC(2, 128);
    temp_codec -> RecvMessage(packet1, std_coef[0]);
    temp_codec -> RecvMessage(packet3, std_coef[1]);
    coef = temp_codec -> encode();

    temp_codec -> get_encode_message(buf1);
    printf("Original Data\n");
    RLNC print(packet1);
    RLNC print(packet3);

    CODEC codec(2, 128);
    codec.RecvMessage(packet1, std_coef[0]);
    codec.RecvMessage(packet2, std_coef[0]);
    codec.RecvMessage(buf1, coef[0]);

    if (codec.LinkMsg()) {
        printf("Link Succeeds\n");
        codec.decode();
        codec.get_decode_message(buf2);
    } else {
        printf("Link Failed\n");
    }

    printf("Decode Data\n");
    RLNC print(buf2);
    RLNC print(buf2 + 128);
}