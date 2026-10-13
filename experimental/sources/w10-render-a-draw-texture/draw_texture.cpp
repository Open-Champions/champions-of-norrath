#include "../level-anim-load-common/mem.hpp"
// Observed drawTexture__FR6PacketP8_textureiiiiii at 0x0016dc60, 1008 bytes. Arguments: a packet
// reference whose first word is the write cursor, a texture pointer, then six integers named
// here p2, p3, p4, p5, p6, p7 after their registers (a2, a3, t0, t1, t2, t3). Only two texture
// halfwords are read: the signed halfwords at +0 (W) and +2 (H). Write positions are
//   x = (p3 << 4) + 29184 and y = (p2 << 5) + 22528 (full-frame mode, supersample mode) or
//   y = (p2 << 4) + 27648 (otherwise).
// The mode is chosen by engineFullFrameMode (word) and, when that is zero, supersampleMode
// (signed byte). Entries are 16-byte quadwords written at the cursor, which then advances;
// the lines below give (low 64 bits, high 64 bits) with E = 0xA00000000, "<<" on 32-bit
// values wrapping and sign-extending exactly as the image's 32-bit shifts do, and "dsll" a
// full 64-bit shift:
// Full-frame mode (11 quadwords, cursor + 176):
//   0: (x&0x10 ? 2 : 3, 34)
//   1: (((p4<<4)+4) | (((p5<<3)+4)<<16), 3)
//   2: ((y | dsll(x,16)) | E, 5)
//   3: (((p6<<4)+4) | (((p7<<3)+4)<<16), 3)
//   4: (((y + (W<<5)) | dsll(x + (H<<4), 16)) | E, 5)
//   5: (x&0x10 ? 3 : 2, 34)
//   6: (((p4<<4)+4) | ((((H>>1)<<4) + (p5<<3))<<16), 3)
//   7: same as 2 (5)
//   8: (((p6<<4)+4) | ((((H>>1)<<4) + (p7<<3))<<16), 3)
//   9: same as 4 (5)
//  10: (0, 34)
// Supersample mode (4 quadwords, cursor + 64): the field adjustment f is taken from
// engineGetField__Fv() & 1 (when x&0x10 is set, x becomes x+16 or x-16 by the first call and
// the second call picks f = -4 if its low bit is clear, else (H>>1)<<4; when x&0x10 is clear,
// f = ((H>>1)<<4) - 4 if the low bit is clear, else 0):
//   0: (((p4<<4)+4) | (((p5<<3)+f+4)<<16), 3)
//   1: ((y | dsll(x,16)) | E, 5)
//   2: (((p6<<4)+4) | (((p7<<3)+f+4)<<16), 3)
//   3: (((y + (W<<5)) | dsll(x + (H<<4), 16)) | E, 5)
// Plain mode (4 quadwords, cursor + 64), with y as above and W<<4 in place of W<<5:
//   0: (((p4<<4)+8) | (0x80000 + (p5<<20)), 3)
//   1: ((y | dsll(x,16)) | E, 5)
//   2: (((p6<<4)+8) | (0x80000 + (p7<<20)), 3)
//   3: (((y + (W<<4)) | dsll(x + (H<<4), 16)) | E, 5)
// Nothing is returned.
// Inferred: the 3 and 5 in the high words look like graphics-synthesizer register selectors
// (UV and XYZ2) for address-plus-data entries, and the values are texture rectangle corners in
// fixed point; the 34 entries and the 2/3 values look like a transfer tag. Unknown: the exact
// tag layouts, the units of p2..p7, and what engineGetField's low bit selects.
extern "C" {
extern int engineFullFrameMode;
extern signed char supersampleMode;
int engineGetField__Fv();

namespace {
typedef long long i64;
typedef unsigned long long u64;
inline i64 wrap(i64 v) { return static_cast<i64>(static_cast<int>(static_cast<unsigned>(v))); }
inline i64 shl(i64 v, int n) { return wrap(static_cast<i64>(static_cast<unsigned>(v) << n)); }
inline i64 sar(i64 v, int n) { return wrap(static_cast<i64>(static_cast<int>(v) >> n)); }
inline i64 dshl(i64 v, int n) { return static_cast<i64>(static_cast<u64>(v) << n); }
const i64 kE = 0xA00000000ll;
inline void quad(unsigned char* base, int index, i64 low, i64 high) {
    using namespace norrath_lal;
    unsigned char* q = base + index * 16;
    store_word(q + 0, static_cast<unsigned>(static_cast<u64>(low)));
    store_word(q + 4, static_cast<unsigned>(static_cast<u64>(low) >> 32));
    store_word(q + 8, static_cast<unsigned>(static_cast<u64>(high)));
    store_word(q + 12, static_cast<unsigned>(static_cast<u64>(high) >> 32));
}
}

void drawTexture__FR6PacketP8_textureiiiiii(unsigned char* packet, unsigned char* texture, int p2,
                                           int p3, int p4, int p5, int p6, int p7) {
    using namespace norrath_lal;
    if (engineFullFrameMode != 0) {
        i64 x = wrap(shl(p3, 4) + 29184);
        i64 y = wrap(shl(p2, 5) + 22528);
        const i64 h = load_signed_half(texture + 2);
        const i64 w = load_signed_half(texture + 0);
        const i64 halfRows = shl(sar(shl(h, 16), 17), 4);
        const i64 rightX = wrap(x + sar(shl(h, 16), 12));
        const i64 bottomY = wrap(y + shl(w, 5));
        const bool odd = (x & 0x10) != 0;
        const i64 corner = dshl(rightX, 16);
        const i64 top = (y | dshl(x, 16)) | kE;
        const i64 bottom = (bottomY | corner) | kE;
        const i64 t4 = shl(p4, 4), t6 = shl(p6, 4);
        unsigned char* base = load_pointer(packet);
        store_pointer(packet, base + 176);
        quad(base, 0, odd ? 2 : 3, 34);
        quad(base, 1, wrap(t4 + 4) | shl(wrap(shl(p5, 3) + 4), 16), 3);
        quad(base, 2, top, 5);
        quad(base, 3, wrap(t6 + 4) | shl(wrap(shl(p7, 3) + 4), 16), 3);
        quad(base, 4, bottom, 5);
        quad(base, 5, odd ? 3 : 2, 34);
        quad(base, 6, wrap(t4 + 4) | shl(wrap(halfRows + shl(p5, 3)), 16), 3);
        quad(base, 7, top, 5);
        quad(base, 8, wrap(t6 + 4) | shl(wrap(halfRows + shl(p7, 3)), 16), 3);
        quad(base, 9, bottom, 5);
        quad(base, 10, 0, 34);
    } else if (supersampleMode != 0) {
        i64 x = wrap(shl(p3, 4) + 29184);
        const i64 y = wrap(shl(p2, 5) + 22528);
        const i64 h = load_signed_half(texture + 2);
        i64 f;
        if ((x & 0x10) != 0) {
            const int first = engineGetField__Fv();
            const i64 up = wrap(x + 16), down = wrap(x - 16);
            x = (first & 1) != 0 ? up : down;
            const int second = engineGetField__Fv();
            f = (second & 1) != 0 ? shl(sar(shl(h, 16), 17), 4) : -4;
        } else {
            const int field = engineGetField__Fv();
            f = (field & 1) == 0 ? wrap(shl(sar(shl(h, 16), 17), 4) - 4) : 0;
        }
        const i64 w = load_signed_half(texture + 0);
        const i64 rightX = wrap(x + sar(shl(h, 16), 12));
        const i64 bottomY = wrap(y + shl(w, 5));
        unsigned char* base = load_pointer(packet);
        store_pointer(packet, base + 64);
        quad(base, 0, wrap(shl(p4, 4) + 4) | shl(wrap(wrap(shl(p5, 3) + f) + 4), 16), 3);
        quad(base, 1, (y | dshl(x, 16)) | kE, 5);
        quad(base, 2, wrap(shl(p6, 4) + 4) | shl(wrap(wrap(shl(p7, 3) + f) + 4), 16), 3);
        quad(base, 3, (bottomY | dshl(rightX, 16)) | kE, 5);
    } else {
        i64 h = load_signed_half(texture + 2);
        const i64 x = wrap(shl(p3, 4) + 29184);
        const i64 y = wrap(shl(p2, 4) + 27648);
        const i64 rightX = wrap(x + shl(h, 4));
        const i64 w = load_signed_half(texture + 0);
        const i64 bottomY = wrap(y + shl(w, 4));
        unsigned char* base = load_pointer(packet);
        store_pointer(packet, base + 64);
        quad(base, 0, wrap(shl(p4, 4) + 8) | wrap(0x80000 + shl(p5, 20)), 3);
        quad(base, 1, (y | dshl(x, 16)) | kE, 5);
        quad(base, 2, wrap(shl(p6, 4) + 8) | wrap(0x80000 + shl(p7, 20)), 3);
        quad(base, 3, (bottomY | dshl(rightX, 16)) | kE, 5);
    }
}
}
