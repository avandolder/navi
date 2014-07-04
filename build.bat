@echo off

if "%1"=="-nonumber" goto :skip_number

:: Read in the current build number then increment it
<buildnum.txt (set /p "buildnum=")
set /a "buildnum += 1"
echo %buildnum% > buildnum.txt

:: Echo the build number to the screen
echo Build number %buildnum%

set "OBJOPTS=-DBUILDNUM=%buildnum%"

:: Skip putting in the build number
:skip_number

set "OBJOPTS=%OBJOPTS% -std=gnu99 -ffreestanding -O2 -Wall -Wextra"
set "BINOPTS=-T kernlink.ld -ffreestanding -O2 -nostdlib -lgcc"
set "CC=i686-elf-gcc"

for %%i in (*.c) do (
	%CC% -c %%i -o %%i.o %OBJOPTS%
)
fasm multiboot.asm
%CC% *.o -o navi.bin %BINOPTS%
