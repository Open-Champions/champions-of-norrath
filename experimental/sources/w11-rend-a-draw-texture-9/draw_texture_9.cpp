#include "../w11-rend-a-common/rend.hpp"
// Observed drawTexture__FR6Packetiiiiiiiii at 0x001732d8, 300 bytes (a file-local function in the
// image, called by the draw code). Arguments: a packet reference whose first word is the write
// cursor, then nine ints, named here i1..i9 in order (registers a1, a2, a3, t0, t1, t2, t3, then
// the first and second stack words). scanMask (a gp-relative word) is read. Let odd = (i3 & 0x10)
// != 0. Two blocks are written, each of two 16-byte quadwords at the cursor, which advances by 32:
//   block A, when scanMask == (odd ? 2 : 3):
//     q0 = ((i6+4) | ((i7+4) << 16),   i2 | (i3 << 16))
//     q1 = ((i8+4) | ((i9+4) << 16),   i4 | (i5 << 16))
//   block B, written after A and judged on scanMask again, when scanMask == (odd ? 3 : 2),
//   with h = (i1 >> 1) << 4 (arithmetic shift):
//     q0 = ((i6+4) | ((h+i7) << 16),   i2 | (i3 << 16))
//     q1 = ((i8+4) | ((h+i9) << 16),   i4 | (i5 << 16))
// All sums and shifts are 32-bit and each 64-bit half is the sign extension of the 32-bit
// register values joined by OR, as the image does; so a half whose bit 31 is set also sets the
// upper 32 bits. Both blocks are never written in one call, since the two conditions exclude each
// other. Nothing is returned.
// Inferred: these look like GS texture-coordinate address/data pairs for a sprite drawn on one of
// the two interlace fields (scanMask 2 or 3). Unknown: the field meaning of scanMask and the
// units of the ints. Nothing here was executed on the EE.
extern "C" {
extern int scanMask;
void drawTexture__FR6Packetiiiiiiiii(unsigned char* packet, int i1, int i2, int i3, int i4, int i5,
                                    int i6, int i7, int i8, int i9) {
    using namespace rend;
    typedef long long __attribute__((may_alias)) i64;
    const bool odd = (i3 & 0x10) != 0;
    const i64 lowA = static_cast<i64>(i6 + 4) | static_cast<i64>(static_cast<int>(static_cast<unsigned>(i7 + 4) << 16));
    const i64 lowB = static_cast<i64>(i8 + 4) | static_cast<i64>(static_cast<int>(static_cast<unsigned>(i9 + 4) << 16));
    const i64 highA = static_cast<i64>(i2) | static_cast<i64>(static_cast<int>(static_cast<unsigned>(i3) << 16));
    const i64 highB = static_cast<i64>(i4) | static_cast<i64>(static_cast<int>(static_cast<unsigned>(i5) << 16));
    if (scanMask == (odd ? 2 : 3)) {
        unsigned char* cursor = ptr(packet, 0);
        at<i64>(cursor, 0) = lowA;
        at<i64>(cursor, 8) = highA;
        at<i64>(cursor, 16) = lowB;
        at<i64>(cursor, 24) = highB;
        set_ptr(packet, 0, cursor + 32);
    }
    if (scanMask == (odd ? 3 : 2)) {
        const int h = (i1 >> 1) << 4;
        unsigned char* cursor = ptr(packet, 0);
        const i64 lowC = static_cast<i64>(i6 + 4) | static_cast<i64>(static_cast<int>(static_cast<unsigned>(h + i7) << 16));
        const i64 lowD = static_cast<i64>(i8 + 4) | static_cast<i64>(static_cast<int>(static_cast<unsigned>(h + i9) << 16));
        set_ptr(packet, 0, cursor + 32);
        at<i64>(cursor, 0) = lowC;
        at<i64>(cursor, 8) = highA;
        at<i64>(cursor, 16) = lowD;
        at<i64>(cursor, 24) = highB;
    }
}
}
