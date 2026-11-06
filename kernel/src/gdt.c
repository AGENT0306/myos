//
// Created by reitr on 7/19/2026.
//

#include "include.h"

void gdt_init(){
    //Fill in 


    //Here we will init each of the six entrys for the gdt

    for(int i = 0; i < 5; i++){
      
    }
}


void encodeGdtEntry(gdt_entry_t * entry, uint64_t base, uint32_t limit /* is only 20 bits not 32 */, uint8_t accs, uint8_t flg){
    //encode base
    entry->base_one = base & 0xFFFF;
    entry->base_two = (base >> 16) & 0xFF;
    entry->base_three = (base >> 24) & 0xFF;
    entry->base_four = (base >> 32) & 0xFFFFFFFF;

    //encode limit
    entry->limit_low = limit & 0xFFFF;
    // I don't think this works, i believe the second limit_high_flags would override the first but too lazy to fix rn
    entry->limit_high_flags = (limit >> 16) & 0x0F;
    entry->limit_high_flags = flg & 0xF;

    //encode access
    entry->access = accs & 0xFF;
}
