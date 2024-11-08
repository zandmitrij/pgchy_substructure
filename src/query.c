#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <includes/query.h>


QMol create_QMol(char* buffer) {
    // numbers
    int numbers_size = read_size(buffer);

    uint64_t *numbers = (uint64_t*)malloc((numbers_size));

    if (numbers == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
    }
    buffer += 2;
    read_64(buffer, numbers, numbers_size);
    buffer += numbers_size;

    // back
    int back_size = read_size(buffer);

    uint32_t *back = (uint32_t*)malloc((back_size));
    if (back == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
    }

    buffer += 2;
    read_32(buffer, back, back_size);
    buffer += back_size;

    // masks1
    int masks1_size = read_size(buffer);

    uint64_t *masks1 = (uint64_t*)malloc((masks1_size));

    if (masks1 == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
    }
    buffer += 2;
    read_64(buffer, masks1, masks1_size);
    buffer += masks1_size;

    // masks1
    int masks2_size = read_size(buffer);

    uint64_t *masks2 = (uint64_t*)malloc((masks2_size));

    if (masks2 == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
    }
    buffer += 2;
    read_64(buffer, masks2, masks2_size);
    buffer += masks2_size;

    // masks3
    int masks3_size = read_size(buffer);

    uint64_t *masks3 = (uint64_t*)malloc((masks3_size));

    if (masks3 == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
    }
    buffer += 2;
    read_64(buffer, masks3, masks3_size);
    buffer += masks3_size;

    // masks4
    int masks4_size = read_size(buffer);

    uint64_t *masks4 = (uint64_t*)malloc((masks4_size));

    if (masks4 == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
    }

    buffer += 2;
    read_64(buffer, masks4, masks4_size);
    buffer += masks4_size;

    // closures
    int closures_size = read_size(buffer);

    uint32_t *closures = (uint32_t*)malloc((closures_size));
    if (closures == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
    }

    buffer += 2;
    read_32(buffer, closures, closures_size);
    buffer += closures_size;

    // from
    int from_size = read_size(buffer);

    uint32_t *from = (uint32_t*)malloc((from_size));
    if (from == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
    }

    buffer += 2;
    read_32(buffer, from, from_size);
    buffer += from_size;

    // to
    int to_size = read_size(buffer);

    uint32_t *to = (uint32_t*)malloc((to_size));
    if (to == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
    }

    buffer += 2;
    read_32(buffer, to, to_size);
    buffer += to_size;

    // indices
    int indices_size = read_size(buffer);

    uint32_t *indices = (uint32_t*)malloc((indices_size));
    if (indices == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
    }

    buffer += 2;
    read_32(buffer, indices, indices_size);
    buffer += indices_size;

    // bonds
    int bonds_size = read_size(buffer);

    uint32_t *bonds = (uint32_t*)malloc((bonds_size));
    if (bonds == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
    }

    buffer += 2;
    read_32(buffer, bonds, bonds_size);
    buffer += bonds_size;

    size_t size = numbers_size / sizeof(uint64_t);

    QMol qmol = {numbers, back, masks1, masks2, masks3, masks4, closures, from, to, indices, bonds, size};
    return qmol;

}

