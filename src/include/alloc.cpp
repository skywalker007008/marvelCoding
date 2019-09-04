/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include "alloc.h"

void free_array2(void** array2, int vec_size) {
    for (int i = 0; i < vec_size; i++) {
        free(array2[i]);
    }
    free(array2);
}

template <typename T>
void alloc_array2(T** array2, int vec_size, int size) {
    array2 = (T**)malloc(vec_size * sizeof(T*));
    for (int i = 0; i < vec_size; i++) {
        array2[i] = (T*)malloc(size * sizeof(T));
    }
}

template <typename T>
void alloc_array(T* array, int size) {
    array = (T*)malloc(size * sizeof(T));
}