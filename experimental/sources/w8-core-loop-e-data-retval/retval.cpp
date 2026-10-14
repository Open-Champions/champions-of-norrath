// Observed retval at 0x008e8100. The symbol has no recorded size; the next
// symbol, sceCdCbfunc, starts 0x40 bytes later, so 64 bytes are reserved. It
// is in .bss and zero at load. The aligned address is kept at 64 bytes; the
// real alignment is not recorded. What the storage holds is unknown.
extern "C" {
alignas(64) unsigned char retval[64];
}
