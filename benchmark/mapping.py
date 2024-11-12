from chython.algorithms._isomorphism import get_mapping


def mapping(query: list, mol: list):
    return next(get_mapping(*query, *mol))
