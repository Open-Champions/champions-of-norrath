#include "../level-anim-load-common/mem.hpp"
// Observed P_ExtendTrail__FiG6Point3 at 0x00183dd0, 252 bytes. The Point3 argument is
// passed by address in a1 (three floats, copied to the stack on entry). A zero handle
// returns. Otherwise slot = handle & 0xff selects the header at trail + slot*1040 + 1024.
// The header halfword at +0 must equal the full handle, else the call returns. With
// head = byte +5 and tail = byte +4, the call also returns if (head + 1) & 0x7f or
// (head + 2) & 0x7f equals tail (the ring holds 128 points and keeps one slot free).
// Otherwise the point at record offset head*8 receives three halfwords: (int)(2*x),
// (int)(2*y), (int)(2*z), each truncated to 16 bits, then the halfword (header +2) + 1 as
// a fourth, and byte +5 becomes (head + 1) & 0x7f; the header halfword at +2 itself is
// not changed. Nothing is returned.
// Inferred: a ring of 128 trail points with a write index at +5 and read index at +4, and
// the +2 halfword as a running point count; points are stored at half-unit resolution.
// Unknown: the meaning of the fourth halfword beyond that arithmetic.
extern "C" {
extern unsigned char trail[66560];
void P_ExtendTrail__FiG6Point3(int handle, const unsigned char* point) {
    using namespace norrath_lal;
    const float x = load_float(point + 0);
    const float y = load_float(point + 4);
    const float z = load_float(point + 8);
    if (handle == 0) return;
    unsigned char* record = trail + (handle & 0xff) * 1040;
    unsigned char* header = record + 1024;
    if (load_signed_half(header + 0) != handle) return;
    const unsigned head = header[5];
    const unsigned tail = header[4];
    if (((head + 1) & 0x7f) == tail || ((head + 2) & 0x7f) == tail) return;
    const float x2 = x + x, y2 = y + y, z2 = z + z;
    const int ix = static_cast<int>(x2), iy = static_cast<int>(y2), iz = static_cast<int>(z2);
    store_half(record + head * 8 + 0, static_cast<unsigned>(ix));
    store_half(record + header[5] * 8 + 2, static_cast<unsigned>(iy));
    store_half(record + header[5] * 8 + 4, static_cast<unsigned>(iz));
    store_half(record + header[5] * 8 + 6, load_half(header + 2) + 1u);
    header[5] = static_cast<unsigned char>((header[5] + 1) & 0x7f);
}
}
