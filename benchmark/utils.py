from itertools import islice
import typing as tp
from sys import byteorder


T = tp.TypeVar('T')


def chunk_collection(it: tp.Iterable[T], size: int) -> tp.Iterator[tp.Tuple[T, ...]]:
    it = iter(it)
    return iter(lambda: tuple(islice(it, size)), ())


def to_hex(__i: int):
    hex = __i.to_bytes(1, byteorder).hex()
    return f'0x{hex}'


def print_bytes(__bytes: bytes) -> None:
    print('    unsigned char buffer[] = {')
    for i in chunk_collection(map(to_hex, __bytes), 8):
        print('        ', end='')
        print(*i, sep=', ', end=',\n')
    print('    };')
