// Observed _args at 0x005192c0, pinned SLUS_205.65: a local zero-initialised object, followed by
// dataBlock at 0x00519440 (a gap of 0x180 bytes; the symbol has no recorded size). The entry code
// passes its address as the argument block to kernel service 60 and, after _InitSys, calls
// main(word 0, address + 4): so word 0 is the argument count and the words behind it are what
// main receives as the vector (see runtime-entry/start.S). The kernel fills it; nothing in the
// program writes it.
// Unknown: the true size. 0x180 is an upper bound from the next symbol, the alignment is at
// least 16 (the address is 64-byte aligned) and the vector/string split is not established.
extern "C" {
unsigned char _args[0x180] __attribute__((aligned(16)));
}
