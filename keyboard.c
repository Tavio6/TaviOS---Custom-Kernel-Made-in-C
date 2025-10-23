#include <stdint.h>
#include "keyboard.h"

//read a byte from an I/O port
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static const char scancode_table[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0, '\\','z','x',
    'c','v','b','n','m',',','.','/', 0, '*', 0, ' ', 0, 0, 0, 0, 0,
};

char get_key_poll(void) {
    while ((inb(0x64) & 1) == 0);
    uint8_t scancode = inb(0x60);
    if (scancode & 0x80) {
        return 0;
    } else {
        return scancode_table[scancode];
    }
}