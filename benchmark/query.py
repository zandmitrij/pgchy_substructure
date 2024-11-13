import itertools
import typing as tp
from array import array
from sys import byteorder

import chython


def unpack(__bytes: bytes):
    atoms_codes = ('Q', 'Q', 'Q', 'Q', 'I', 'I', 'I', 'I')
    atoms_sizes = (8, 8, 8, 8, 4, 4, 4, 4)

    bonds_codes = ('Q', 'I')
    bonds_sizes = (8, 4)
    
    package = []
    atoms_count = int.from_bytes(__bytes[0: 2], byteorder)
    bonds_count = int.from_bytes(__bytes[2: 4], byteorder)
    
    shift = 4
    for atom_code, atom_size in zip(atoms_codes, atoms_sizes):
        size = atom_size * atoms_count
        package.append(array(atom_code, __bytes[shift: shift + size]))
        shift += size
    
    for bond_code, bond_size in zip(bonds_codes, bonds_sizes):
        size = bond_size * bonds_count
        package.append(array(bond_code, __bytes[shift: shift + size]))
        shift += size

    return package


def pack(query: chython.QueryContainer) -> bytes:
    atom_sizes = (8, 8, 8, 8, 4, 4, 4, 4)
    bond_sizes = (8, 4)

    if query.connected_components_count != 1:
        return None

    _, back, masks1, masks2, masks3, masks4, closures, from_, to, indices, bonds =  query._cython_compiled_query[0]

    atoms_count = len(masks1).to_bytes(2, byteorder)
    bonds_count = len(bonds).to_bytes(2, byteorder)

    atoms_ = []
    for i in zip(masks1, masks2, masks3, masks4, back, closures, from_, to):
        for size, j in zip(atom_sizes, i):
            x = j.to_bytes(size, byteorder)
            atoms_.append(x)

    bonds_ = []
    for i in zip(bonds, indices):
        for size, j in zip(bond_sizes, i):
            x = j.to_bytes(size, byteorder)
            bonds_.append(x)

    return b''.join([atoms_count, bonds_count, *atoms_, *bonds_])


def save(smarts: str, filepath: str):
    query = chython.smarts(smarts)

    query_bytes = pack(query)
    with open(filepath, 'wb') as f:
        f.write(query_bytes)



# -- for testing purposes


class QAtom(tp.NamedTuple):
    mask1: int
    mask2: int
    mask3: int
    mask4: int
    back: int
    closure: int
    from_: int
    to: int

    @classmethod
    def from_string(cls, line: str):
        mask1, mask2, mask3, mask4, back, closure, from_, to = map(int, line.split(', '))
        return cls(mask1, mask2, mask3, mask4, back, closure, from_, to)


class QBond(tp.NamedTuple):
    bond: int
    index: int

    @classmethod
    def from_string(cls, line: str):
        bond, index = map(int, line.split(', '))
        return cls(bond, index)


class Qmol(tp.NamedTuple):
    atoms_count: int
    bonds_count: int

    atoms: tp.List[QAtom]
    bonds: tp.List[QBond]

    @classmethod
    def from_c_output(cls, output: str):
        lines = iter(output.split('\n'))

        atoms_count = int(next(lines).replace('atoms: ', ''))
        atoms = list(map(QAtom.from_string, itertools.islice(lines, atoms_count)))

        # skip empty line
        next(lines)

        bonds_count = int(next(lines).replace('bonds: ', ''))
        bonds = list(map(QBond.from_string, itertools.islice(lines, bonds_count)))

        return cls(atoms_count, bonds_count, atoms, bonds)

    @classmethod
    def from_chython_query(cls, query: chython.QueryContainer):
        if query.connected_components_count != 1:
            raise ValueError()

        _, back, masks1, masks2, masks3, masks4, closures, from_, to, indices, bonds =  query._cython_compiled_query[0]

        atoms_count = len(masks1)
        atoms = [QAtom(*i) for i in zip(masks1, masks2, masks3, masks4, back, closures, from_, to)]

        bonds_count = len(bonds)
        bonds = [QBond(*i) for i in zip(bonds, indices)]
        
        return cls(atoms_count, bonds_count, atoms, bonds)

    @classmethod
    def from_smarts(cls, smarts: str):
        mol_container = chython.smarts(smarts)
        return cls.from_chython_query(mol_container)
