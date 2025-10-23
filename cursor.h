#ifndef CURSOR_H
#define CURSOR_H

#include <stddef.h>
#include <stdint.h>

//enable the VGA cursor
void enable_cursor(uint8_t start, uint8_t end);

//update VGA hardware cursor position
void update_cursor(size_t pos);

#endif