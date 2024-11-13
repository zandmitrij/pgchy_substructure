#include <stdlib.h>
#include <stdint.h>


#ifndef QUERY_H
#define QUERY_H


typedef struct {
    uint64_t mask1;
    uint64_t mask2;
    uint64_t mask3;
    uint64_t mask4;
    uint32_t back;
    uint32_t closure;  // count of closures
    uint32_t from;
    uint32_t to;  // equal to the number of atoms. which element in the array of indices and bonds contains the necessary information (from + closure)
} __attribute__((packed)) QAtom;


typedef struct {
    uint64_t bond;
    uint32_t index;
} __attribute__((packed)) QBond;


typedef struct {
    size_t atoms_count;
    size_t bonds_count;

    QAtom *atoms;
    QBond *bonds;
} QMol;


QMol create_QMol(unsigned char* buffer);


#endif // QUERY_H
