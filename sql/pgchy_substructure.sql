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
