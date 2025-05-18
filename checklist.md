# `eshcoreos` Development Checklist

---

## Boot and Kernel Basics

- [x] Enter 16-bit real mode
- [x] Load kernel sectors into memory
- [x] Switch to 32-bit protected mode
- [x] Jump to C kernel
- [x] VGA text output (80×25, colored)
- [x] Modular console driver (`print()`, `putchar()`)
- [x] Hardware cursor management

---

## Memory Management

- [ ] Parse BIOS memory map (E820)
- [ ] Physical frame allocator (e.g. bitmap-based)
- [ ] Virtual memory and paging (basic page tables)
- [ ] Page faults and protection bits
- [ ] Dynamic kernel heap allocator (e.g. bump or slab)

---

## Processes and Multitasking

- [ ] IDT (Interrupt Descriptor Table)
- [ ] Timer interrupt handler (PIT, IRQ0)
- [ ] Context switch logic
- [ ] Basic process control block (PCB)
- [ ] Cooperative multitasking
- [ ] Preemptive multitasking via timer IRQ

---

## Filesystem and Storage

- [ ] Read sectors using LBA or CHS
- [ ] Simple in-memory "filesystem"
- [ ] Read a flat file by name
- [ ] Load a user program from disk
- [ ] Write a basic FAT12 or custom FS reader

---

## User Programs & ABI

- [ ] Define a simple ABI (e.g. calling convention for `main()`)
- [ ] Support compiled user programs in a known binary format
- [ ] Add a syscall interface (`int 0x80` or similar)
- [ ] Implement basic syscalls (`print`, `exit`, etc.)
- [ ] Shell to select and run programs

---

## Drivers and I/O

- [ ] PS/2 keyboard driver
- [ ] Interrupt-based input
- [ ] Basic command-line parser
- [ ] VGA text mode scrolling + line editing

---

## Dev and UX Tools

- [x] Makefile-based build system
- [ ] `make run` support for QEMU
- [ ] `make iso` for bootable CD-ROM
- [ ] GDB integration (`make debug`)
- [ ] Simple kernel debugger (`kdebug>`)
- [ ] `printf()` with formatting

---

## Stretch Goals / Nice-to-Haves

- [ ] ELF loader for user programs
- [ ] Linker script support for user-space memory layout
- [ ] Paging with copy-on-write
- [ ] Pipes and file descriptors
- [ ] Shell scripting engine
- [ ] Graphical framebuffer mode

---

## Long-Term Vision

- [ ] Allow users to write and compile programs in C for `eshcoreos`
- [ ] Implement a toy standard library (`libesh`)
- [ ] Support filesystem loading from a real disk image or ISO
- [ ] Document system calls and APIs
