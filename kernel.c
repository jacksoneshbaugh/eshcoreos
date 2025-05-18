// eshcoreos kernel
// &copy; Jackson Eshbaugh, 2025

#include <stdint.h>

#define VGA_ADDRESS 0xb8000
#define WHITE_ON_BLACK 0x0f

/**
 * Entrypoint for the kernel
 */
void kernel_main(void) {


  // Write characters to VGA memory, starting at 0xb8000
  // In VGA memory: 2 bytes per character:
  // byte 0 -> ASCII Character
  // byte 1 -> Attribute
  char *video = (char *) VGA_ADDRESS;
  const char *message = "Hello from kernel!";

  for(int i = 0; message[i] != '\0'; ++i) {
    video[i * 2] = message[i]; // set the character byte
    video[i * 2 + 1] = WHITE_ON_BLACK; // set the attribute byte
  }

  // once done, halt forever
  while(1) __asm__("hlt");


}