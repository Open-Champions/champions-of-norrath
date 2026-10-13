// Observed .bss symbol beardSetting at 0x8d5b58. The ELF size field is 0; the next symbol
// (skinSetting) is 16 bytes later. changeBeard__Fii (0x280810) indexes it as bytes by its first
// argument, reading a signed byte and storing -1 or an incremented byte.
// Inferred: one signed byte per index, the gap taken as the size. Unknown: how many indices are used.
extern "C" {
signed char norrath_front_a_view_8d5b58[16];
}
