.section .text.kernel_entry
.global kernel_entry
.extern kernel_main

kernel_entry:
  mov $0x90000, %esp
  call kernel_main
  cli
  hlt
  jmp .
