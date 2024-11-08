#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <includes/utils.h>


int read_size(unsigned char *buffer) {
    int s = (int)((buffer[1] << 8) | buffer[0]) - 2;
    printf("Read size: %d (buffer[0]: %d, buffer[1]: %d)\n", s, buffer[0], buffer[1]);
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
    MemoryBuffer buf;
    buf.path = (unsigned int *)malloc(q_size_dec * sizeof(unsigned int));
    buf.stack_index = (unsigned int *)malloc(2 * mol_size * sizeof(unsigned int));
    buf.stack_depth = (unsigned int *)malloc(2 * mol_size * sizeof(unsigned int));
    buf.matched = (bool *)malloc(mol_size * sizeof(bool));
    buf.closures = (unsigned long long *)malloc(mol_size * sizeof(unsigned long long));

    if (!buf.path || !buf.stack_index || !buf.stack_depth || !buf.matched || !buf.closures) {
        free(buf.path);
        free(buf.stack_index);
        free(buf.stack_depth);
        free(buf.matched);
        free(buf.closures);
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    memset(buf.matched, 0, mol_size * sizeof(bool));
    memset(buf.closures, 0, mol_size * sizeof(unsigned long long));
    return buf;
}


void free_buffers(MemoryBuffer* buf) {
    free(buf->path);
    free(buf->stack_index);
    free(buf->stack_depth);
    free(buf->matched);
    free(buf->closures);
}

