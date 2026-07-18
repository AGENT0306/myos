//
// Created by Rei Trebicka on 6/26/2026.
//

#include "include.h"

uint64_t hhdm_off;

addr_space_t addr_space;

void vmm_init(volatile struct limine_memmap_request *memmap_request, volatile struct limine_hhdm_request *hhdm_request){
    hhdm_off = hhdm_request->response->offset;
    struct limine_memmap_response *mem_res = memmap_request->response;

    uint64_t pml4_phys = pmm_alloc();
    addr_space.pml4 = (uint64_t *)(pml4_phys + hhdm_off);
    memset(addr_space.pml4, 0, 4096);

    // Need to point CR3 to PML4 addr
    kprint("Setting CR3 to PLM4 addr...\n");
    asm("movq %0, %%cr3": : "r" (pml4_phys): "memory");

    //Map all frames to PML4 tree

    for (uint64_t i = 0; i < mem_res->entry_count; i++) {
        if (mem_res->entries[i]->type == LIMINE_MEMMAP_USABLE) {

        }
    }
}

treeidx_t find_levels(uint64_t va) {
    treeidx_t levels;

    levels.pml4i = (va >> 39) & 0x1FF;
    levels.pdpti = (va >> 30) & 0x1FF;
    levels.pdi = (va >> 21) & 0x1FF;
    levels.pti = (va >> 12) & 0x1FF;

    return levels;
}

void map_page(uint64_t paddr, addr_space_t *aspc, uint64_t vaddr, uint64_t flags) {
    treeidx_t treelvls = find_levels(vaddr);

    uint64_t pdpt_addr = aspc->pml4[treelvls.pml4i];
    if (pdpt_addr == 0) {
        kprint("PLM4 entry doesn't store a PDPT addr\n");
        kprint("Storing now...\n");
        pdpt_addr = pmm_alloc();
        memset((uint64_t*)(pdpt_addr + hhdm_off), 0, 4096);
        aspc->pml4[treelvls.pml4i] = pdpt_addr;
        kprint("PDPT addr stored!\n");
    }
    uint64_t *pdpt = (uint64_t *)(pdpt_addr + hhdm_off);

    uint64_t pd_addr = pdpt[treelvls.pdpti];
    if (pd_addr == 0) {
        kprint("PDPT entry doesn't store a PD addr\n");
        kprint("Storing now...\n");
        pd_addr = pmm_alloc();
        memset((uint64_t*)(pd_addr + hhdm_off), 0, 4096);
        pdpt[treelvls.pdpti] = pd_addr;
        kprint("PD addr stored!\n");
    }
    uint64_t *pd = (uint64_t *)(pd_addr + hhdm_off);

    uint64_t pt_addr = pd[treelvls.pdi];
    if (pt_addr == 0) {
        kprint("PD entry doesn't store a PT addr\n");
        kprint("Storing now...\n");
        pt_addr = pmm_alloc();
        memset((uint64_t*)(pt_addr + hhdm_off), 0, 4096);
        pd[treelvls.pdi] = pt_addr;
        kprint("PT addr stored!\n");
    }
    uint64_t *pt = (uint64_t *)(pt_addr + hhdm_off);

    uint64_t page_frame = pt[treelvls.pti];
    if (page_frame == 0) {
        kprint("PT doesn't store a frame\n");
        kprint("Storing now...\n");
        page_frame = paddr | (flags | 0x1);
        pt[treelvls.pti] = page_frame;
        kprint("Frame stored!\n");
    }
}
