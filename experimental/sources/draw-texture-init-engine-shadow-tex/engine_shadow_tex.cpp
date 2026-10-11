// Observed engineShadowTexAddress at 0x004844d4, a 4-byte .sdata zero.
// spriteCount begins at 0x004844d8. engineInit stores 0x00358000 here at
// runtime. The reviewed -G0 profile places the zero in BSS.

extern "C" {
int engineShadowTexAddress;
}
