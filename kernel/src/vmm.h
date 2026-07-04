//
// Created by Rei Trebicka on 6/26/2026.
//

#ifndef MYOS_VMM_H
#define MYOS_VMM_H

#include <stdint.h>
#include "pmm.h"

typedef struct addr_space{
    uint64_t *pml4;
}addr_space_t;

// Want to change so that I can use loops
typedef struct treeidx {
    uint16_t pml4i;
    uint16_t pdpti;
    uint16_t pdi;
    uint16_t pti;

}treeidx_t;

void vmm_init(uint64_t hhdm_offset);
void map_page(uint64_t frame, addr_space_t pml4, uint64_t vaddr, uint64_t flags);
void unmap_page(uint64_t pml4, uint64_t vaddr);

#endif //MYOS_VMM_H
