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
try:
    query = chython.smarts(smarts)
    if query.connected_components_count != 1:
        return None
    package = []
    for x in query._cython_compiled_query[0]:
        package.append((x.itemsize * len(x) + 2).to_bytes(2, byteorder))
        package.append(x)
    return b''.join(package)
except Exception as e:
    return None
$function$;



create or replace function smiles_to_bytes(smiles text)
returns bytea
language plpython3u
as $function$
import chython
from sys import byteorder
try:
    mol_container = chython.smiles(smiles)
    package = []
    for x in mol_container._cython_compiled_structure:
        package.append((x.itemsize * len(x) + 2).to_bytes(2, byteorder))
        print((x.itemsize * len(x) + 2))
        package.append(x)
    x = array('I', [1 for _ in mol_container])
    package.append((x.itemsize * len(x) + 2).to_bytes(2, byteorder))
    package.append(x)
    return b''.join(package)

except Exception as e:
    return None
$function$;