<H1> FalconOS </H1>

 64 bit kernel that gets loaded by limine and actually works on real hardware



 <H1> How to run </H1>

- Put one foot in front of the other
- Repeat

 <H3> dependencies </H3>
 
  - make
  - 64 bit cross compiler (x86_64-elf-gcc)
  - qemu
  - git
  - curl
  - sgdisk (gptfdisk)

<H3> commands </H3>

`make` will build all files needed to run the kernel in a virtual machine or on real hardware.

`make run` is same as `make` and runs it in a virtual machine.

`make clean` will delete all binary files, it will not delete `ovmf/` and `limine/`.

`make deepclean` is same as `make clean`, but will also delete `ovmf/` and `limine/`.

`make lines` will print all lines per source file and also total line count.

<H1> Finished </H1>

- IDT
- IRQ0 PIT
- IRQ1 keyboard

<H1> To do </H1>

- memory
- shell
- tasks / processes / multitasking
- filesystem
- executable files
- AVX
- 
