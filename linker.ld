/* eshcoreos linker.ld
  &copy; Jackson Eshbaugh, 2025 */

ENTRY(kernel_entry)

SECTIONS {
  . = 0x10000;

  .text : {
    KEEP(*(.text.kernel_entry))
    *(.text)
  }

  .data : {
    *(.data)
  }

  .bss : {
    *(.bss)
  }
}