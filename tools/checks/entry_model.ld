/* Instruction-model fixture only. Absolute imports supply intercepted test
 * addresses, not implementations. This is NOT a game linker script. */
OUTPUT_ARCH(mips:5900)
ENTRY(_start)
_fbss = 0x00486800;
_bssend = 0x008eab1c;
_gp = 0x004879f0;
_args = 0x005192c0;
_root = 0x00100250;
_end = 0x008ec91c;
_InitSys = 0x002dd8e8;
FlushCache = 0x002d5360;
main = 0x002d0818;
exit = 0x002ffb08;
SECTIONS {
 .text 0x00100008 : { *(.text.startup) }
 .MIPS.abiflags 0x500000 : { *(.MIPS.abiflags) }
}
