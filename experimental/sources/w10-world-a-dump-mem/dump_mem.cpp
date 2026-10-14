// Observed dumpMem__FPvi at 0x00144730, 68 bytes in SLUS_205.65. The two arguments are a
// pointer and a signed length. The routine computes end = pointer + length, rounds the start
// down to a multiple of 16, and then steps a byte counter from that start up to `end` with
// five no-operation instructions per step (one compare and four nops per step in the image).
// It never reads or writes memory and returns nothing. It is a busy loop that runs
// (end - aligned start) steps, or none when that is not positive (unsigned compare).
// Unknown: why it exists. The name says "dump" but no output call is present in this body, so
// the original may have lost its print call. The compiler here lays the loop out differently
// from the image (no branch-likely), so byte equality is not claimed.
extern "C" void dumpMem__FPvi(void* p, int n) {
    unsigned char* a = reinterpret_cast<unsigned char*>(reinterpret_cast<__UINTPTR_TYPE__>(p) & ~static_cast<__UINTPTR_TYPE__>(15));
    unsigned char* end = reinterpret_cast<unsigned char*>(p) + n;
    for (; a < end; a++) {
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
    }
}
