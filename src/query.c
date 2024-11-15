#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <includes/query.h>
#include <includes/utils.h>


QMol create_QMol(unsigned char* buffer) {
    int atoms_count = read_size(buffer);
    buffer += 2;
    int bonds_count = read_size(buffer);
    buffer += 2;

    QAtom *atoms = (QAtom*)(buffer);

    buffer += (sizeof(QAtom) * atoms_count);
    QBond *bonds = (QBond*)(buffer);

    QMol mol = {atoms_count, bonds_count, atoms, bonds};
    return mol;
}

