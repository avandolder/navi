OBJOPTS="-std=gnu99 -ffreestanding -O2 -Wall -Wextra"
BINOPTS="-T kernlink.ld -ffreestanding -O2 -nostdlib -lgcc"
CC="i686-elf-gcc"

$CC -c kernel.c -o kernel.o $OBJOPTS
$CC -c console.c -o console.o $OBJOPTS
$CC -c keyboard.c -o keyboard.o $OBJOPTS
fasm multiboot.asm
$CC multiboot.o kernel.o console.o keyboard.o -o navi.bin $BINOPTS

rm *.o

cp navi.bin isodir/boot/
grub-mkrescue /usr/lib/grub/i386-pc -o navi.iso isodir

