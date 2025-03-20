# Toolchain
AS = i686-elf-as
CXX = i686-elf-g++
CC = i686-elf-gcc
LD = i686-elf-ld

# Flags
CXXFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Ilibc/include -Ikernel/include
CFLAGS = -ffreestanding -O2 -Wall -Wextra -Ilibc/include -Ikernel/include -I.
LDFLAGS = -T linker.ld -nostdlib -ffreestanding

# Directories
KERNEL_DIR = kernel
DRIVERS_DIR = drivers
LIBC_DIR = libc
ISO_DIR = isodir
GRUB_CFG = $(ISO_DIR)/boot/grub/grub.cfg
OBJ_DIR = build

# Files
KERNEL_OBJS = \
	$(OBJ_DIR)/kernel/boot.o \
    $(OBJ_DIR)/kernel/kernel.o \
    $(OBJ_DIR)/kernel/tty.o \
    $(OBJ_DIR)/kernel/keyboard.o \
	$(OBJ_DIR)/drivers/timer.o


LIBC_OBJS = \
    $(OBJ_DIR)/libc/string/strlen.o \
	$(OBJ_DIR)/libc/stdio/printf.o \
	$(OBJ_DIR)/libc/stdio/putchar.o

OBJS = $(LIBC_OBJS) $(KERNEL_OBJS)
KERNEL_BIN = $(ISO_DIR)/boot/fos.bin
ISO_FILE = fos.iso

# Targets
.PHONY: all clean run iso

all: $(ISO_FILE)

# Compile kernel objects
$(OBJ_DIR)/kernel/%.o: $(KERNEL_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/kernel/%.o: $(KERNEL_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/kernel/%.o: $(KERNEL_DIR)/%.s
	mkdir -p $(dir $@)
	$(AS) $< -o $@

# Compile driver objects
$(OBJ_DIR)/drivers/%.o: $(DRIVERS_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/drivers/%.o: $(DRIVERS_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/drivers/%.o: $(DRIVERS_DIR)/%.s
	mkdir -p $(dir $@)
	$(AS) $< -o $@

# Compile libc objects
$(OBJ_DIR)/libc/string/%.o: $(LIBC_DIR)/string/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/libc/stdio/%.o: $(LIBC_DIR)/stdio/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Link kernel binary
$(KERNEL_BIN): $(OBJS)
	$(CXX) -v $(LDFLAGS) -o $@ $(OBJS) -lgcc

# Create ISO
$(ISO_FILE): $(KERNEL_BIN) $(GRUB_CFG)
	grub-mkrescue -o $@ $(ISO_DIR)

# Run in QEMU
run: $(ISO_FILE)
	qemu-system-i386 -cdrom $(ISO_FILE)

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(ISO_FILE) $(ISO_DIR)/boot/fos.bin

