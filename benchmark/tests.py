
import subprocess

from . import mol
from . import query


def get_c_output(executable: str) -> str:
    result = subprocess.run([executable], capture_output=True, text=True)
    return result.stdout


def check_smiles_read(executable: str, smiles: str):
    expected = mol.Omol.from_smiles(smiles)
    output = get_c_output(executable)
    res = mol.Omol.from_c_output(output)
    
    assert res.atoms_count == expected.atoms_count, f'{res.atoms_count=}, {expected.atoms_count=}'
    assert res.bonds_count == expected.bonds_count, f'{res.bonds_count=}, {expected.bonds_count=}'
    
    for res_atom, expected_atom in zip(res.atoms, expected.atoms):
        assert res_atom == expected_atom, f'{res_atom=}, {expected_atom=}'
        
    for res_bond, expected_bond in zip(res.bonds, expected.bonds):
        assert res_bond == expected_bond, f'{res_bond=}, {expected_bond=}'

    print('OMol creation tests passed')


def check_smarts_read(executable: str, smarts: str):
    expected = query.Qmol.from_smarts(smarts)
    output = get_c_output(executable)
    res = query.Qmol.from_c_output(output)
    
    assert res.atoms_count == expected.atoms_count, f'{res.atoms_count=}, {expected.atoms_count=}'
    assert res.bonds_count == expected.bonds_count, f'{res.bonds_count=}, {expected.bonds_count=}'
    
    for res_atom, expected_atom in zip(res.atoms, expected.atoms):
        assert res_atom == expected_atom, f'{res_atom=}, {expected_atom=}'
        
    for res_bond, expected_bond in zip(res.bonds, expected.bonds):
        assert res_bond == expected_bond, f'{res_bond=}, {expected_bond=}'

    print('QMol creation tests passed')


def check_mapping(executable: str, expected_res: int):
    output = int(get_c_output(executable))
    assert output == expected_res, 'mapping is not working'
    print('mapping tests passed')
