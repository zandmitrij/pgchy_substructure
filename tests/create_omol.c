#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <mol.h>


char* read_file(const char* filepath) {
    FILE* fileptr;
    char* buffer;
    long filelen;

    fileptr = fopen(filepath, "rb");  // Open the file in binary mode
    if (!fileptr) {
        perror("File error");
    }
    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file

    buffer = (char *)malloc(filelen * sizeof(char)); // Enough memory for the file
    fread(buffer, filelen, 1, fileptr); // Read in the entire file
    fclose(fileptr); // Close the file
    return buffer;
}


int main() {
    const char* filepath = "molbytes";

    char* buffer = read_file(filepath);
    OMol mol = create_OMol(buffer);

    printf("atoms: %lu\n", mol.atoms_count);
    for (int i = 0; i < mol.atoms_count; i++) {
        OAtom atom = mol.atoms[i];
        printf("%lu, %lu, %lu, %lu, %u, %u\n", atom.bits1, atom.bits2, atom.bits3, atom.bits4, atom.from, atom.to);
    }
    printf("\n");

    printf("bonds: %lu\n", mol.bonds_count);
    for (int i = 0; i < mol.bonds_count; i++) {
        OBond bond = mol.bonds[i];
        printf("%lu, %u\n", bond.bond, bond.index);
    }
    printf("\n");

    return 0;
}
