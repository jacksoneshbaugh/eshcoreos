# eshcoreos Makefile

CC = x86_64-elf-gcc							# C compiler
LD = x86_64-elf-ld							# linker
AS = x86_64-elf-as							# assembler
OBJCOPY = x86_64-elf-objcopy		# objcopy instance

# Flags
CFLAGS = -ffreestanding -m32 -O0 -Wall -Wextra -nostdlib
LDFLAGS = -T linker.ld -m elf_i386 -z max-page-size=0x1000

C_SOURCES = $(wildcard *.c)
C_OBJECTS = $(C_SOURCES:.c=.o)

# Default target
all: os-image.bin

# Compile the bootloader
boot.bin: boot.asm
	nasm -f bin boot.asm -o boot.bin

# Assemble the kernel entry stub
kernel_entry.o: kernel_entry.s
	$(AS) --32 kernel_entry.s -o kernel_entry.o

# Compile all .c files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link the kernel together
kernel.bin: kernel_entry.o $(C_OBJECTS)
	$(LD) $(LDFLAGS) -o kernel.elf kernel_entry.o $(C_OBJECTS)
	$(OBJCOPY) -O binary --set-start=0x10000 kernel.elf kernel.bin

# Combine bootloader and kernel
os-image.bin: boot.bin kernel.bin
	cat boot.bin kernel.bin > os-image.bin

# Cleanup
clean:
	rm -f *.o *.elf *.bin

.PHONY: all clean