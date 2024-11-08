CREATE TABLE public.mol_table (
    mol_bytes bytea
);


select smiles_to_bytes('C1CCCCC1')::bytea;


INSERT INTO public.mol_table (mol_bytes) VALUES 
(smiles_to_bytes('C1CCCCC1')::bytea),
(smiles_to_bytes('C1CCCCC1С')::bytea);
-- (smiles_to_bytes('C1=CC=CC=C1')::bytea),
-- (smiles_to_bytes('C1=CC=CC=C1CCCCN')::bytea),
-- (smiles_to_bytes('C1=CC=CC=C1CC(CCCC)CC')::bytea);



SELECT * from public.mol_table
WHERE is_substructure(smarts_to_bytes('CCC'), mol_bytes);
