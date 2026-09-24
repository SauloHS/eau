#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "limine.h"
#include "drivers/serial/com1.h"


// Defines limine protocol revision 6
__attribute__((used, section(".limine_requests")))
LIMINE_BASE_REVISION(6);

// Framebuffer request
__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
  .id = LIMINE_FRAMEBUFFER_REQUEST,
  .revision = 0
};


// kernel entry point
void kmain(void) {
  serial_init();
  serial_write("EauOS booting\n");
  if (framebuffer_request.response == NULL) {
    // no response from framebuffer request; halt here
    for (;;) {
      asm("hlt");
    }
  }

  serial_write("Framebuffer response NOT NULL\n");
  
  // getting only first framebuffer
  // FIXME: this only gets first framebuffer; if there is more than one monitor, probably smt will go wrong
  struct limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];

  // this ain't gonna stay here for a long time
  for (uint64_t y = 0; y < fb->height; y++) {
    for (uint64_t x = 0; x < fb->width; x++) {
      // fancy math to discover pixel pos given a x and an y
      uint32_t *pixel = (uint32_t *)((uint8_t *)fb->address + y * fb->pitch + x * (fb->bpp / 8));
      *pixel = 0x0000FF;
    }
  }

  serial_write("Screen should be blue\n");
  
  // nothing more to do
  for (;;) {
    asm ("hlt");
  }
}
