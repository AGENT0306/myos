//
// Created by reit on 6/12/26.
//
#include "pmm.h"

#include "limine.h"
#include "framebuffer.h"

static uint8_t *bitmap;
static uint64_t bitmap_frames;
static uint64_t bitmap_size;
static uint64_t hhdm_offset;

void pmm_init(struct limine_memmap_request *memmap_request, struct limine_hhdm_request *hhdm_request) {

    struct limine_memmap_response *memmap_response = memmap_request->response;

    hhdm_offset = hhdm_request->response->offset;

    uint64_t top = 0;

    bool spaceForBitmap = false;

    for (uint64_t i = 0; i < memmap_response->entry_count; i++) {
        uint64_t ebase = memmap_response->entries[i]->base;
        uint64_t endOfEntry = ebase + memmap_response->entries[i]->length;

        if (!spaceForBitmap) {

        }

        if (memmap_response->entries[i]->type == LIMINE_MEMMAP_USABLE) {
            top = endOfEntry;
        }
    }

    bitmap_frames = top / 4096;
    bitmap_size = (bitmap_frames + 7) / 8;


}
