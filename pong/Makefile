OUTPUT = pong.elf

CC = x86_64-elf-gcc
LD = x86_64-elf-ld

# Internal C flags that should not be changed by the user.
override CFLAGS += \
    -Wall \
    -Wextra \
    -ffreestanding \
	-mcmodel=large


# Internal C preprocessor flags that should not be changed by the user.
override CPPFLAGS := \
	-I ../libc/include \
	-I include


# Internal nasm flags that should not be changed by the user.
override NASMFLAGS += \
    -Wall \
    -f elf64

# Internal linker flags that should not be changed by the user.
override LDFLAGS += \
    -T linker.ld \
    -nostdlib

# Use "find" to glob all *.c, *.S, and *.asm files in the tree and obtain the
# object and header dependency file names.
override SRCFILES := $(shell find -L * -type f | LC_ALL=C sort)
override CFILES := $(filter %.c,$(SRCFILES))
override ASFILES := $(filter %.S,$(SRCFILES))
override NASMFILES := $(filter %.asm,$(SRCFILES))
override OBJ := $(addprefix obj/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o) $(NASMFILES:.asm=.asm.o)) $(shell find ../libc/obj -type f -name "*.o")

# Default target. This must come first, before header dependencies.
.PHONY: all
all: obj/$(OUTPUT)

# Link rules for the final executable.
obj/$(OUTPUT): linker.ld $(OBJ)
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@

# Compilation rules for *.c files.
obj/%.c.o: %.c 
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compilation rules for *.S files.
obj/%.S.o: %.S 
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compilation rules for *.asm (nasm) files.
obj/%.asm.o: %.asm 
	mkdir -p "$$(dirname $@)"
	nasm $(NASMFLAGS) $< -o $@

# Remove object files and the final executable.
.PHONY: clean
clean:
	rm -rf bin obj
