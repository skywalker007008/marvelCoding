/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#ifndef MARVELCODING_CODEC_H
#define MARVELCODING_CODEC_H

#include "matrix.h"


#define CODEC_LIB codec::Codec


static int matrix_coef[MAX_PART_NUM][MAX_PART_NUM];


namespace codec {
    /*!
     * @brief The lib of the codec
     */
    template <typename Type>
    class Codec<Type>  {
    public:
        Codec();
        ~Codec();

        /*!
         *
         * @return if this codec is available
         */
        bool is_init();

        /*!
         * Add msg into uncode_msg with each part with a sizeof msg_size;
         * @param msg msg to be set
         * @param msg_size sizeof the msg
         */
        template<size_t M>
        void set_msg(char* msg, int msg_size);

        /*!
         * Set the encoding-iterate time
         * @param time the total iterate-time of the encoding (default < 16)
         *
         */
        void set_time(uint8_t time);

        /*!
         * Set the matrix in the codec
         * @param matrix matrix to be set
         */
        template<size_t M>
        void set_matrix(SqMat<Type, M>* square_matrix);

        /*!
         * Encode a msg
         * @param msg_out buffer to receive the encoded msg
         * @return sizeof the matrix
         */
        template<size_t M>
        int encode(char* msg_out);

        /*!
         * Decode a msg
         * @param msg_out buffer to receive the encoded msg
         * @return sizeof the matrix
         */
        template<size_t M>
        int decode(char* msg_out);
        //
    private:
        /*! the init matrix */
        SqMat* _square_matrix;
        /*! the total iterate time */
        uint8_t _iter_time;
        /*! the size of the matrix */
        int _mat_size;
        /*! size of the vector of message */
        int _vec_size;
        /*! the uncoded message */
        Vec _uncode_msg;
        /*! the cache of the encoded_msg */
        Vec _encode_msg;
    };

    /*!
     * Turn a message into the format of the Vector
     * @param msg unchanged message
     * @return the vector format of the msg
     */
    Vec msg2vec(char* msg);

    /*!
     * Turn the message vector to the original format of char*
     * @param vec the unchanged message vector
     * @param size the size of the vector
     * @return the char* format of the msg
     */
    char* vec2msg(Vec& vec, int size);

    /*!
     * Turn the message vector to the format of uint64_t*
     * @param vec the unchanged message vector
     * @param size the size of the vector
     * @return the uint64_t* format of the msg
     */
    uint64_t* vec2u64(Vec& vec, int size);
}

#endif //MARVELCODING_CODEC_H
