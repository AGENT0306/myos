//
// Created by reit on 6/12/26.
//

#ifndef MYOS_PMM_H
#define MYOS_PMM_H

#include "limine.h"
#include <stdint.h>

void pmm_init(volatile struct limine_memmap_request *memmap_request, volatile struct limine_hhdm_request *hhdm_request);

uint64_t pmm_alloc(void);
void pmm_free(uint64_t addr);

#endif //MYOS_PMM_H
