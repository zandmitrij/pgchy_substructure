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


test:
	gcc -I./includes test_o.c src/mol.c src/utils.c -o test_o
	./test_o
	rm ./test_o


test_1:
	gcc -I./includes test_q.c src/query.c src/utils.c -o test_q
	./test_q
	rm test_q


test_o:
	@python -m benchmark smiles 'C1=CC=CC=C1CC(CCCC)CC' -o ./molbytes
	@gcc -I./includes tests/create_omol.c src/mol.c src/utils.c -o test_o
	@python -m benchmark check_smiles 'C1=CC=CC=C1CC(CCCC)CC' --executable ./test_o
	@rm ./test_o ./molbytes


test_q:
	@python -m benchmark smarts C1CCCC1 -o ./molbytes
	@gcc -I./includes tests/create_qmol.c src/query.c src/utils.c -o test_q
	@python -m benchmark check_smarts C1CCCC1 --executable ./test_q
	@rm ./test_q ./molbytes


test_map:
	@python -m benchmark smiles C1CCNCC1 -o ./molbytes
	@python -m benchmark smarts CCN -o ./qbytes
	@gcc -I./includes tests/mapping.c src/query.c src/mol.c src/get_mapping.c src/utils.c -o test_map
	@python -m benchmark check_mapping --executable ./test_map --res 1
	@rm ./test_map ./molbytes ./qbytes
