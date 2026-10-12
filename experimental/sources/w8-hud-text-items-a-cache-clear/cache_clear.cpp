// Observed cacheClear__Fv at 0x0016ce88, 148 bytes, pinned SLUS_205.65.
// `cache` (0x006e5d80) is read as sixteen 12-byte records; `dummyTex`
// (0x006e5e40) follows it. cacheHead and cacheTail are ring positions.
// Observed: starting at cacheHead and stepping by one modulo 16, the halfword
// at byte 24 of the object each record's first word points to is set to 0.
// The last record cleared is the one at cacheTail (the first when head equals
// tail). Then both positions become 0 and every record is reset: word 0
// receives the address of dummyTex and the halfwords at bytes 4, 6 and 8 become 0.
// Inferred: the first word is a texture pointer and byte 24 is a residency
// marker of that texture; not established. Byte 10 of each record is untouched.
// Not executed on hardware.
#ifdef NORRATH_HOST_CONTRACT
extern unsigned char* norrath_host_base;
#define NORRATH_PTR(w) (norrath_host_base + (w))
#define NORRATH_WORD(p) static_cast<unsigned>(reinterpret_cast<unsigned long>(p))
#else
#define NORRATH_PTR(w) reinterpret_cast<unsigned char*>(w)
#define NORRATH_WORD(p) reinterpret_cast<unsigned>(p)
#endif
extern "C" {
extern unsigned char cache[];
extern unsigned char dummyTex[];
extern int cacheHead, cacheTail;
}

extern "C" void cacheClear__Fv() {
    int i = cacheHead;
    int last = cacheTail;
    for (;;) {
        unsigned tex = *reinterpret_cast<unsigned*>(cache + i * 12);
        *reinterpret_cast<unsigned short*>(NORRATH_PTR(tex) + 24) = 0;
        if (i == last) break;
        i = (i + 1) & 15;
    }
    cacheHead = 0;
    cacheTail = 0;
    for (int n = 0; n < 16; ++n) {
        unsigned char* record = cache + n * 12;
        *reinterpret_cast<unsigned*>(record) = NORRATH_WORD(dummyTex);
        *reinterpret_cast<unsigned short*>(record + 4) = 0;
        *reinterpret_cast<unsigned short*>(record + 6) = 0;
        *reinterpret_cast<unsigned short*>(record + 8) = 0;
    }
}
