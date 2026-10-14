// Observed _GLOBAL_$I$_fontStringSize__FP4FontPci at 0x001745b8, 32 bytes,
// pinned SLUS_205.65, and the translation unit's local initialization
// routine at 0x00174328 (32 bytes) that it calls with arguments 1 and 0xffff.
// The local routine does nothing unless it receives exactly those arguments.
// With them it copies the word glistp (0x004844c8) into the first word of the
// 16-byte object `p` (0x006e93d0). Its other three words are not written.
// Inferred: the wrapper is the compiler-generated constructor entry for the
// font unit; it is listed in the startup constructor table by another package.
// No matching destructor entry exists for this unit in the image.
// Not executed on hardware. Neither `p` nor glistp is defined here.
extern "C" {
extern unsigned p[4];
extern unsigned glistp;
}

__attribute__((noinline)) static void staticInitFont(int phase, int priority) {
    if (priority == 0xffff && phase == 1) p[0] = glistp;
}

extern "C" void _GLOBAL_$I$_fontStringSize__FP4FontPci() {
    staticInitFont(1, 0xffff);
}
