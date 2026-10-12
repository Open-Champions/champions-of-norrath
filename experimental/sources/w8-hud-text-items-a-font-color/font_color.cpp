// Observed fontColor__FUiii at 0x00172dc0, 200 bytes, pinned SLUS_205.65.
// It closes the open run of the previous string packet and appends a three
// quadword packet. Observed, in order:
//  - reglistLoopCount holds a pointer to a halfword; nmChars*2 is added to it.
//  - nmDirectQWords grows by nmChars*2, nmChars becomes 0, then it grows by 3.
//  - At the packet cursor (first word of the 16-byte object `p`) it writes
//    quadword 0 = 0x1000000000000001, quadword 1 = 14, a word holding the
//    colour argument, a zero word, quadword 3 = 1, quadword 4 =
//    0x2400000000008000, and quadword 5 = 83.
//  - p word 0 moves 48 bytes on, p word 2 receives the old cursor, and
//    reglistLoopCount receives the old cursor + 32.
//  - A zero third argument also stores the colour in lastFontColor and the
//    second argument in lastFontCharAlpha and fontCharAlpha.
// Inferred: the packet shape is a GIF tag plus register list; not executed.
// Unknown: the meaning of word 1 and word 3 of `p`; the signedness of the
// globals; the name `fontColor` is the ELF name only.
#ifdef NORRATH_HOST_CONTRACT
extern unsigned char* norrath_host_base;
#define NORRATH_PTR(w) (norrath_host_base + (w))
#else
#define NORRATH_PTR(w) reinterpret_cast<unsigned char*>(w)
#endif
extern "C" {
extern unsigned p[4];
extern unsigned reglistLoopCount;
extern int nmChars, nmDirectQWords, lastFontColor, lastFontCharAlpha, fontCharAlpha;
}

extern "C" void fontColor__FUiii(unsigned color, int alpha, int keep) {
    unsigned short* loop = reinterpret_cast<unsigned short*>(NORRATH_PTR(reglistLoopCount));
    int n = nmChars;
    *loop = static_cast<unsigned short>(*loop + n * 2);
    unsigned at = p[0];
    unsigned char* q = NORRATH_PTR(at);
    nmDirectQWords += n * 2;
    nmChars = 0;
    reinterpret_cast<unsigned long long*>(q)[0] = 0x1000000000000000ull;
    reinterpret_cast<unsigned long long*>(q)[1] = 14;
    reinterpret_cast<unsigned long long*>(q + 24)[0] = 1;
    reinterpret_cast<unsigned*>(q + 16)[0] = color;
    reinterpret_cast<unsigned*>(q + 16)[1] = 0;
    reinterpret_cast<unsigned long long*>(q)[0] |= 1;
    p[0] = at + 48;
    reglistLoopCount = at + 32;
    reinterpret_cast<unsigned long long*>(q + 40)[0] = 83;
    reinterpret_cast<unsigned long long*>(q + 32)[0] = 0x2400000000008000ull;
    p[2] = at;
    nmDirectQWords += 3;
    if (keep == 0) {
        lastFontColor = static_cast<int>(color);
        lastFontCharAlpha = alpha;
        fontCharAlpha = alpha;
    }
}
