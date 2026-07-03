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

void map_page(uint64_t frame, addr_space_t aspc, uint64_t vaddr, uint16_t flags) {
    treeidx_t treelvls = find_levels(vaddr);

    uint64_t pdpt_addr = aspc.pml4[treelvls.pml4i];
    if (pdpt_addr == 0) {
        kprint("PLM4 entry doesn't store a PDPT addr\n");
        kprint("Storing now...");
        pdpt_addr = pmm_alloc();
        memset((uint64_t*)(pdpt_addr + hhdm_off), 0, 4096);
        aspc.pml4[treelvls.pml4i] = pdpt_addr;
    }
    uint64_t *pdpt = (uint64_t *)(pdpt_addr + hhdm_off);
}
