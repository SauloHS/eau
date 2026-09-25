#include <stdint.h>

struct gdt_entry {
  uint16_t limit_low; // ignored
  uint16_t base_low; //ignored
  uint8_t base_mid; // ignored
  uint8_t access;   
  uint8_t granularity;
  uint8_t base_high; // ignored
} __attribute__((packed));

struct gdt_entry entries[3] = {
  { 0 },
  {
    .access = 0x9A,
    .granularity = 0x20,
  },
  {
    .access = 0x92
  }
};

struct gdt_descriptor {
  uint16_t limit; // table size - 1;
  uint64_t base; // table memory address
} __attribute__((packed));

struct gdt_descriptor gdtr = {
  .limit = sizeof(entries) - 1,
  .base = (uint64_t)&entries
};

void gdt_load(void) {
  asm volatile ("lgdt %0" : : "m"(gdtr));
}

void gdt_reload_segments(void) {
    asm volatile (
        "push $0x08 \n"
        "lea 1f(%%rip), %%rax \n"
        "push %%rax \n"
        "lretq \n"
        "1: \n"
        "mov $0x10, %%ax \n"
        "mov %%ax, %%ds \n"
        "mov %%ax, %%es \n"
        "mov %%ax, %%fs \n"
        "mov %%ax, %%gs \n"
        "mov %%ax, %%ss \n"
        : : : "rax"
    );
}
