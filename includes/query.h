#include <stdlib.h>
#include <stdint.h>


#ifndef QUERY_H
#define QUERY_H


typedef struct {
    uint64_t* numbers;
    uint32_t* back;
    uint64_t* masks1;
    uint64_t* masks2;
    uint64_t* masks3;
    uint64_t* masks4;
    uint32_t* closures;
    uint32_t* from;
    uint32_t* to;
    uint32_t* indices;
    uint32_t* bonds;
    size_t size;
} QMol;


QMol create_QMol(char* buffer);


#endif // QUERY_H
