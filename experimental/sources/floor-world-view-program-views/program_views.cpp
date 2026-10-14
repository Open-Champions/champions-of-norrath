// Observed vu1floorstart at 0x00333df0 and vu1floorend at 0x00334120.
// floorDraw names those addresses norrath_floor_program_start_view and
// norrath_floor_program_end_view and subtracts them. The difference is
// 816 bytes. Each symbol's recorded size is 64. vu1floorend is the same
// address as vu1mapstart. The 619 nonzero bytes in that span are the
// original VU program and are not reproduced here. The two labels are
// emitted in one block so the end stays 816 bytes after the start.
#if defined(__mips__)
asm(
    ".section .bss\n"
    ".globl norrath_floor_program_start_view\n"
    ".globl norrath_floor_program_end_view\n"
    ".type norrath_floor_program_start_view, @object\n"
    ".type norrath_floor_program_end_view, @object\n"
    ".size norrath_floor_program_start_view, 816\n"
    ".size norrath_floor_program_end_view, 64\n"
    ".balign 1\n"
    "norrath_floor_program_start_view:\n"
    ".zero 816\n"
    "norrath_floor_program_end_view:\n"
    ".zero 64\n");
#else
extern "C" {
unsigned char norrath_floor_program_start_view[816];
unsigned char norrath_floor_program_end_view[64];
}
#endif
