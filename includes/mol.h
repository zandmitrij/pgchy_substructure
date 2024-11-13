#include <stdint.h>
#include <stdlib.h>


#ifndef MOL_H
#define MOL_H


typedef struct {
    uint64_t bits1;
    uint64_t bits2;
    uint64_t bits3;
    uint64_t bits4;
    uint32_t from;
    uint32_t to;
} __attribute__((packed)) OAtom;


typedef struct {
    uint64_t bond;
    uint32_t index;
} __attribute__((packed)) OBond;


typedef struct {
    size_t atoms_count;
    size_t bonds_count;

    OAtom *atoms;
    OBond *bonds;
} OMol;


OMol create_OMol(unsigned char* buffer);


#endif // MOL_H
