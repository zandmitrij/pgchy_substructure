#include <stdint.h>


#ifndef MOL_H
#define MOL_H


typedef struct {
    uint64_t* numbers;
    uint64_t* bits1;
    uint64_t* bits2;
    uint64_t* bits3;
    uint64_t* bits4;
    uint64_t* bonds;
    uint32_t* from;
    uint32_t* to;
    uint32_t* indices;
    uint32_t* scope;
    size_t size;
} OMol;


OMol create_Mol(char* buffer);


#endif // MOL_H

