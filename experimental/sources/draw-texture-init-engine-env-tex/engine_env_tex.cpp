// Observed engineEnvTexAddress at 0x004844d0, a 4-byte .sdata zero.
// engineShadowTexAddress begins at 0x004844d4. engineInit stores 0x00348000
// here at runtime. The reviewed -G0 profile places the zero in BSS.

extern "C" {
unsigned engineEnvTexAddress;
}
