# pgchy_substructure

`pgchy_substructure` is a PostgreSQL extension designed for efficient chemical substructure searches. This extension integrates with the chython library to provide advanced molecular matching capabilities.


# Features

* Chemical Substructure Search: Perform fast substructure queries directly in PostgreSQL.

* Integration with chython: Utilize the power of the chython library.

* Custom PostgreSQL Functions: Includes functions for generating molecular and query processing and performing substructure matching.

* High Performance: Optimized for speed and scalability, suitable for large datasets.


# Installation

Prerequisites
* PostgreSQL (version 12 or higher recommended)
* [chython](https://github.com/chython/chython) library installed:

```bash
pip install chython
```
* PostgreSQL development headers and tools:
```bash
sudo apt-get install postgresql-server-dev-<your_version>
```

* C compiler (e.g., gcc)


# Build and Install

Clone the repository:

```bash
git clone https://github.com/zandmitrij/pgchy_substructure.git
cd pgchy_substructure
```

1. Install the extension:

```bash
make install
```

2. Load the extension into your database:

```sql
CREATE EXTENSION pgchy_substructure;
```


# Usage

* `smiles_to_bytes(smiles TEXT)`
Generates bytes from a SMILES string.

Example:

```sql
SELECT smiles_to_bytes('C1=CC=CC=C1'); -- Benzene
```

* `smarts_to_bytes(smarts TEXT)`
Generates bytes from a SMARTS string.

Example:

```sql
SELECT smarts_to_bytes('CC');
```

* `is_substructure(smarts BYTEA, smiles BYTEA)`
Performs a substructure match between a query molecule and a target molecule.

Example:

```sql
SELECT substructure_match(smarts_to_bytes('CC'), smiles_to_bytes('C1=CC=CC=C1C')); -- True
SELECT substructure_match(smarts_to_bytes('CCN'), smiles_to_bytes('C1=CC=CC=C1C')); -- False
```


# Development

Debugging
To enable debugging, set the DEBUG flag in the Makefile:

```bash
CFLAGS += -DDEBUG
make clean && make
```

Check logs for output using printf in the code.


# Troubleshooting

Ensure chython is properly installed and accessible.
Permission Denied During Installation

Run `make install` with `sudo` or appropriate permissions.


# Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature/bug fix.
3. Commit your changes with descriptive messages.
4. Submit a pull request.


# License

This project is licensed under the MIT License. See the [LICENSE](./LICENSE) file for details.


# Acknowledgements

Inspired by PostgreSQL's extensibility.

Built with the [chython](https://github.com/chython/chython) library.

For questions or feedback, feel free to open an issue on GitHub.
