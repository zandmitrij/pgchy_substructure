from array import array
from sys import byteorder
import chython


def unpack_query(query: bytes) -> list:
    shift = 0
    package = []
    for code in ('L', 'I', 'Q', 'Q', 'Q', 'Q', 'I', 'I', 'I', 'I', 'Q'):
        size = int.from_bytes(query[shift: shift + 2], byteorder)
        arr = array(code, query[shift + 2: shift + size])
        package.append(arr)
        shift += size
    return package


def read_query(filepath: str):
    with open(filepath, 'rb') as f:
        return unpack_query(f.read())


def pack_query(query: chython.QueryContainer):
    if query.connected_components_count != 1:
        return None
    package = []
    for x in query._cython_compiled_query[0]:
        package.append((x.itemsize * len(x) + 2).to_bytes(2, byteorder))
        package.append(x)
    return b''.join(package)


def save_query(smarts: str, filepath: str):
    mol = chython.smarts(smarts)

    mol_bytes = pack_query(mol)
    with open(filepath, 'wb') as f:
        f.write(mol_bytes)
