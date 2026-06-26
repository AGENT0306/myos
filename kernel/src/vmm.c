//
// Created by Rei Trebicka on 6/26/2026.
//

#include "vmm.h"

uint64_t hhdm_off;

void vmm_init(uint64_t hhdm_offset){
    hhdm_off = hhdm_offset;
}
