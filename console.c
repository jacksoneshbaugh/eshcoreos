// Console drivers for eshcoreos
// &copy; Jackson Eshbaugh, 2025

#include "console.h"
#include "utils.h"

#define VGA_ADDRESS 0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t* const vga = (uint16_t*) VGA_ADDRESS;
static int row = 0;
static int col = 0;
static uint8_t color = 0x07;

void setColor(uint8_t blink, vga_color_t foreground, vga_color_t background) {
  // each color parameter is 4 bits (i.e., one hex digit)
  color = ((blink & 1) << 7) | (background << 4) | (foreground & 0x0f);
}

void clearScreen(void) {

  for(int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
    // fill the screen with ' ' colored the set color.
    vga[i] = (color << 8) | ' ';
  }

  // reset row and column
  row = 0;
  col = 0;

}

void putChar(char c) {
  if(c == '\n') {
    // new line
    col = 0;
    row++;
  } else {
    // have an actual character to print to the screen
    vga[row * VGA_WIDTH + col] = (color << 8) | c;
    col++;
    if(col >= VGA_WIDTH) {
      col = 0;
      row++;
    }

    if(row >= VGA_HEIGHT) {
      // TODO: Implement scrolling later
      row = 0;
    }
  }
}

void print(const char* str) {
  // until we reach '\0', print each character
  while(*str) putChar(*str++);
}

void moveCursor(uint8_t row, uint8_t col) {
    uint16_t pos = row * 80 + col;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void enableCursor(uint8_t start_scanline, uint8_t end_scanline) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | (start_scanline & 0x1F));

    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | (end_scanline & 0x1F));
}

void disableCursor(void) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);  // Set cursor start bit 5 (bit 5 = disable)
}

