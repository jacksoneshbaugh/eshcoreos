# Development Log

The whole purpose of this project is for me to learn, so here's where I'll (undoubtedly imperfectly) summarize what I'm learning as I work on the project.

## Interrupts and Timer Bring-up (May 16, 2026)

This round moved `eshcoreos` from a kernel that simply prints text into one that can receive and dispatch interrupts. I reorganized the repository into `boot/`, `kernel/`, `docs/`, and `build/` so the project has room to grow without every source file living in the root directory.

The kernel now builds an Interrupt Descriptor Table (IDT) for CPU exceptions and remapped hardware IRQs. Assembly stubs in `kernel/isr.s` normalize the interrupt stack frame before calling a shared C handler, which lets the kernel print useful exception information and dispatch hardware IRQs through a small registration table.

I also added a `ports` module for `inb`/`outb`, a PIC driver that remaps IRQs to vectors 32-47, and a PIT driver that programs channel 0 at 100 Hz. The PIT owns IRQ0 internally and exposes lightweight timer hooks for other kernel systems. For now, the kernel registers a simple debug hook that prints one dot per second, proving that timer interrupts are firing.

The next major topic is memory. Before real processes or serious scheduling, I want the kernel to understand its own memory layout and then bring up basic paging with an identity map of early memory.

## Console Drivers (May 18, 2025)

Today, I had a relatively straightforward task in front of me: I began to develop the console drivers for the kernel. Specifically, I had to write my own print function, and all the other things I'd need to get there. This included some simpler functions and some more complex ones. Specifically, the `inb` and `outb` functions were tough—because I didn't even know I needed them. These are used to write bytes to and read bytes from I/O ports. I needed this functionality to disable the blinking cursor. All in all, I learned how powerful inline assembly can really be, and (somewhat) how to control the I/O controller using C.

Also, the kernel exceeded 1 sector, so I had to modify the bootloader to boot the entire thing. Things are taking shape, and they look pretty cool!

### Next on the Chopping block

- screen scrolling
- keyboard driver
- prototype shell

## Bootloaders (May 17, 2025)

I knew when I began this quest that one of the most curious parts of OS development would come almost immediately: the **bootloader**. How does the kernel get into memory, and what calls its `main` method? Answer: the bootloader.

It was very interesting writing my own version of a bootloader—you have to learn how to talk to the BIOS and call interrupts in order to load the kernel into memory (or to just print a string to the screen—ain't no `printf` here!)

The bootloader occupies the first sector of the booted medium—think hard drive, floppy disk, USB stick, etc. The BIOS knows to load this sector, which in turn allows us to write assembly instructions to—do anything. In our case, we need to write instructions that load the kernel into memory, then move the program counter to it. Luckily the kernel is simple: it just writes a line of text to video memory. So, our kernel is smaller than 1 sector, and thus is entirely contained within the 2nd sector. Earlier versions of the bootloader just loaded this sector into memory, and it worked.

But, what about when the kernel gets bigger? With help from LLMs, I was able to develop a version of the bootloader that loads a variable number of sectors into memory. Problem solved (for now, anyways).

So, check out `boot/boot.asm`, its a super interesting and revealing file that shows a lot about the first few seconds when you power up your machine.

The other things I wrote are:

- kernel related files (`kernel/kernel_entry.s`, `kernel/kernel.c`)
- `linker.ld` to instruct the linker where to place the kernel in the final `build/os-image.bin` file (so the bootloader jumps to the right place)
- `Makefile` to build everything

The next steps I plan to take are:

- Get some screen drivers working in the kernel, things like clearing the screen and writing text to the screen. One thing that needs done is some function like `kernel_printf` that prints text to the screen and keeps track of where the next character should go (with scrolling and all that).
- After that (though that will take some time), I think a keyboard driver and a way to read from `stdin` is in order.
