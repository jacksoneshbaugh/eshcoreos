.section .text
.extern isr_handler

/*
 * CPU exceptions and hardware IRQs enter C through one common stack layout.
 *
 * Exceptions 8, 10-14, 17, 21, 29, and 30 push a CPU error code. Every other
 * exception and every IRQ gets a dummy zero here, followed by the vector number.
 * isr_common_stub then saves general-purpose and segment registers before
 * passing a registers_t* to isr_handler().
 */

.macro ISR_NOERR num
.global isr\num
isr\num:
  pushl $0
  pushl $\num
  jmp isr_common_stub
.endm

.macro ISR_ERR num
.global isr\num
isr\num:
  pushl $\num
  jmp isr_common_stub
.endm

.macro IRQ num
.global irq\num
irq\num:
  pushl $0               # dummy error code
  pushl $32 + \num       # remapped PIC vector
  jmp isr_common_stub
.endm

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR 8
ISR_NOERR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR_NOERR 15
ISR_NOERR 16
ISR_ERR 17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_ERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_ERR 29
ISR_ERR 30
ISR_NOERR 31

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15


isr_common_stub:
  pusha

  /*
   * Save caller segment registers, then switch to the kernel data segment.
   * The C handler can now safely access kernel data regardless of the
   * interrupted context.
   */
  push %ds
  push %es
  push %fs
  push %gs

  mov $0x10, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs

  push %esp
  call isr_handler
  add $4, %esp

  pop %gs
  pop %fs
  pop %es
  pop %ds

  popa
  add $8, %esp
  iret
