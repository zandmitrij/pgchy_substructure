CREATE TABLE public.mol_table (
    smiles text,
    mol_bytes bytea
);


select smiles_to_bytes('C1CCCCC1')::bytea;


INSERT INTO public.mol_table (mol_bytes) VALUES 
('C1CCCCC1', smiles_to_bytes('C1CCCCC1')::bytea),
('C1CCCCC1CC', smiles_to_bytes('C1CCCCC1CC')::bytea),
('C1=CC=CC=C1', smiles_to_bytes('C1=CC=CC=C1')::bytea),
('C1=CC=CC=C1CCCCN', smiles_to_bytes('C1=CC=CC=C1CCCCN')::bytea),
('C1=CC=CC=C1CC(CCCC)CC', smiles_to_bytes('C1=CC=CC=C1CC(CCCC)CC')::bytea);



SELECT * from public.mol_table
WHERE is_substructure(smarts_to_bytes('CCC'), mol_bytes);
