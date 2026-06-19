//
// Created by reit on 6/12/26.
//

#ifndef MYOS_PMM_H
#define MYOS_PMM_H

#pragma once

#include <limine.h>
#include <stdint-gcc.h>

void pmm_init(struct limine_memmap_request *memmap_request, struct limine_hhdm_request *hhdm_request);

#endif //MYOS_PMM_H
