//
// Created by reitr on 7/19/2026.
//

#include "gdt.h"

void encodeGdtEntry(gdt_entry_t * entry, uint64_t base, uint32_t limit /* is only 20 bits not 32 */ ){
    //encode base
    entry->base_one = base & 0xFFFF;
    entry->base_two = (base >> 16) & 0xFF;
    entry->base_three = (base >> 24) & 0xFF;
    entry->base_four = (base >> 32) & 0xFFFFFFFF;

    //encode limit
    
}
