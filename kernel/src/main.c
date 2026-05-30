#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <memory.h>
#include <framebuffer.h>

// Set the base revision to 6, this is recommended as this is the latest
// base revision described by the Limine boot protocol specifcation.

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] =
LIMINE_BASE_REVISION(6);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request
framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] = 
LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] = 
LIMINE_REQUESTS_END_MARKER;

static void hcf(void) {
    for(;;) {
        asm("hlt");
    }
}

void kmain(void){
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
        hcf();
    }

    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1){
        hcf();
    }

    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    fb_init(framebuffer);

    kprint("Hello from myos!!!\n");
    kprint("MY PRINT FUNCTION WORKSSSS!!!\n");

    hcf();
}
