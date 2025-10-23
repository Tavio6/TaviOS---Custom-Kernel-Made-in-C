#include "tty.h"
#include "keyboard.h"
#include "cursor.h"

#define VGA_MEM        ((volatile uint8_t*)0xB8000)
#define VGA_SIZE       (80 * 25)
#define VGA_ATTR       0x07

void kernel_main(void) {
    for (size_t i = 0; i < VGA_SIZE; i++) {
        VGA_MEM[i * 2]     = ' ';
        VGA_MEM[i * 2 + 1] = VGA_ATTR;
    }

    enable_cursor(0, 15);//from cursor
    update_cursor(0);//from cursor

    print("Hello World from Tavi!\n");
    print("boot                 LOADED\n");
    print("kernel               LOADED\n");
    print("\nwrite anything below\n\n");

    for (;;) {
        char c = get_key_poll();//from keyboard
        if (c) {
            putchar(c);//from tty
        }
    }
}