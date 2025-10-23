# Cross compiler tools
CC = i686-linux-gnu-gcc
LD = i686-linux-gnu-ld
GRUBMKRESCUE = grub-mkrescue
QEMU = qemu-system-i386   # QEMU binary

# Compiler and linker flags
CFLAGS = -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -T linker.ld

# Source files
SRC_C = kernel.c tty.c cursor.c keyboard.c
SRC_ASM = boot.s

# Object files
OBJ = $(SRC_C:.c=.o) $(SRC_ASM:.s=.o)

# Output kernel name
KERNEL = kernel.elf

# ISO folder structure
ISO_DIR = iso_root
ISO_BOOT_DIR = $(ISO_DIR)/boot
ISO_GRUB_DIR = $(ISO_BOOT_DIR)/grub
ISO_FILE = tavios.iso

# Default target: build kernel and copy to ISO folder
all: $(KERNEL) move

# Link the kernel ELF
$(KERNEL): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

# Compile C files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble assembly files
%.o: %.s
	nasm -f elf32 $< -o $@

# Copy kernel.elf into ISO boot folder
move:
	mkdir -p $(ISO_BOOT_DIR)
	cp $(KERNEL) $(ISO_BOOT_DIR)/

# Build bootable ISO (requires grub.cfg in $(ISO_GRUB_DIR))
iso: all
	mkdir -p $(ISO_GRUB_DIR)
	cp grub.cfg $(ISO_GRUB_DIR)/
	$(GRUBMKRESCUE) -o $(ISO_FILE) $(ISO_DIR)

# Run in QEMU
run: iso
	$(QEMU) -cdrom $(ISO_FILE)

# Clean build artifacts
clean:
	rm -f $(OBJ) $(KERNEL) $(ISO_FILE)