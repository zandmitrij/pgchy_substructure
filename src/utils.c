#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <utils.h>


int read_size(unsigned char *buffer) {
    int s = (int)((buffer[1] << 8) | buffer[0]);
    return s;
}


void read_64(char *buffer, uint64_t* numbers, int size) {
    int array_size = size / OFFSET_64;

    for (int i = 0; i < array_size; i++) {
        numbers[i] = (
            ((uint64_t)(uint8_t)buffer[OFFSET_64 * i + 7] << 56) |
            ((uint64_t)(uint8_t)buffer[OFFSET_64 * i + 6] << 48) |
            ((uint64_t)(uint8_t)buffer[OFFSET_64 * i + 5] << 40) |
            ((uint64_t)(uint8_t)buffer[OFFSET_64 * i + 4] << 32) |
            ((uint64_t)(uint8_t)buffer[OFFSET_64 * i + 3] << 24) |
            ((uint64_t)(uint8_t)buffer[OFFSET_64 * i + 2] << 16) |
            ((uint64_t)(uint8_t)buffer[OFFSET_64 * i + 1] << 8) |
            ((uint64_t)(uint8_t)buffer[OFFSET_64 * i + 0])
        );
    }
}


void read_32(char *buffer, uint32_t* numbers, int size) {
    int offset = sizeof(uint32_t);
    int array_size = size / offset;

    for (int i = 0; i < array_size; i++) {
        numbers[i] = (
            ((uint32_t)(uint8_t)buffer[OFFSET_32 * i + 3] << 24) |
            ((uint32_t)(uint8_t)buffer[OFFSET_32 * i + 2] << 16) |
            ((uint32_t)(uint8_t)buffer[OFFSET_32 * i + 1] << 8) |
            ((uint32_t)(uint8_t)buffer[OFFSET_32 * i + 0])
        );
    }
}


MemoryBuffer allocate_buffers(size_t q_size_dec, size_t mol_size) {

    uint32_t *path = (uint32_t*) malloc(q_size_dec * sizeof(uint32_t));
    uint32_t *stack_index = (uint32_t*) malloc(2 * mol_size * sizeof(uint32_t));
    uint32_t *stack_depth = (uint32_t*) malloc(2 * mol_size * sizeof(uint32_t));
    bool *matched = (bool*)malloc(mol_size * sizeof(bool));
    uint64_t *o_closures = (uint64_t*) malloc(mol_size * sizeof(uint64_t));

    if (path == NULL || stack_index == NULL || stack_depth == NULL || matched == NULL || o_closures == NULL) {
        free(path);
        free(stack_index);
        free(stack_depth);
        free(matched);
        free(o_closures);
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    memset(matched, 0, mol_size * sizeof(bool));
    memset(o_closures, 0, mol_size * sizeof(uint64_t));

    MemoryBuffer buf = {path, stack_index, stack_depth, matched, o_closures};
    return buf;
}


void free_buffers(MemoryBuffer* buf) {
    free(buf->path);
    free(buf->stack_index);
    free(buf->stack_depth);
    free(buf->matched);
    free(buf->o_closures);
}

