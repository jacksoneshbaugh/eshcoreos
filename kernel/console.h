// Console drivers for eshcoreos
// © Jackson Eshbaugh, 2025

#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>

/**
 * VGA color codes (4-bit values).
 * Can be used for foreground or background colors.
 */
typedef enum {
    COLOR_BLACK = 0x0,
    COLOR_BLUE = 0x1,
    COLOR_GREEN = 0x2,
    COLOR_CYAN = 0x3,
    COLOR_RED = 0x4,
    COLOR_MAGENTA = 0x5,
    COLOR_BROWN = 0x6,
    COLOR_LIGHT_GRAY = 0x7,
    COLOR_DARK_GRAY = 0x8,
    COLOR_LIGHT_BLUE = 0x9,
    COLOR_LIGHT_GREEN = 0xA,
    COLOR_LIGHT_CYAN = 0xB,
    COLOR_LIGHT_RED = 0xC,
    COLOR_PINK = 0xD,
    COLOR_YELLOW = 0xE,
    COLOR_WHITE = 0xF
} vga_color_t;

/**
 * Clears the entire screen and resets the cursor to the top-left corner.
 */
void clearScreen(void);

/**
 * Writes a single character to the screen at the current cursor position.
 * Automatically advances the cursor and handles newlines.
 *
 * @param c The character to write.
 */
void putChar(char c);

/**
 * Writes a null-terminated string to the screen, starting at the current cursor position.
 *
 * @param str The string to write.
 */
void print(const char* str);

/**
 * Sets the color for all subsequent text output.
 *
 * @param blink      If nonzero, enables the blink bit (may disable high background colors).
 * @param foreground The foreground (text) color.
 * @param background The background color (only 3 bits are used).
 */
void setColor(uint8_t blink, vga_color_t foreground, vga_color_t background);

/**
 * Moves the hardware cursor to a specific screen position.
 */
void moveCursor(uint8_t row, uint8_t col);

/**
 * Enables the VGA hardware cursor.
 */
void enableCursor(uint8_t start_scanline, uint8_t end_scanline);

/**
 * Disables the VGA hardware cursor.
 */
void disableCursor(void);

#endif // CONSOLE_H