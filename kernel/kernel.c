// eshcoreos kernel
// &copy; Jackson Eshbaugh, 2025

#include <stdint.h>
#include "console.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"

static void timer_debug_hook(uint32_t ticks) {
  // Cheap heartbeat while bringing up IRQ0: one dot per second at 100 Hz.
  if (ticks % 100 == 0) print(".");
}

/**
 * Entrypoint for the kernel
 */
void kernel_main(void) {

  disableCursor();

  setColor(0, COLOR_LIGHT_CYAN, COLOR_BLACK);
  clearScreen();
  print("          =================== Welcome to eshcoreos ===================\n");

  /*
   * Interrupt setup order matters:
   *   1. Load IDT entries for CPU exceptions and remapped IRQs.
   *   2. Remap the PIC so hardware IRQs use vectors 32-47.
   *   3. Program the PIT and register its IRQ0 handler.
   *   4. Enable maskable interrupts with sti.
   */

  idt_init();
  pic_remap();
  pit_init(100);

  setColor(0, COLOR_WHITE, COLOR_BLACK);
  timer_register_hook(timer_debug_hook);

  __asm__ volatile ("sti");


#ifdef TEST_EXCEPTION
  __asm__ volatile ("int $0");
#endif

  // once done, halt forever
  while(1) __asm__("hlt");


}
