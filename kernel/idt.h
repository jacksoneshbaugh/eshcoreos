#ifndef IDT_H
#define IDT_H

#include <stdint.h>

typedef struct {
  uint16_t offset_low;    // handler address bits 0-15
  uint16_t selector;      // kernel code segment selector (usually 0x08)
  uint8_t  zero;          // always 0
  uint8_t  flags;         // present bit, privilege level, gate type
  uint16_t offset_high;   // handler address bits 16-31
} __attribute__((packed)) idt_entry_t;

typedef struct {
  uint16_t limit;   // sizeof(idt) - 1
  uint32_t base;    // address of idt[0]
} __attribute__((packed)) idt_ptr_t;

typedef struct {
  /*
   * Stack frame created by isr.s before calling isr_handler().
   *
   * The CPU pushes eip/cs/eflags for every interrupt. Some exceptions also
   * push an error code; isr.s adds a dummy zero for exceptions and IRQs that
   * do not, so C always sees the same layout.
   */
  uint32_t gs, fs, es, ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uint32_t int_no, err_code;
  uint32_t eip, cs, eflags;
} __attribute__((packed)) registers_t;

typedef void (*irq_handler_t)(registers_t* regs);

/**
 * register a handler for a given IRQ
 */
void irq_register_handler(uint8_t irq, irq_handler_t handler);

/**
 * unregister the registered handler for a given IRQ
 */
void irq_unregister_handler(uint8_t irq);

void idt_init(void);
void idt_set_gate(uint8_t vector, uint32_t handler);
void idt_load(idt_ptr_t* ptr);
void isr_handler(registers_t* regs);

#endif // IDT_H
