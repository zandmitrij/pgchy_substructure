import typing as tp

import chython
from array import array
from sys import byteorder


def pack_mol(mol_container: chython.MoleculeContainer) -> bytes:
    package = []
    for x in mol_container._cython_compiled_structure:
        print(x)
        print(len(x))
        print()
        package.append((x.itemsize * len(x) + 2).to_bytes(2, byteorder))
        package.append(x)
    x = array('I', [1 for _ in mol_container])
    package.append((x.itemsize * len(x) + 2).to_bytes(2, byteorder))
    package.append(x)
    return b''.join(package)


def unpack_mol(mol: bytes) -> list:    
    shift = 0
    package = []
    for code in ('L', 'Q', 'Q', 'Q', 'Q', 'Q', 'I', 'I', 'I', 'I'):
        size = int.from_bytes(mol[shift: shift + 2], byteorder)
        package.append(array(code, mol[shift + 2: shift + size]))
        shift += size
    return package


def save_mol(smiles: str, filepath: str):
    mol = chython.smiles(smiles)

    mol_bytes = pack_mol(mol)
    with open(filepath, 'wb') as f:
        f.write(mol_bytes)


def read_mol(filepath: str):
    with open(filepath, 'rb') as f:
        return unpack_mol(f.read())


class Atom(tp.NamedTuple):
    bits1: int
    bits2: int
    bits3: int
    bits4: int
    from_: int
    to: int


class Bond(tp.NamedTuple):
    pass


class Mol(tp.NamedTuple):
    pass
