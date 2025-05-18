# eshcoreos

A hobbyist OS built for understanding

## About

As a sophomore computer science student, I felt an urge to understand some of the more low-level things that our systems do. There's no better way to understand exactly what things do than to build them, so I decided that in pursuit of an understanding of exactly how operating systems work, I'd implement my own. I also decided to record the things I learned along the way—check out the [`devlog.md`](./devlog.md) file!

The operating system itself is designed to run in `QEMU` and is compiled in x86 assembly.

## Requirements

To build eshcoreos, you'll need the following:

- `nasm` (the Netwide Assembler), to assemble the bootloader
- `make`, to run the build automation
- `qemu`, to emulate the system
- a **bare-metal C compiler** targeting `i386-elf` (or similar)—_I use `x86_64-elf-gcc` on macOS._

You can install these easily on macOS (with Homebrew) or Linux (with APT):

```bash
# macOS, using Homebrew
brew update && brew upgrade
brew install nasm qemu
brew tap nativeos/i386-elf-toolchain
brew install x86_64-elf-binutils x86_64-elf-gcc

# Ubuntu/Debian Linux
sudo apt install nasm qemu make gcc gcc-i386-elf
```

**Note**: Depending on which compiler you use, you'll need to update the `Makefile` so that it runs your compiler.

## Build & Run

To build eshcoreos and run it in `QEMU`:

```bash
make
qemu-system-i386 -hda os-image.bin
```

## About Me

My name is Jackson Eshbaugh, and I'm a passionate teacher-researcher studying computer science and French at Lafayette College. It is my goal to join academia and contribute to the conversation of science while inspiring and guiding future students through the wonderful world of computer science. You can read more about me, my research interests, and my work on my [website](https://jacksoneshbaugh.github.io).
