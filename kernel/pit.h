// Programmable Interval Timer (PIT) driver.

#ifndef PIT_H
#define PIT_H

#include <stdint.h>

#define TIMER_MAX_HOOKS 8

typedef void (*timer_hook_t)(uint32_t ticks);

/**
 * Register a lightweight callback to run on every timer tick.
 *
 * Hooks run from IRQ0 interrupt context, so they must stay short and nonblocking.
 */
void timer_register_hook(timer_hook_t hook);

/**
 * Program PIT channel 0 to fire IRQ0 at the requested frequency.
 */
void pit_init(uint32_t frequency);

/**
 * Return the number of PIT ticks observed since initialization.
 */
uint32_t pit_get_ticks();

#endif // PIT_H
