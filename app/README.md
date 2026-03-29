# Compile

arm-none-eabi-gcc -nostdlib -T linker.ld main.c -o main.elf

# Run

../build/qemu-system-arm \
  -M my-machine \
  -nographic \
  -kernel main.elf