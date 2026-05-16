#include "idt.h"
#include "console.h"
#include "pic.h"
#include <stddef.h>

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

static idt_entry_t idt[256];
static idt_ptr_t idt_ptr;
static irq_handler_t irq_handlers[16];

static const char* exception_names[] = {
  "Divide Error",
  "Debug",
  "Non-maskable Interrupt",
  "Breakpoint",
  "Overflow",
  "Bound Range Exceeded",
  "Invalid Opcode",
  "Device Not Available",
  "Double Fault",
  "Coprocessor Segment Overrun",
  "Invalid TSS",
  "Segment Not Present",
  "Stack-Segment Fault",
  "General Protection Fault",
  "Page Fault",
  "Reserved",
  "x87 Floating-Point Exception",
  "Alignment Check",
  "Machine Check",
  "SIMD Floating-Point Exception",
  "Virtualization Exception",
  "Control Protection Exception",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Hypervisor Injection Exception",
  "VMM Communication Exception",
  "Security Exception",
  "Reserved"
};

static void print_u32_decimal(uint32_t value) {
  char buffer[11];
  int i = 0;

  if(value == 0) {
    putChar('0');
    return;
  }

  while(value > 0) {
    buffer[i++] = '0' + (value % 10);
    value /= 10;
  }

  while(i > 0) putChar(buffer[--i]);
}

static void print_u32_hex(uint32_t value) {
  const char* digits = "0123456789abcdef";
  print("0x");

  for(int shift = 28; shift >= 0; shift -= 4) {
    putChar(digits[(value >> shift) & 0xf]);
  }
}

static void idt_clear_gate(uint8_t vector) {
  idt[vector].offset_low = 0;
  idt[vector].selector = 0;
  idt[vector].zero = 0;
  idt[vector].flags = 0;
  idt[vector].offset_high = 0;
}

void idt_set_gate(uint8_t vector, uint32_t handler) {
  idt[vector].offset_low = handler & 0xffff;
  idt[vector].selector = 0x08; // kernel code segment from the GDT
  idt[vector].zero = 0;
  idt[vector].flags = 0x8e; // present, ring 0, 32-bit interrupt gate
  idt[vector].offset_high = (handler >> 16) & 0xffff;
}

void idt_load(idt_ptr_t* ptr) {
  __asm__ volatile ("lidt (%0)" : : "r"(ptr));
}

void idt_init(void) {
  idt_ptr.limit = sizeof(idt) - 1;
  idt_ptr.base = (uint32_t) &idt;
  
  // Leave unused vectors not-present. A stray interrupt should fault loudly.
  for (int i = 0; i < 256; i++) idt_clear_gate(i);

  // CPU-defined exceptions occupy vectors 0-31.
  idt_set_gate(0, (uint32_t) isr0);
  idt_set_gate(1, (uint32_t) isr1);
  idt_set_gate(2, (uint32_t) isr2);
  idt_set_gate(3, (uint32_t) isr3);
  idt_set_gate(4, (uint32_t) isr4);
  idt_set_gate(5, (uint32_t) isr5);
  idt_set_gate(6, (uint32_t) isr6);
  idt_set_gate(7, (uint32_t) isr7);
  idt_set_gate(8, (uint32_t) isr8);
  idt_set_gate(9, (uint32_t) isr9);
  idt_set_gate(10, (uint32_t) isr10);
  idt_set_gate(11, (uint32_t) isr11);
  idt_set_gate(12, (uint32_t) isr12);
  idt_set_gate(13, (uint32_t) isr13);
  idt_set_gate(14, (uint32_t) isr14);
  idt_set_gate(15, (uint32_t) isr15);
  idt_set_gate(16, (uint32_t) isr16);
  idt_set_gate(17, (uint32_t) isr17);
  idt_set_gate(18, (uint32_t) isr18);
  idt_set_gate(19, (uint32_t) isr19);
  idt_set_gate(20, (uint32_t) isr20);
  idt_set_gate(21, (uint32_t) isr21);
  idt_set_gate(22, (uint32_t) isr22);
  idt_set_gate(23, (uint32_t) isr23);
  idt_set_gate(24, (uint32_t) isr24);
  idt_set_gate(25, (uint32_t) isr25);
  idt_set_gate(26, (uint32_t) isr26);
  idt_set_gate(27, (uint32_t) isr27);
  idt_set_gate(28, (uint32_t) isr28);
  idt_set_gate(29, (uint32_t) isr29);
  idt_set_gate(30, (uint32_t) isr30);
  idt_set_gate(31, (uint32_t) isr31);

  // After pic_remap(), hardware IRQs arrive on vectors 32-47.
  idt_set_gate(32, (uint32_t) irq0);
  idt_set_gate(33, (uint32_t) irq1);
  idt_set_gate(34, (uint32_t) irq2);
  idt_set_gate(35, (uint32_t) irq3);
  idt_set_gate(36, (uint32_t) irq4);
  idt_set_gate(37, (uint32_t) irq5);
  idt_set_gate(38, (uint32_t) irq6);
  idt_set_gate(39, (uint32_t) irq7);
  idt_set_gate(40, (uint32_t) irq8);
  idt_set_gate(41, (uint32_t) irq9);
  idt_set_gate(42, (uint32_t) irq10);
  idt_set_gate(43, (uint32_t) irq11);
  idt_set_gate(44, (uint32_t) irq12);
  idt_set_gate(45, (uint32_t) irq13);
  idt_set_gate(46, (uint32_t) irq14);
  idt_set_gate(47, (uint32_t) irq15);

  idt_load(&idt_ptr);
}

void irq_register_handler(uint8_t irq, irq_handler_t handler) {
  if(irq >= 16) return; // out of bounds
  if(irq_handlers[irq] == NULL) irq_handlers[irq] = handler;
}

void irq_unregister_handler(uint8_t irq) {
  if(irq >= 16) return; // out of bounds
  irq_handlers[irq] = NULL;
}

void isr_handler(registers_t* regs) {
  // Hardware IRQs share the common assembly path, then dispatch by IRQ line.
  if (regs->int_no >= 32 && regs->int_no <= 47) {

    uint8_t irq = regs->int_no - 32;

    if(irq_handlers[irq]) irq_handlers[irq](regs);

    pic_send_eoi(irq);
    return;
  }

  // CPU exceptions are fatal until the kernel has recovery policies.
  setColor(0, COLOR_LIGHT_RED, COLOR_BLACK);
  print("\nCPU EXCEPTION ");
  print_u32_decimal(regs->int_no);
  print(": ");

  if(regs->int_no < 32) {
    print(exception_names[regs->int_no]);
  } else {
    print("Unknown");
  }

  print("\nError code: ");
  print_u32_hex(regs->err_code);
  print("\nEIP: ");
  print_u32_hex(regs->eip);
  print("\nSYSTEM HALTED");

  while(1) __asm__ volatile ("cli; hlt");
}
