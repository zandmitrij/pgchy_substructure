import itertools
import typing as tp

from array import array
from sys import byteorder

import chython


def pack(mol_container: chython.MoleculeContainer) -> bytes:
    atom_sizes = (8, 8, 8, 8, 4, 4)
    bond_sizes = (8, 4)

    _, bits1, bits2, bits3, bits4, bonds, from_, to, indices =  mol_container._cython_compiled_structure

    atoms_count = len(bits1).to_bytes(2, byteorder)
    bonds_count = len(bonds).to_bytes(2, byteorder)

    atoms_ = []
    for i in zip(bits1, bits2, bits3, bits4, from_, to):
        for size, j in zip(atom_sizes, i):
            x = j.to_bytes(size, byteorder)
            atoms_.append(x)

    bonds_ = []
    for i in zip(bonds, indices):
        for size, j in zip(bond_sizes, i):
            x = j.to_bytes(size, byteorder)
            bonds_.append(x)

    return b''.join([atoms_count, bonds_count, *atoms_, *bonds_])


def save(smiles: str, filepath: str):
    mol = chython.smiles(smiles)

    mol_bytes = pack(mol)
    with open(filepath, 'wb') as f:
        f.write(mol_bytes)


def unpack(mol: bytes) -> list:
    atoms_codes = ('Q', 'Q', 'Q', 'Q', 'I', 'I')
    atoms_sizes = (8, 8, 8, 8, 4, 4)

    bonds_codes = ('Q', 'I')
    bonds_sizes = (8, 4)
    
    package = []
    atoms_count = int.from_bytes(mol[0: 2], byteorder)
    bonds_count = int.from_bytes(mol[2: 4], byteorder)
    
    shift = 4
    for atom_code, atom_size in zip(atoms_codes, atoms_sizes):
        size = atom_size * atoms_count
        package.append(array(atom_code, mol[shift: shift + size]))
        shift += size
    
    for bond_code, bond_size in zip(bonds_codes, bonds_sizes):
        size = bond_size * bonds_count
        package.append(array(bond_code, mol[shift: shift + size]))
        shift += size

    return package


# -- for testing purposes


class OAtom(tp.NamedTuple):
    bits1: int
    bits2: int
    bits3: int
    bits4: int
    from_: int
    to: int

    @classmethod
    def from_string(cls, line: str):
        bits1, bits2, bits3, bits4, from_, to = map(int, line.split(', '))
        return cls(bits1, bits2, bits3, bits4, from_, to)


class OBond(tp.NamedTuple):
    bond: int
    index: int

    @classmethod
    def from_string(cls, line: str):
        bond, index = map(int, line.split(', '))
        return cls(bond, index)


class Omol(tp.NamedTuple):
    atoms_count: int
    bonds_count: int

    atoms: tp.List[OAtom]
    bonds: tp.List[OBond]

    @classmethod
    def from_c_output(cls, output: str):
        lines = iter(output.split('\n'))

        atoms_count = int(next(lines).replace('atoms: ', ''))
        atoms = list(map(OAtom.from_string, itertools.islice(lines, atoms_count)))

        # skip empty line
        next(lines)

        bonds_count = int(next(lines).replace('bonds: ', ''))
        bonds = list(map(OBond.from_string, itertools.islice(lines, bonds_count)))

        return cls(atoms_count, bonds_count, atoms, bonds)

    @classmethod
    def from_chython_mol(cls, mol_container: chython.MoleculeContainer):
        _, bits1, bits2, bits3, bits4, bonds, from_, to, indices =  mol_container._cython_compiled_structure

        atoms_count = len(bits1)
        atoms = [OAtom(*i) for i in zip(bits1, bits2, bits3, bits4, from_, to)]

        bonds_count = len(bonds)
        bonds = [OBond(*i) for i in zip(bonds, indices)]
        
        return cls(atoms_count, bonds_count, atoms, bonds)

    @classmethod
    def from_smiles(cls, smiles: str):
        mol_container = chython.smiles(smiles)
        return cls.from_chython_mol(mol_container)
