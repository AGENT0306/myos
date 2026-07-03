//
// Created by Rei Trebicka on 6/26/2026.
//

#include "vmm.h"

#include "framebuffer.h"
#include "memory.h"

uint64_t hhdm_off;
addr_space_t addr_space;

void vmm_init(uint64_t hhdm_offset){
    hhdm_off = hhdm_offset;

    addr_space.pml4 = (uint64_t *)(pmm_alloc() + hhdm_offset);
    memset(addr_space.pml4, 0, 4096);
}

treeidx_t find_levels(uint64_t va) {
    treeidx_t levels;

    levels.pml4i = (va >> 39) & 0x1FF;
    levels.pdpti = (va >> 30) & 0x1FF;
    levels.pdi = (va >> 21) & 0x1FF;
    levels.pti = (va >> 12) & 0x1FF;

    return levels;
}
