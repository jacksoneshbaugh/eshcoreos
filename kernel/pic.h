#ifndef PIC_H
#define PIC_H

#include <stdint.h>

/**
 * Reprogram the legacy 8259 PICs so hardware IRQs use IDT vectors 32-47.
 *
 * The BIOS default mapping overlaps with CPU exception vectors. After remap:
 *   IRQ0-IRQ7   -> vectors 32-39
 *   IRQ8-IRQ15  -> vectors 40-47
 */
void pic_remap(void);

/**
 * Tell the PIC that an IRQ has been handled.
 *
 * IRQs from the slave PIC (8-15) require an EOI to both the slave and master.
 */
void pic_send_eoi(uint8_t irq);

/**
 * Disable or enable an individual IRQ line.
 */
void pic_mask_irq(uint8_t irq);
void pic_unmask_irq(uint8_t irq);

#endif // PIC_H
