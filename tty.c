#include <stdint.h>
#include "tty.h"
#include "cursor.h"

//this script handles all thing related to displaying the characters and handling them
#define VGA_MEM        ((volatile uint8_t*)0xB8000)
#define VGA_WIDTH      80
#define VGA_HEIGHT     25
#define VGA_SIZE       (VGA_WIDTH * VGA_HEIGHT)
#define VGA_ATTR       0x07

static size_t cursor_pos = 0;

static void clear_last_line(void) {
    size_t row = VGA_HEIGHT - 1;
    for (size_t col = 0; col < VGA_WIDTH; col++) {
        size_t idx = row * VGA_WIDTH + col;
        VGA_MEM[idx * 2]     = ' ';
        VGA_MEM[idx * 2 + 1] = VGA_ATTR;
    }
}

static void scroll_if_needed(void) {
    if (cursor_pos < VGA_SIZE) return;
    for (size_t row = 1; row < VGA_HEIGHT; row++) {
        for (size_t col = 0; col < VGA_WIDTH; col++) {
            size_t src = row * VGA_WIDTH + col;
            size_t dst = (row - 1) * VGA_WIDTH + col;
            VGA_MEM[dst * 2]     = VGA_MEM[src * 2];
            VGA_MEM[dst * 2 + 1] = VGA_MEM[src * 2 + 1];
        }
    }
    clear_last_line();
    cursor_pos = (VGA_HEIGHT - 1) * VGA_WIDTH;
}

static void newline(void) {
    cursor_pos = (cursor_pos / VGA_WIDTH + 1) * VGA_WIDTH;
    scroll_if_needed();
}

void putchar(char c) {
    if (c == '\n') {
        newline();
    }
    else if (c == '\b') {
        if (cursor_pos > 0) {
            cursor_pos--;
            VGA_MEM[cursor_pos * 2]     = ' ';
            VGA_MEM[cursor_pos * 2 + 1] = VGA_ATTR;
        }
    }
    else {
        VGA_MEM[cursor_pos * 2]     = (uint8_t)c;
        VGA_MEM[cursor_pos * 2 + 1] = VGA_ATTR;
        cursor_pos++;
    }
    scroll_if_needed();
    update_cursor(cursor_pos);
}

void print(const char* str) {
    while (*str) putchar(*str++);
}