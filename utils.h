// Utilities for eshcoreos
// &copy; Jackson Eshbaugh, 2025

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

/**
 * @brief Write a byte to an I/O port.
 *
 * Sends the given 8-bit value to the specified I/O port.
 * This is used to communicate with hardware devices that
 * are controlled via the x86 I/O port space (e.g., VGA controller,
 * keyboard controller, PIT).
 *
 * @param port The I/O port number to write to (0-65535)
 * @param val  The 8-bit value to send
 */
void outb(uint16_t port, uint8_t val);


/**
 * @brief Read a byte from an I/O port.
 *
 * Reads an 8-bit value from the specified I/O port.
 * Typically used to poll hardware status or receive input
 * from devices like the keyboard or PIC.
 *
 * @param port The I/O port number to read from
 * @return The 8-bit value read from the port
 */
uint8_t inb(uint16_t port);

#endif // UTILS_H