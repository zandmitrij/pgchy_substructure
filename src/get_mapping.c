#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <includes/mol.h>
#include <includes/query.h>
#include <includes/utils.h>
#include <includes/get_mapping.h>



int get_mapping(QMol *query, OMol *mol) {
    // expected less than 2^16 atoms in structure.

    uint32_t path_size = 0;
    uint32_t q_size_dec = query->size - 1;;
    uint32_t depth;
    uint32_t front;
    uint32_t back;
    uint32_t closures_num;

    uint32_t n, m, o, i = 0, j = 0;
    uint32_t mol_index;
    uint32_t closures_counter;
    uint64_t q_mask1, q_mask2, q_mask3, q_mask4;
    uint64_t o_bond, c_bond;

    uint32_t *path = (uint32_t*) malloc(q_size_dec * sizeof(uint32_t));
    uint32_t *stack_index = (uint32_t*) malloc(2 * mol->size * sizeof(uint32_t));
    uint32_t *stack_depth = (uint32_t*) malloc(2 * mol->size * sizeof(uint32_t));
    bool *matched = (bool*)malloc(mol->size * sizeof(bool));
    uint64_t *o_closures = (uint64_t*) malloc(mol->size * sizeof(uint64_t));

    if (path == NULL || stack_index == NULL || stack_depth == NULL || matched == NULL || o_closures == NULL) {
        free(path);
        free(stack_index);
        free(stack_depth);
        free(matched);
        free(o_closures);
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    memset(matched, 0, mol->size * sizeof(bool));
    memset(o_closures, 0, mol->size * sizeof(uint64_t));

    q_mask1 = query->masks1[0];
    q_mask2 = query->masks2[0];
    q_mask3 = query->masks3[0];
    q_mask4 = query->masks4[0];

    uint32_t stack = 0;
    for (n = 0; n < mol->size; n++) {
        if (
            mol->scope[n] &&
            (q_mask1 & mol->bits1[n]) &&
            ((q_mask2 & mol->bits2[n]) == mol->bits2[n]) &&
            ((q_mask3 & mol->bits3[n]) == mol->bits3[n]) &&
            (q_mask4 & mol->bits4[n])
        ) {
            stack_index[stack] = n;
            stack_depth[stack] = 0;
            stack++;
        }
    }

    while (stack > 0) {
        stack--;
        depth = stack_depth[stack];
        n = stack_index[stack];

        if (depth == q_size_dec) {
            free(path);
            free(matched);
            free(stack_index);
            free(stack_depth);
            free(o_closures);
            return 1;
        }
        // dead end reached
        if (path_size != depth) {
            for (i = depth; i < path_size; i++) {
                // mark unmatched
                matched[path[i]] = false; 
            }
            path_size = depth;
        }
        
        matched[n] = true;
        path[path_size] = n;
        path_size += 1;

        front = depth + 1;
        back = query->back[front];
        // branch
        if (back != depth) {
            n = path[back];
        }

        // load next query atom
        q_mask1 = query->masks1[front];
        q_mask2 = query->masks2[front];
        q_mask3 = query->masks3[front];
        q_mask4 = query->masks4[front];
        closures_num = query->closures[front];

        for (i = mol->from[n]; i < mol->to[n]; i++) {
            o_bond = mol->bonds[i];
            m = mol->indices[i];
            // bond order, in ring mark and atom bit should match.
            if (
                mol->scope[m] &&
                !matched[m] &&
                ((q_mask1 & o_bond) == o_bond) &&  
                ((q_mask2 & mol->bits2[m]) == mol->bits2[m]) &&
                ((q_mask3 & mol->bits3[m]) == mol->bits3[m]) &&
                (q_mask4 & mol->bits4[m])
            ) {
                //  candidate atom should have same closures.
                if (closures_num != 0) {
                    closures_counter = 0;
                    // make a map of closures for o_n atom
                    // an index is a neighbor atom and a value is a bond between o_n and the neighbor
                    for (j = mol->from[m]; j < mol->to[m]; j++) {
                        // TODO: replace to mol_idx
                        o = mol->indices[j];
                        if (o != n && matched[o]) {
                            o_closures[o] = mol->bonds[j];
                            closures_counter ++;
                        }
                    }
                    if (closures_counter == closures_num) {
                        int flag = 0;
                        for (j = query->from[front]; j < query->to[front]; j++) {
                            c_bond = o_closures[path[query->indices[j]]];
                            // compare order and ring bits
                            if (c_bond == 0 || ((query->bonds[j] & c_bond) != c_bond)) {
                                flag = 1;
                                break;
                            }
                        }
                        if (flag == 0) {
                            stack_index[stack] = m;
                            stack_depth[stack] = front;
                            stack++;
                        }
                    }     
                    
                    // fill an array with zeroes
                    for (j = mol->from[m]; j < mol->to[m]; j++) {
                        o_closures[mol->indices[j]] = 0;
                    }
                } else {
                    // candidate atom should not have closures.
                    int flag = 0;
                    for (j = mol->from[m]; j < mol->to[m]; j++) {
                        o = mol->indices[j];
                        if (o != n && matched[o]) {
                            flag = 1;
                            break;  // found closure
                        }
                    }
                    if (flag == 0) {
                        stack_index[stack] = m;
                        stack_depth[stack] = front;
                        stack += 1;
                    }
                }
            }
        }
    }
    free(path);
    free(matched);
    free(stack_index);
    free(stack_depth);
    free(o_closures);
    return 0;
}
