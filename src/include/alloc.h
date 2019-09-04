/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#ifndef MARVELCODING_FREE_H
#define MARVELCODING_FREE_H

#include <stdlib.h>

void free_array2(void** array2, int vec_size);

template <typename T>
void alloc_array2(T** array2, int vec_size, int size);

template <typename T>
void alloc_array(T* array, int size);

#endif //MARVELCODING_FREE_H
