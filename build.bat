@echo off

:: Read in the current build number then increment it
<buildnum.txt (set /p "buildnum=")
set /a "buildnum += 1"
echo %buildnum% > buildnum.txt

:: Echo the build number to the screen
echo %buildnum%

set "OBJOPTS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra -DBUILDNUM=%buildnum%"
set "BINOPTS=-T kernlink.ld -ffreestanding -O2 -nostdlib -lgcc"
set "CC=i686-elf-gcc"

%CC% -c kernel.c -o kernel.o %OBJOPTS%
fasm multiboot.asm
%CC% kernel.o multiboot.o -o navi.bin %BINOPTS%
