#include <framebuffer.h>

static int cursor_x = 0;
static int cursor_y = 0;

static uint32_t *fb_addr;
static uint64_t fb_width;
static uint64_t fb_height;
static uint64_t fb_pitch;

#define FONT_W 8
#define FONT_H 8
#define FG_COLOR 0x00FFFFFF
#define BG_COLOR 0x00000000

void fb_init(struct limine_framebuffer *fb){
    fb_addr = (uint32_t*)fb->address;
    fb_width = fb->width;
    fb_height = fb->height;
    fb_pitch = fb->pitch / 4;
}


static inline void put_pixel(int x, int y, uint32_t color){
    fb_addr[y * fb_pitch + x] = color;
}

static void render_char(char c, int px, int py){
    // clamp to printable ASCII
    if ((unsigned char)c >= 128)c = '?';

    uint8_t *glyph = font8x8_basic[(unsigned char)c];

    for(int row = 0; row < FONT_H; row++){
        for (int col = 0; col < FONT_W; col++){
            // MSB is leftmost pixel
            uint32_t color = (glyph[row] & (1 <<  col)) ? FG_COLOR : BG_COLOR;
            put_pixel(px + col, py + row, color);
        }
    }
}

static void scroll_up(void){
    // Shift everything up by one character row
    uint64_t row_words = fb_pitch * FONT_H;
    uint64_t total = fb_pitch * fb_height;

    for (uint64_t i = 0; i < total - row_words; i++)
        fb_addr[i] = fb_addr[i + row_words];
    
    for (uint64_t i = total - row_words; i < total; i++)
        fb_addr[i] = BG_COLOR;

    cursor_y -= FONT_H;
}

void kprint_hex(uint64_t value){
    char buf[19];           // "0x" + 16 hex digits + '\0'
    const char *digits = "0123456789abcdef";

    buf[0] = '0';
    buf[1] = 'x';
    for (int i = 0; i < 16; i++){
        // fill from the least-significant nibble backwards
        buf[17 - i] = digits[value & 0xF];
        value >>= 4;
    }
    buf[18] = '\n';

    kprint(buf);
}

void kprint(const char *str){
    while (*str){
        char c = *str++;

        if (c == '\n'){
            cursor_x = 0;
            cursor_y += FONT_H;
        } else if (c == '\r') {
            cursor_x = 0;
        } else if (c == '\t') {
            cursor_x = (cursor_x + 4) & ~3;
        } else {
            render_char(c, cursor_x * FONT_W, cursor_y);
            cursor_x++;

            if (cursor_x >= (int)(fb_width / FONT_W)){
                cursor_x = 0;
                cursor_y += FONT_H;
            }
        }

        if (cursor_y + FONT_H > (int)fb_height)
            scroll_up();
    }
}
