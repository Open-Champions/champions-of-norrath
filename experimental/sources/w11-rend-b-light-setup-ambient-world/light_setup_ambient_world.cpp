// Observed lightSetupAmbientWorld__FP6Packeti at 0x0013e160, extent 208. Arguments: a
// Packet (its first word is the output cursor; the second is not used here) and an int.
// Returns nothing.
//
// At the cursor it writes one word 0x6d048000, and then, from the byte after it ("base"):
//   base+0, +2, +4 : the low halfword of the int, three times; base+6 : halfword 0
//   base+8 .. +31  : zero (twenty-four bytes)
//   base+32 : 0x1400000e        base+36 : 0x6c06c000
//   base+40 .. +52 : four floats 1.0f
//   base+56 .. +100: zero (eleven words)
//   base+104 : 1.0e21f (a float read from a literal at 0x0047fcc0, bits 0x6258d726)
//   base+108 .. +116 : zero (three words)
//   base+120 : -1.0f            base+124 .. +132 : zero (three words)
//   base+136 : 0x14000004
// and the cursor becomes base+140. Every byte of base..base+140 is written exactly once.
// The sixteen floats from base+40 form four rows of four, then base+104 and base+120 start
// two more rows; whether that is one matrix is not shown here.
// Unknown: what the tag words, the halfwords and the 1.0e21f value mean to the display
// list consumer, and whether the int is a color or an intensity.
struct AmbientWorldPacket {
    unsigned char* cursor;
    unsigned char* last;
};

namespace {
inline void store32(unsigned char* where, unsigned value) { *reinterpret_cast<unsigned*>(where) = value; }
inline void storef(unsigned char* where, float value) { *reinterpret_cast<float*>(where) = value; }
inline void zero_words(unsigned char* base, int from, int to) {
    for (int at = from; at <= to; at += 4) store32(base + at, 0);
}
}

extern "C" void lightSetupAmbientWorld__FP6Packeti(AmbientWorldPacket* packet, int value) {
    unsigned char* start = packet->cursor;
    store32(start, 0x6d048000u);
    unsigned char* base = start + 4;
    short* halves = reinterpret_cast<short*>(base);
    halves[0] = static_cast<short>(value);
    halves[1] = static_cast<short>(value);
    halves[2] = static_cast<short>(value);
    halves[3] = 0;
    zero_words(base, 8, 28);
    store32(base + 32, 0x1400000eu);
    store32(base + 36, 0x6c06c000u);
    for (int at = 40; at <= 52; at += 4) storef(base + at, 1.0f);
    zero_words(base, 56, 100);
    storef(base + 104, 1.0e21f);
    zero_words(base, 108, 116);
    storef(base + 120, -1.0f);
    zero_words(base, 124, 132);
    store32(base + 136, 0x14000004u);
    packet->cursor = base + 140;
}
