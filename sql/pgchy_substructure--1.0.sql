-- pgchy_substructure--1.0.sql


CREATE FUNCTION is_substructure(query_bytes bytea, mol_bytes bytea)
RETURNS boolean
AS 'MODULE_PATHNAME', 'is_substructure'
LANGUAGE C STRICT;


create or replace function smarts_to_bytes(smarts text)
returns bytea
language plpython3u
as $function$
import chython
from sys import byteorder
from array import array
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
$function$;



create or replace function smiles_to_bytes(smiles text)
returns bytea
language plpython3u
as $function$
import chython
from sys import byteorder
from array import array
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

$function$;