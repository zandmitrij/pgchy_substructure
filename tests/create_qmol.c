// #include "postgres.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <src/pgchy_substructure.c>


int main() {
    FILE *fileptr;
    char *buffer;
    long filelen;

    fileptr = fopen("../data/query_bytes1", "rb");  // Open the file in binary mode
    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file

    buffer = (char *)malloc(filelen * sizeof(char)); // Enough memory for the file
    fread(buffer, filelen, 1, fileptr); // Read in the entire file

    QMol qmol = create_QMol(buffer);

    for (int i = 0; i < 6; i++) {
        printf("%lu\n", qmol.masks1[i]);
    }
    fclose(fileptr); // Close the file

    return 0;
}
