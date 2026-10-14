// Observed: InitChars__7VIRFontPUc at 0x011359f8 is 284 bytes in the pinned SLUS-20565 image. It keeps a running 32-bit offset, started at
// (word at +76) - (word at +84), and for each entry i of the n = word at +40 (none if n <= 0) takes the same byte-to-step value as
// CalcDataSize__7VIRFontPUc from data[i] (selector at +28: 0 -> (b+1)>>1, 1 -> b, 2/3/6 -> b<<2, 4 -> 3*b, 5 -> b<<1, otherwise return -1 at
// once). Entry i is a 12-byte record at (word at +64) + 12 * i: halfword +4 = step, byte +10 = data[i], word +0 = the running offset,
// halfword +8 = 0xffff. Then the running offset grows by step * (word at +20). It returns 0.
// Inferred: this fills the per-character table that CalcDataSize sized (+64 is the table, +76/+84 are the offsets and base that Init
// adjusts); the running offset is each entry's byte offset into a data area; halfword +8 reads like a "no slot yet" marker (-1).
// Unknown: what the records represent exactly, and the meaning of halfword +6 (not written here). Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
int InitChars__7VIRFontPUc(unsigned char* self, unsigned char* data) {
    int* w = reinterpret_cast<int*>(self);
    unsigned offset = static_cast<unsigned>(w[19]) - static_cast<unsigned>(w[21]);
    for (int i = 0; i < w[10]; ++i) {
        unsigned b = data[i];
        unsigned step;
        switch (static_cast<unsigned>(w[7])) {
        case 0: step = (b + 1) >> 1; break;
        case 1: step = b; break;
        case 2: case 3: case 6: step = b << 2; break;
        case 4: step = b * 3; break;
        case 5: step = b << 1; break;
        default: return -1;
        }
        unsigned char* rec = NORRATH_WORD_TO_POINTER(static_cast<unsigned>(w[16])) + i * 12;
        *reinterpret_cast<unsigned short*>(rec + 4) = static_cast<unsigned short>(step);
        rec[10] = static_cast<unsigned char>(b);
        rec = NORRATH_WORD_TO_POINTER(static_cast<unsigned>(w[16])) + i * 12;
        *reinterpret_cast<unsigned*>(rec) = offset;
        *reinterpret_cast<unsigned short*>(rec + 8) = 0xffff;
        offset += step * static_cast<unsigned>(w[5]);
    }
    return 0;
}
}
