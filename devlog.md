# Development Log

The whole purpose of this project is for me to learn, so here's where I'll (undoubtedly imperfectly) summarize what I'm learning as I work on the project.

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

So, check out `boot.asm`, its a super interesting and revealing file that shows a lot about the first few seconds when you power up your machine.

The other things I wrote are:

- kernel related files (`kernel_entry.s`, `kernel.c`)
- `linker.ld` to instruct the linker where to place the kernel in the final `os-image.bin` file (so the bootloader jumps to the right place)
- `Makefile` to build everything

The next steps I plan to take are:

- Get some screen drivers working in the kernel, things like clearing the screen and writing text to the screen. One thing that needs done is some function like `kernel_printf` that prints text to the screen and keeps track of where the next character should go (with scrolling and all that).
- After that (though that will take some time), I think a keyboard driver and a way to read from `stdin` is in order.
