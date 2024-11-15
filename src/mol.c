#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <includes/mol.h>
#include <includes/utils.h>


OMol create_OMol(unsigned char* buffer) {
    int atoms_count = read_size(buffer);
    buffer += 2;
    int bonds_count = read_size(buffer);
    buffer += 2;

    OAtom *atoms = (OAtom *)(buffer);

    buffer += sizeof(OAtom) * atoms_count;
    OBond *bonds = (OBond *)(buffer);

    OMol mol = {atoms_count, bonds_count, atoms, bonds};
    return mol;
}

