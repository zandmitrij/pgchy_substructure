import argparse

from . import mol
from . import query
from . import tests


def arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    
    subparsers = parser.add_subparsers(dest='command')

    smiles = subparsers.add_parser('smiles', help='generate bytes struct for SMILES')
    smiles.add_argument('smiles', type=str, help='SMILES to generate bytes')
    smiles.add_argument('--output', '-o', help='output file', default='./molbytes')

    smarts = subparsers.add_parser('smarts', help='generate bytes struct for SMARTS')
    smarts.add_argument('smarts', type=str, help='SMARTS to generate bytes')
    smarts.add_argument('--output', '-o', help='output file', default='./querybytes')

    smiles_checker = subparsers.add_parser('check_smiles', help='testing c program')
    smiles_checker.add_argument('smiles', type=str, help='SMILES check if reading is correct')
    smiles_checker.add_argument('--executable', type=str, help='binary c file', required=True)

    smarts_checker = subparsers.add_parser('check_smarts', help='testing c program')
    smarts_checker.add_argument('smarts', type=str, help='SMARTS check if reading is correct')
    smarts_checker.add_argument('--executable', type=str, help='binary c file', required=True)

    mapping_checker = subparsers.add_parser('check_mapping', help='testing c program')
    mapping_checker.add_argument('--executable', type=str, help='binary c file', required=True)
    mapping_checker.add_argument('--res', type=int, help='binary c file', required=True)

    return parser.parse_args()


def main():
    args = arguments()

    if args.command == 'smiles':
        mol.save(args.smiles, args.output)

    elif args.command == 'smarts':
        query.save(args.smarts, args.output)

    elif args.command == 'check_smiles':
        tests.check_smiles_read(args.executable, args.smiles)

    elif args.command == 'check_smarts':
        tests.check_smarts_read(args.executable, args.smarts)

    elif args.command == 'check_mapping':
        tests.check_mapping(args.executable, args.res)
        

if __name__ == '__main__':
    main()
