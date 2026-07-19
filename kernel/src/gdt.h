//
// Created by reitr on 7/19/2026.
//

#ifndef MYOS_GDT_H
#define MYOS_GDT_H

#include <stdint.h>


typedef struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t limit_high_flags;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct tss_descriptor {
    gdt_entry_t entry;
    uint32_t base_upper;
    uint32_t reserved;
} __attribute__((packed)) tss_descriptor_t;

#endif //MYOS_GDT_H
