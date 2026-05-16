// Programmable Interval Timer (PIT) driver.

#include "pit.h"
#include "idt.h"
#include "pic.h"
#include "ports.h"

#define PIT_COMMAND  0x43
#define PIT_DATA_0   0x40
#define PIT_BASE_HZ  1193182

static timer_hook_t timer_hooks[TIMER_MAX_HOOKS];
static int next_hook_idx = 0;

static uint32_t ticks = 0;

static void pit_irq_handler(registers_t *registers) {
  (void) registers;

  ticks++;

  for(int i = 0; i < TIMER_MAX_HOOKS; i++) {
    if(timer_hooks[i]) timer_hooks[i](ticks);
  }
  
}

void timer_register_hook(timer_hook_t hook) {
  if(hook == 0) return;

  if(next_hook_idx < TIMER_MAX_HOOKS) 
    timer_hooks[next_hook_idx++] = hook;
}

void pit_init(uint32_t frequency) {

  if(frequency == 0) return;

  /*
   * Command 0x36:
   *   channel 0, low byte then high byte, mode 3 square wave, binary counter.
   * Channel 0 is wired to IRQ0, making it the kernel's periodic timer source.
   */

  uint8_t  command = 0x36;
  uint16_t divisor = PIT_BASE_HZ / frequency;

  outb(PIT_COMMAND, command);
  outb(PIT_DATA_0, divisor & 0xff);
  outb(PIT_DATA_0, (divisor >> 8) & 0xff);
  
  // Own IRQ0 inside the PIT driver; higher-level code can use timer hooks.
  irq_register_handler(0, pit_irq_handler);
  pic_unmask_irq(0);
}

uint32_t pit_get_ticks() {
  return ticks;
}
