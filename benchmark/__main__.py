from .mol import read_mol, pack_mol, unpack_mol, save_mol
from .mapping import mapping
from .query import read_query, pack_query, unpack_query, save_query

import chython

def get_mol_structure(smiles: str):
    mol = chython.smiles(smiles)
    return unpack_mol((pack_mol(mol)))


def get_query_structure(smarts: str):
    q = chython.smarts(smarts)
    return unpack_query(pack_query(q))


def main():
    
    # smiles = 'CC(=O)OC1=CC=CC=C1C(=O)O'
    smiles = 'C1CCCC1'
    
    pack_mol(chython.smiles(smiles))
    # smarts = 'CC(=O)O'
    # save_mol(smiles=smiles, filepath='../data/mol_bytes3')
    # save_query(smarts=smarts, filepath='../data/query_bytes3')
    
    # mol_struct = get_mol_structure(smiles)
    
    # save
    # query_struct = get_query_structure()
    # query_struct = read_query('../data/query_bytes3')
    # mol_struct = read_mol('../data/mol_bytes3')
    
    # for i in mol_struct:
    #     print(i)

    # x = mapping(query_struct, mol_struct)
    # print(x)


if __name__ == "__main__":
    main()


# typedef struct {
#     uint64_t* numbers;
#     uint64_t* bits1;
#     uint64_t* bits2;
#     uint64_t* bits3;
#     uint64_t* bits4;
#     uint64_t* bonds;
#     uint32_t* from;
#     uint32_t* to;
#     uint32_t* indices;
# } OMol;