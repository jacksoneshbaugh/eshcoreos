// I/O port helpers for eshcoreos
// &copy; Jackson Eshbaugh, 2025

#ifndef PORTS_H
#define PORTS_H

#include <stdint.h>

/**
 * Write one byte to an x86 I/O port.
 * Used by low-level drivers such as the VGA controller, PIC, PIT, and keyboard.
 */
void outb(uint16_t port, uint8_t val);


/**
 * Read one byte from an x86 I/O port.
 */
uint8_t inb(uint16_t port);

#endif // PORTS_H
