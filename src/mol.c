#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <includes/mol.h>
#include <includes/utils.h>


OMol create_Mol(char* buffer) {
    // numbers
    int numbers_size = read_size(buffer);
    buffer += 2;

    uint64_t *numbers = (uint64_t*)malloc((numbers_size));

    if (numbers == NULL) {
        fprintf(stderr, "Numbers memory allocation failed\n");
    }

    read_64(buffer, numbers, numbers_size);
    buffer += numbers_size;

    printf("numbers size: %lu\n", numbers_size);
    for (int i = 0; i < numbers_size / 8; i++) {
        printf("%llu\n", numbers[i]);
    }

    // bits1
    int bits1_size = read_size(buffer);
    buffer += 2;

    uint64_t *bits1 = (uint64_t*)malloc((bits1_size));

    if (bits1 == NULL) {
        fprintf(stderr, "Bits 1 Memory allocation failed\n");
    }
    read_64(buffer, bits1, bits1_size);
    buffer += bits1_size;

    printf("bits1 size: %lu\n", bits1_size);
    for (int i = 0; i < bits1_size / 8; i++) {
        printf("%llu\n", bits1[i]);
    }

    // bits2
    int bits2_size = read_size(buffer);
    printf("bits2 size: %lu\n", bits2_size);

    buffer += 2;

    uint64_t *bits2 = (uint64_t*)malloc((bits2_size));

    if (bits2 == NULL) {
        fprintf(stderr, "Bits 2 Memory allocation failed\n");
    }
    read_64(buffer, bits2, bits2_size);
    buffer += bits2_size;

    // bits3
    int bits3_size = read_size(buffer);
    printf("bits3 size: %lu\n", bits3_size);

    buffer += 2;

    uint64_t *bits3 = (uint64_t*)malloc((bits3_size));

    if (bits3 == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
    }
    read_64(buffer, bits3, bits3_size);
    buffer += bits3_size;

    // bits4
    int bits4_size = read_size(buffer);
    printf("bits4 size: %lu\n", bits4_size);

    buffer += 2;

    uint64_t *bits4 = (uint64_t*)malloc((bits4_size));

    if (bits4 == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
    }
    read_64(buffer, bits4, bits4_size);
    buffer += bits4_size;

    for (int i = 0; i < bits4_size / 8; i++) {
        printf("%llu\n", bits4[i]);
    }

    // bonds

    int bonds_size = read_size(buffer);
    buffer += 2;

    printf("bonds size %u\n", bonds_size);
    uint64_t *bonds = (uint64_t*)malloc((bonds_size));

    if (bonds == NULL) {
        fprintf(stderr, "Memory allocation for bonds failed\n");
    }
    read_64(buffer, bonds, bonds_size);
    buffer += bonds_size;

    for (int i = 0; i < bonds_size / 8; i++) {
        printf("%llu\n", bonds[i]);
    }

    // from
    int from_size = read_size(buffer);
    buffer += 2;

    uint32_t *from = (uint32_t*)malloc((from_size));

    if (from == NULL) {
        fprintf(stderr, "Memory allocation for from failed\n");
    }
    read_32(buffer, from, from_size);
    buffer += from_size;

    // to
    int to_size = read_size(buffer);
    buffer += 2;

    uint32_t *to = (uint32_t*)malloc((to_size));

    if (to == NULL) {
        fprintf(stderr, "Memory allocation for to failed\n");
    }
    read_32(buffer, to, to_size);
    buffer += to_size;

    // indices
    int indices_size = read_size(buffer);
    buffer += 2;

    uint32_t *indices = (uint32_t*)malloc((indices_size));

    if (indices == NULL) {
        fprintf(stderr, "Memory allocation for indices failed\n");
    }
    read_32(buffer, indices, indices_size);
    buffer += indices_size;

    // scope
    int scope_size = read_size(buffer);
    buffer += 2;

    uint32_t *scope = (uint32_t*)malloc((scope_size));
    if (scope == NULL) {
        fprintf(stderr, "Memory allocation for scope failed\n");
    }
    read_32(buffer, scope, scope_size);

    // size
    size_t size = numbers_size / sizeof(uint64_t);

    OMol mol = {numbers, bits1, bits2, bits3, bits4, bonds, from, to, indices, scope, size};
    return mol;
}

