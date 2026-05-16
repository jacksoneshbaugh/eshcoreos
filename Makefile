# eshcoreos Makefile

CC = x86_64-elf-gcc
LD = x86_64-elf-ld
AS = x86_64-elf-as
OBJCOPY = x86_64-elf-objcopy

BOOT_DIR = boot
KERNEL_DIR = kernel
BUILD_DIR = build

BOOT_BIN = $(BUILD_DIR)/boot.bin
KERNEL_ELF = $(BUILD_DIR)/kernel.elf
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
OS_IMAGE = $(BUILD_DIR)/os-image.bin

CFLAGS = -ffreestanding -m32 -O0 -Wall -Wextra -nostdlib -I$(KERNEL_DIR)
LDFLAGS = -T linker.ld -m elf_i386 -z max-page-size=0x1000

C_SOURCES = $(wildcard $(KERNEL_DIR)/*.c)
C_OBJECTS = $(patsubst $(KERNEL_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
ASM_SOURCES = $(wildcard $(KERNEL_DIR)/*.s)
ASM_OBJECTS = $(patsubst $(KERNEL_DIR)/%.s,$(BUILD_DIR)/%.o,$(ASM_SOURCES))

all: $(OS_IMAGE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOT_BIN): $(BOOT_DIR)/boot.asm $(KERNEL_BIN) | $(BUILD_DIR)
	nasm -f bin -D KERNEL_SECTORS=$$(expr \( $$(wc -c < $(KERNEL_BIN)) + 511 \) / 512) $< -o $@

$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.s | $(BUILD_DIR)
	$(AS) --32 $< -o $@

$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_BIN): $(ASM_OBJECTS) $(C_OBJECTS) linker.ld | $(BUILD_DIR)
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) $(ASM_OBJECTS) $(C_OBJECTS)
	$(OBJCOPY) -O binary --set-start=0x10000 $(KERNEL_ELF) $(KERNEL_BIN)

$(OS_IMAGE): $(BOOT_BIN) $(KERNEL_BIN) | $(BUILD_DIR)
	cat $(BOOT_BIN) $(KERNEL_BIN) > $(OS_IMAGE)
	dd if=/dev/zero bs=1 count=$$(expr \( 512 - $$(wc -c < $(KERNEL_BIN)) % 512 \) % 512) >> $(OS_IMAGE) 2>/dev/null

run: $(OS_IMAGE)
	qemu-system-i386 -drive file=$(OS_IMAGE),format=raw,if=ide

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
