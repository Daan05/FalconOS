<H1> FalconOS </H1>

 64 bit kernel that gets loaded by limine and actually works on real hardware



 <H1> How to run </H1>
 
 <H3> dependencies </H3>
 
  - make
  - 64 bit cross compiler (x86_64-elf-gcc)
  - qemu
  - git
  - curl
  - sgdisk (gptfdisk)

<H3> commands </H3>

`make` will build all files needed to run the kernel in a virtual machine or on real hardware. `make run` build the kernel and runs it in a virtual machine. `make clean` will delete all binary files, it will not delete `ovmf/` and `limine/`. To count all lines in the FalconOS project run `find . -type f \( -name '*.c' -o -name '*.h' -o -name Makefile -o -name *.asm \) -not -path '*limine*' | xargs wc -l`
