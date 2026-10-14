// Observed engineDrawBufferStartAddress at 0x004844cc, a 4-byte .sdata
// zero. engineEnvTexAddress begins at 0x004844d0. engineInit stores 0 here
// again at runtime. The reviewed -G0 profile places the zero in BSS.

extern "C" {
unsigned engineDrawBufferStartAddress;
}
