#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <mol.h>
#include <query.h>
#include <get_mapping.h>
#include <utils.h>


int get_mapping(QMol *query, OMol *mol) {
    // expected less than 2^16 atoms in structure.

    uint32_t path_size = 0;
    uint32_t q_size_dec = query->atoms_count - 1;;
    uint32_t depth;
    uint32_t front;
    uint32_t back;
    uint32_t closures_num;

    uint32_t n, m, i = 0, j = 0;
    uint32_t closures_counter;
    uint64_t o_bond, c_bond;

    MemoryBuffer buf = allocate_buffers(q_size_dec, mol->atoms_count);

    uint64_t q_mask1 = query->atoms[0].mask1;
    uint64_t q_mask2 = query->atoms[0].mask2;
    uint64_t q_mask3 = query->atoms[0].mask3;
    uint64_t q_mask4 = query->atoms[0].mask4;

    uint32_t stack = 0;

    for (n = 0; n < mol->atoms_count; n++) {
        OAtom atom = mol->atoms[n];
        if (
            (q_mask1 & atom.bits1) &&
            ((q_mask2 & atom.bits2) == atom.bits2) &&
            ((q_mask3 & atom.bits3) == atom.bits3) &&
            (q_mask4 & atom.bits4)
        ) {
            buf.stack_index[stack] = n;
            buf.stack_depth[stack] = 0;
            stack++;
        }
    }

    while (stack > 0) {
        stack--;
        depth = buf.stack_depth[stack];
        n = buf.stack_index[stack];

        // substructure found
        if (depth == q_size_dec) {
            free_buffers(&buf);
            return 1;
        }

        // dead end reached
        if (path_size != depth) {
            for (i = depth; i < path_size; i++) {
                // mark unmatched
                buf.matched[buf.path[i]] = false; 
            }
            path_size = depth;
        }

        buf.matched[n] = true;
        buf.path[path_size] = n;
        path_size += 1;

        front = depth + 1;
        back = query->atoms[front].back;
        // branch
        if (back != depth) {
            n = buf.path[back];
        }

        // load next query atom
        q_mask1 = query->atoms[front].mask1;
        q_mask2 = query->atoms[front].mask2;
        q_mask3 = query->atoms[front].mask3;
        q_mask4 = query->atoms[front].mask4;
        closures_num = query->atoms[front].closure;

        for (i = mol->atoms[n].from; i < mol->atoms[n].to; i++) {
            o_bond = mol->bonds[i].bond;
            m = mol->bonds[i].index;
            // bond order, in ring mark and atom bit should match.
            OAtom atom = mol->atoms[m];
            if (
                !buf.matched[m] &&
                ((q_mask1 & o_bond) == o_bond) &&  
                ((q_mask2 & atom.bits2) == atom.bits2) &&
                ((q_mask3 & atom.bits3) == atom.bits3) &&
                (q_mask4 & atom.bits4)
            ) {
                //  candidate atom should have same closures.
                if (closures_num != 0) {
                    closures_counter = 0;
                    // make a map of closures for o_n atom
                    // an index is a neighbor atom and a value is a bond between o_n and the neighbor
                    
                    for (j = atom.from; j < atom.to; j++) {
                        uint32_t bond_index = mol->bonds[j].index;
                        if (bond_index != n && buf.matched[bond_index]) {
                            buf.o_closures[bond_index] = mol->bonds[j].bond;
                            closures_counter++;
                        }
                    }
                    if (closures_counter == closures_num) {
                        int flag = 0;
                        for (j = query->atoms[front].from; j < query->atoms[front].to; j++) {
                            c_bond = buf.o_closures[buf.path[query->bonds[j].index]];
                            // compare order and ring bits
                            if (c_bond == 0 || ((query->bonds[j].bond & c_bond) != c_bond)) {
                                flag = 1;
                                break;
                            }
                        }
                        if (flag == 0) {
                            buf.stack_index[stack] = m;
                            buf.stack_depth[stack] = front;
                            stack++;
                        }
                    }
                    // fill an array with zeroes
                    for (j = atom.from; j < atom.to; j++) {
                        buf.o_closures[mol->bonds[j].index] = 0;
                    }
                } else {
                    // candidate atom should not have closures.
                    int flag = 0;
                    for (j = atom.from; j < atom.to; j++) {
                        uint32_t bond_index = mol->bonds[j].index;
                        if (bond_index != n && buf.matched[bond_index]) {
                            flag = 1;
                            break;  // found closure
                        }
                    }
                    if (flag == 0) {
                        buf.stack_index[stack] = m;
                        buf.stack_depth[stack] = front;
                        stack += 1;
                    }
                }
            }
        }
    }
    free_buffers(&buf);
    return 0;
}
