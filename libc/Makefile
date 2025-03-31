CC = x86_64-elf-gcc

# Internal C flags that should not be changed by the user.
override CFLAGS += \
    -Wall \
    -Wextra \
    -ffreestanding \
	-mcmodel=large \
	-nostdlib


# Internal C preprocessor flags that should not be changed by the user.
override CPPFLAGS := \
    -I src \
	-I include

override SRCFILES := $(shell find -L * -type f | LC_ALL=C sort)
override CFILES := $(filter %.c,$(SRCFILES))
override OBJ := $(addprefix obj/,$(CFILES:.c=.c.o))

.PHONY: all

all: $(OBJ)

# Compilation rules for *.c files.
obj/%.c.o: %.c 
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf obj/
	
