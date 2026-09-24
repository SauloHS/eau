#include <stdint.h>
#include <stddef.h>
#include "utils/iob.h"

#define COM1_PORT 0x3F8

void serial_init(void) {
  outb(COM1_PORT + 1, 0x00);
  outb(COM1_PORT + 3, 0x80);
  outb(COM1_PORT + 0, 0x03);
  outb(COM1_PORT + 1, 0x00);
  outb(COM1_PORT + 3, 0x03);
  outb(COM1_PORT + 2, 0xC7);
  outb(COM1_PORT + 4, 0x0B);
}

int serial_is_transmit_empty(void) {
  return inb(COM1_PORT + 5) & 0x20;
}

void serial_putc(char c) {
  while (serial_is_transmit_empty() == 0) {
    // wait
  }

  outb(COM1_PORT, c);
}

void serial_write(const char *str) {
  for (size_t i = 0; str[i] != '\0'; i++) {
    serial_putc(str[i]);
  }
}
