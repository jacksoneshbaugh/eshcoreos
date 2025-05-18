// eshcoreos kernel
// &copy; Jackson Eshbaugh, 2025

#include <stdint.h>
#include "console.h"

/**
 * Entrypoint for the kernel
 */
void kernel_main(void) {

  disableCursor();

  setColor(0, COLOR_LIGHT_CYAN, COLOR_BLACK);
  clearScreen();
  print("          =================== Welcome to eshcoreos ===================\n");

  // once done, halt forever
  while(1) __asm__("hlt");


}