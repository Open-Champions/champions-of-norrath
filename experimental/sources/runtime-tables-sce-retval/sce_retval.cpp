// Observed retval at 0x8e8100, 64 nobits bytes in .bss. The memory-card
// initializer reads service version words from offsets 0, 4, and 8 after
// its bind call; the object starts cleared.
// Inferred: this reconstruction name is that object, zero-initialized
// like the nobits image.
// Unknown: the bind call that fills it, and placement at the original
// address.
extern "C" {
unsigned char retval[64] = {};
}
