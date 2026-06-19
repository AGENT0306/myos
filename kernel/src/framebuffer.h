#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <limine.h>
#include <stdint.h>
#include <stddef.h>
#include <font8x8_basic.h>

void fb_init();
void kprint(const char *str);
void kprint_hex(uint64_t value);

#endif
