#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <includes/mol.h>
#include <query.h>
#include <utils.h>
#include <get_mapping.h>
#include <pgchy_substructure.h>

#include "postgres.h"
#include "fmgr.h"


#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif


PGDLLEXPORT Datum is_substructure(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(is_substructure);


Datum is_substructure(PG_FUNCTION_ARGS) {
    bytea* query_bytes = PG_GETARG_BYTEA_P(0);
    bytea* mol_bytes = PG_GETARG_BYTEA_P(1);

    QMol query = create_QMol(VARDATA(query_bytes));
    OMol mol = create_OMol(VARDATA(mol_bytes));

    bool result = get_mapping(&query, &mol);

    PG_RETURN_BOOL(result);
}
