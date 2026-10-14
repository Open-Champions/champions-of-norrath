// Observed: mcClientID at 0x008e6b40 is a local .bss symbol with st_size 0.
// The next symbol, typeAddr, is 40 bytes later. sceMcInit reads and writes
// the word at offset 36, which is inside those 40 bytes.
// Inferred: the object is 40 zero bytes. It is global here because
// sceMcInit names it from another unit.
// Unknown: the bytes sceMcInit does not touch, and the original section
// under -G0.
extern "C" {
unsigned char mcClientID[40];
}
