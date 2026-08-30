//
// Created by reitr on 7/19/2026.
//

#ifndef MYOS_GDT_H
#define MYOS_GDT_H

#include <stdint.h>


typedef struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_one;
    uint8_t base_two;
    uint8_t access;
    // becuase limit_high and flags are only 4 bits need to make sure packed removes the extra
    // four bits from each of these attrs.
    uint8_t limit_high;
    uint8_t flags;
    uint8_t base_three;
    uint32_t base_four; 
} __attribute__((packed)) gdt_entry_t;

typedef struct gdt{
    gdt_entry_t null;
    gdt_entry_t kernel_code;
    gdt_entry_t kernel_data;
    gdt_entry_t user_data;
    gdt_entry_t user_code;
    gdt_entry_t tss; // I think this is wrong but I'll check later
} __attribute__((packed)) gdt_t;

typedef struct tss_descriptor {
    gdt_entry_t entry;
    uint32_t base_upper;
    uint32_t reserved;
} __attribute__((packed)) tss_descriptor_t;

#endif //MYOS_GDT_H
