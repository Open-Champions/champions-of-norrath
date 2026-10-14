// Observed: sifParamOrd at 0x008e6bc0. The symbol size is 0; sifParamFname
// starts 48 bytes later. The memory-card calls store one word at offset 0
// and pass the address with a send size of 48 to sceSifCallRpc. The image
// bytes are zero.
// Inferred: the extent is 48 bytes. Alignment 16 follows the RPC send buffer
// use; the original address is 64-byte aligned.
// Unknown: the layout of the other words.
extern "C" {
alignas(16) unsigned char sifParamOrd[48];
}
