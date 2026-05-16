#include "pic.h"
#include "ports.h"

#define PIC1_COMMAND  0x20
#define PIC1_DATA     0x21
#define PIC2_COMMAND  0xa0
#define PIC2_DATA     0xa1

#define PIC_EOI       0x20

/*
 * A tiny delay between PIC port writes. Modern emulators rarely need this,
 * but the legacy PIC programming sequence assumes slow I/O hardware.
 */
static void io_wait(void) {
  outb(0x80, 0);
}

void pic_remap(void) {
  // Preserve the current interrupt masks while reinitializing the PICs.
  uint8_t master_mask = inb(PIC1_DATA);
  uint8_t slave_mask = inb(PIC2_DATA);

  // ICW1: start initialization; both PICs will receive ICW4.
  outb(PIC1_COMMAND, 0x11);
  io_wait();
  outb(PIC2_COMMAND, 0x11);
  io_wait();
  
  // ICW2: choose IDT vector offsets for master and slave IRQ ranges.
  outb(PIC1_DATA, 0x20);    // master offset: 32
  io_wait();
  outb(PIC2_DATA, 0x28);    // slave offset: 40
  io_wait();

  // ICW3: describe the cascade wiring between the two PICs.
  outb(PIC1_DATA, 0x04);    // tell master slave is at IRQ2
  io_wait();
  outb(PIC2_DATA, 0x02);    // tell slave its cascade identity
  io_wait();

  // ICW4: use 8086/88 mode, which matches protected-mode x86 interrupt gates.
  outb(PIC1_DATA, 0x01);    // 8086 mode
  io_wait();
  outb(PIC2_DATA, 0x01);
  io_wait();

  // Restore masks so remapping does not unexpectedly enable devices.
  outb(PIC1_DATA, master_mask);
  io_wait();
  outb(PIC2_DATA, slave_mask);
  io_wait();
}

void pic_send_eoi(uint8_t irq) {
  if(irq >= 8) {
    outb(PIC2_COMMAND, PIC_EOI);
  }

  outb(PIC1_COMMAND, PIC_EOI);
}

void pic_mask_irq(uint8_t irq) {
  uint16_t port;
  uint8_t value;

  if(irq < 8) {
    port = PIC1_DATA;
  } else {
    port = PIC2_DATA;
    irq -= 8;
  }

  value = inb(port) | (1 << irq);
  outb(port, value);
}

void pic_unmask_irq(uint8_t irq) {
  uint16_t port;
  uint8_t value;

  if(irq < 8) {
    port = PIC1_DATA;
  } else {
    port = PIC2_DATA;
    irq -= 8;
  }

  value = inb(port) & ~(1 << irq);
  outb(port, value);
}
