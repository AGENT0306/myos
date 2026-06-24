//
// Created by reit on 6/12/26.
//
#include "pmm.h"

#include "limine.h"
#include "framebuffer.h"
#include "memory.h"

static uint8_t *bitmap;
static uint64_t bitmap_frames;
static uint64_t bitmap_size;
static uint64_t hhdm_offset;
static uint64_t free_frames;
static uint64_t total_frames;

static void bitmap_clear(uint64_t frame){
    if ((bitmap[frame / 8] & (1 << (frame % 8))) != 0)
    {
        bitmap[frame / 8] &= ~(1 << (frame % 8));
        free_frames++;
    }
}

static void bitmap_set(uint64_t frame)
{
    if ((bitmap[frame / 8] & (1 << (frame % 8))) == 0)
    {
        bitmap[frame / 8] |= (1 << (frame % 8));
        free_frames--;
    }
}

void pmm_init(volatile struct limine_memmap_request *memmap_request, volatile struct limine_hhdm_request *hhdm_request) {

    struct limine_memmap_response *memmap_response = memmap_request->response;

    hhdm_offset = hhdm_request->response->offset;

    uint64_t top = 0;

    // Loops through limine memmap_response entries to find highest usable physical memory address
    for (uint64_t i = 0; i < memmap_response->entry_count; i++) {
        uint64_t ebase = memmap_response->entries[i]->base;
        uint64_t endOfEntry = ebase + memmap_response->entries[i]->length;

        if (memmap_response->entries[i]->type == LIMINE_MEMMAP_USABLE) {
            if (endOfEntry > top){
                top = endOfEntry;
            }
        }
    }

    // Determine bitmap paras from given the highest address
    uint64_t bitmapAddr = 0;
    bitmap_frames = top / 4096;
    bitmap_size = (bitmap_frames + 7) / 8;

    // Loop through entries again to find first entry large enough to store bitmap
    for (uint64_t i = 0; i < memmap_response->entry_count; i++) {
        uint64_t ebase = memmap_response->entries[i]->base;

        if (memmap_response->entries[i]->type == LIMINE_MEMMAP_USABLE && memmap_response->entries[i]->length >= bitmap_size) {
                bitmapAddr = ebase;
                break;
        }
    }

    // Check to make sure we actually found an address to store bitmap at
    if (bitmapAddr != 0) {
        kprint("Bit Map Address Found!\n");
    }else {
        kprint("ERROR!!! BIT MAP ADDRESS NOT FOUND!");

    }

    bitmap = (uint8_t *)(bitmapAddr + hhdm_offset);
    memset(bitmap, 0xFF, bitmap_size);

    kprint("Bitmap memory set!\n");

    kprint_hex(bitmapAddr);
    kprint("\n");

    for (uint64_t i = 0; i < memmap_response->entry_count; i++){
        if (memmap_response->entries[i]->type != LIMINE_MEMMAP_USABLE)
            continue;

        uint64_t ebase = memmap_response->entries[i]->base;
        uint64_t elength = memmap_response->entries[i]->length;

        uint64_t start_frame = ebase / 4096;
        uint64_t end_frame = (ebase + elength) / 4096;

        for (uint64_t j = start_frame; j < end_frame; j++){
            total_frames++;
            bitmap_clear(j);
        }
    }

    uint64_t bitmap_str_frame = bitmapAddr / 4096;
    uint64_t bitmap_end_frame = (bitmapAddr + bitmap_size + 4095) / 4096;

    for (uint64_t j = bitmap_str_frame; j < bitmap_end_frame; j++){
        bitmap_set(j);
    }

    uint64_t usable_memory_maybe_idk = free_frames * 4096 / 1024 / 1024;

    kprint_num(usable_memory_maybe_idk);

}
