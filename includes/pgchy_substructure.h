#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "postgres.h"
#include "fmgr.h"


#ifndef PGCHY_SUBSTRUCTURE_H
#define PGCHY_SUBSTRUCTURE_H


Datum is_substructure(PG_FUNCTION_ARGS);

#endif // PGCHY_SUBSTRUCTURE_H
