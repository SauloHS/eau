// defines inb and outb functions
#include "iob.h"
#include <stdint.h>

void outb(uint16_t port, uint8_t value) {
  asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port) {
  uint8_t ret;
  asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
}

// 1 microsecond delay
void io_wait(void) {
  outb(0x80, 0);
}
