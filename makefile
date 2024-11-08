EXTENSION = pgchy_substructure
MODULE_big = pgchy_substructure
DATA = sql/pgchy_substructure--1.0.sql
OBJS = src/pgchy_substructure.o src/mol.o src/get_mapping.o src/query.o src/utils.o

PG_CONFIG = pg_config


PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

PG_CFLAGS += -I$(shell $(PG_CONFIG) --includedir-server) -O3 -msse4.2
PG_CFLAGS += src/*
install:
	$(MKDIR_P) '$(DESTDIR)$(shell $(PG_CONFIG) --pkglibdir)'
	$(INSTALL_SHLIB) pgchy_substructure.so '$(DESTDIR)$(shell $(PG_CONFIG) --pkglibdir)'
	$(MKDIR_P) '$(DESTDIR)$(shell $(PG_CONFIG) --sharedir)/extension'
	$(INSTALL_DATA) pgchy_substructure.control '$(DESTDIR)$(shell $(PG_CONFIG) --sharedir)/extension'
	$(INSTALL_DATA) sql/pgchy_substructure--1.0.sql '$(DESTDIR)$(shell $(PG_CONFIG) --sharedir)/extension'
