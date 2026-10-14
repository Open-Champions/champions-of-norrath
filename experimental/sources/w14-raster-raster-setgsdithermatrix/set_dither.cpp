// Observed: SetGSDitherMatrix__8VIRasteriPP10_sceDmaTag at 0x011149c0 is 216 bytes in the pinned SLUS-20565 image. Arguments: the
// raster (this), a mode and a pointer to the current DMA write pointer. If the raster's word at +17820 already equals the mode it
// returns 0 and writes nothing. Otherwise, with p the current write pointer, it writes four consecutive 16-byte quadwords and
// advances the write pointer by 64:
//   p+0:  byte 3 = 0x10, halfword at 0 = 3, word at +4 = 0 (a DMA tag with QWC 3 and tag id 1; words +8 and +12 are not written)
//   p+16: words 0, 0, 0x10000000, 0x50000002 (two zero words, then the VIF codes 0x10000000 and 0x50000002)
//   p+32: 64-bit 0x1000000000008001 then 64-bit 14 (a GIF tag: one A+D register write, end of packet)
//   p+48: 64-bit pattern then 64-bit 0x44; the pattern is 0x6071243571603524 when the mode is 1 and 0x2031312020313120 otherwise
// and stores the mode at +17820. It returns 0.
// Inferred: this loads the GS dither matrix register (address 0x44 is DIMX in the GS register map) with one of two matrices through
// a DIRECT packet, and +17820 caches the current mode so that repeats cost nothing.
// Unknown: the dither matrix semantics, whether modes other than 0 and 1 occur, and the DMA buffer's capacity. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int SetGSDitherMatrix__8VIRasteriPP10_sceDmaTag(unsigned char* self, int mode, unsigned* writePointer) {
    if (*reinterpret_cast<int*>(self + 17820) == mode) return 0;
    unsigned char* p = NORRATH_WORD_TO_POINTER(*writePointer);
    p[3] = 0x10;
    *reinterpret_cast<unsigned*>(p + 4) = 0;
    *writePointer = *writePointer + 64;
    *reinterpret_cast<unsigned short*>(p) = 3;
    unsigned* vif = reinterpret_cast<unsigned*>(p + 16);
    vif[3] = 0x50000002u;
    vif[2] = 0x10000000u;
    vif[0] = 0;
    vif[1] = 0;
    unsigned long long* gif = reinterpret_cast<unsigned long long*>(p + 32);
    gif[1] = 14;
    gif[0] = 0x1000000000008001ull;
    unsigned long long* data = reinterpret_cast<unsigned long long*>(p + 48);
    data[0] = mode == 1 ? 0x6071243571603524ull : 0x2031312020313120ull;
    data[1] = 0x44;
    *reinterpret_cast<int*>(self + 17820) = mode;
    return 0;
}
}
