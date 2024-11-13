#include <stdint.h>
#include <stdbool.h>


#define OFFSET_64 8
#define OFFSET_32 4

#ifndef UTILS_H
#define UTILS_H


int read_size(unsigned char *buffer);


void read_64(char *buffer, uint64_t* numbers, int size);


void read_32(char *buffer, uint32_t* numbers, int size);


typedef struct {
    uint32_t *path;
    uint32_t *stack_index;
    uint32_t *stack_depth;
    bool *matched;
    uint64_t *o_closures;
} MemoryBuffer;


MemoryBuffer allocate_buffers(size_t q_size_dec, size_t mol_size);


void free_buffers(MemoryBuffer *buf);


#endif // UTILS_H
