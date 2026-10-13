#include "../w11-rend-a-common/rend.hpp"
// Observed modelSetupAttachment__FP6PacketP12_modelHeaderT1i at 0x00169528, extent 800.
// Arguments: a Packet (words at +0 cursor, +4 and +8 are written; the caller's frame holds 16
// bytes), a model header m (the attachment), a second model header n, and an int index.
// Returns nothing. Header fields used: m: halfword +16 (bit 0x10 = flag), signed byte +19,
// doubleword +24, byte +32; n: byte +34, and a table of 52-byte records at n + byte34 * 16 + k * 52.
// Globals: modelOption (word; bit 0x80 mirrors three rows, bit 0x40 adds character lighting) and
// attachmentLightPos (a Point3). The cursor is a byte address; "aligned" means a multiple of 16.
//  1. head = cursor. *(head + 8) = 0x11000000; packet[+4] = head; t = head + 12; packet[+0] = t.
//     Zero bytes are stored from t until (t & 15) == 12 (the packet cursor is not moved by these).
//  2. At t: word 0x50000004, or 0x50000005 when flag is set. At g = t + 4 (16-aligned): doublewords
//     0x1000000000008000 and 14. Then at t + 20: 0, 63; t + 36: 0, 6; t + 52: the doubleword at
//     m+24, then 66. packet[+8] = g. The cursor becomes t + 68. When flag is set, at t + 68 the
//     doubleword ((m byte +32) << 32) | 100 and at t + 76 the doubleword 67 follow and the cursor
//     becomes t + 84.
//  3. g's first doubleword gets ((cursor - g) >> 3 - 2) >> 1 ORed in (a quadword count minus one).
//  4. When the signed byte m+19 is not -1: k = index, or m+19 when index is -1. Let r be the record
//     for k in n. At the cursor c: word 0x6001c000; word at c + 4 = (word at r) << 2; word at
//     c + 8 = 0x6c03c001; then twelve floats at c + 12, 16, ... 56 taken from r + 4 plus the
//     offsets 0, 12, 24, 36, 4, 16, 28, 40, 8, 20, 32, 44 in that order (a 3x4 block read
//     transposed), with the three floats read from offsets 0, 4 and 8 negated when modelOption has
//     bit 0x80. Then word 0x1400001a at c + 60 and the cursor becomes c + 64.
//  5. When modelOption has bit 0x40, lightSetupForCharacter(packet, &attachmentLightPos).
//  6. Zero bytes are stored from the cursor until it is 16-aligned. The first doubleword at
//     packet[+4] becomes 0x10000000 | (((cursor - packet[+4]) >> 3 - 2) >> 1) and the packet
//     cursor is the aligned value.
// Inferred: builds a short DMA packet (a header tag, a GIF-like block with register writes, the
// attachment's 3x4 matrix, and optional character lighting) for modelDrawAttachment; the three
// negated entries look like a mirror about one axis. Unknown: what each tag word selects, the units
// of the model fields, and the roles of +16 bit 4 and the doubleword at +24. Nothing here was
// executed on the EE.
struct Point3 {
    float x, y, z;
};
typedef unsigned long long __attribute__((may_alias)) u64;
extern "C" {
extern int modelOption;
extern Point3 attachmentLightPos;
void lightSetupForCharacter__FP6PacketG6Point3(void* packet, const Point3* point);
}
static inline unsigned addr(const unsigned char* p) {
#ifdef NORRATH_HOST_ARENA
    return norrath_host_address(p);
#else
    return reinterpret_cast<unsigned>(p);
#endif
}
extern "C" void modelSetupAttachment__FP6PacketP12_modelHeaderT1i(unsigned char* packet, const unsigned char* m,
                                                                   const unsigned char* n, int index) {
    using namespace rend;
    unsigned char* head = ptr(packet, 0);
    at<u32>(head, 8) = 0x11000000u;
    set_ptr(packet, 4, head);
    unsigned char* t = head + 12;
    set_ptr(packet, 0, t);
    while ((addr(t) & 0xf) != 12) *t++ = 0;
    const bool flag = (at<u16>(m, 16) & 0x10) != 0;
    unsigned char* g = t + 4;
    at<u64>(g, 0) = 0x1000000000008000ull;
    at<u64>(g, 8) = 14;
    at<u64>(t, 20) = 0;
    at<u64>(t, 28) = 63;
    at<u64>(t, 36) = 0;
    at<u64>(t, 44) = 6;
    at<u32>(t, 0) = 0x50000000u | (flag ? 5u : 4u);
    at<u64>(t, 52) = at<u64>(m, 24);
    at<u64>(t, 60) = 66;
    set_ptr(packet, 8, g);
    unsigned char* cursor = t + 68;
    if (flag) {
        at<u64>(t, 68) = (static_cast<u64>(m[32]) << 32) | 100u;
        at<u64>(t, 76) = 67;
        cursor = t + 84;
    }
    set_ptr(packet, 0, cursor);
    at<u64>(g, 0) |= static_cast<u64>(static_cast<long long>((((static_cast<int>(addr(cursor) - addr(g))) >> 3) - 2) >> 1));
    const int b19 = at<i8>(m, 19);
    if (b19 != -1) {
        const int k = (index != -1) ? index : b19;
        const unsigned char* r = n + n[34] * 16 + k * 52;
        unsigned char* c = cursor;
        at<u32>(c, 0) = 0x6001c000u;
        at<u32>(c, 4) = at<u32>(r, 0) << 2;
        at<u32>(c, 8) = 0x6c03c001u;
        set_ptr(packet, 0, c + 12);
        const unsigned char* s = r + 4;
        const bool flip = (modelOption & 0x80) != 0;
        at<f32>(c, 12) = flip ? -at<f32>(s, 0) : at<f32>(s, 0);
        at<f32>(c, 16) = at<f32>(s, 12);
        at<f32>(c, 20) = at<f32>(s, 24);
        at<f32>(c, 24) = at<f32>(s, 36);
        at<f32>(c, 28) = flip ? -at<f32>(s, 4) : at<f32>(s, 4);
        at<f32>(c, 32) = at<f32>(s, 16);
        at<f32>(c, 36) = at<f32>(s, 28);
        at<f32>(c, 40) = at<f32>(s, 40);
        at<f32>(c, 44) = flip ? -at<f32>(s, 8) : at<f32>(s, 8);
        at<f32>(c, 48) = at<f32>(s, 20);
        at<f32>(c, 52) = at<f32>(s, 32);
        at<f32>(c, 56) = at<f32>(s, 44);
        set_ptr(packet, 0, c + 60);
        at<u32>(c, 60) = 0x1400001au;
        set_ptr(packet, 0, c + 64);
    }
    if ((modelOption & 0x40) != 0) lightSetupForCharacter__FP6PacketG6Point3(packet, &attachmentLightPos);
    unsigned char* end = ptr(packet, 0);
    while ((addr(end) & 0xf) != 0) *end++ = 0;
    unsigned char* start = ptr(packet, 4);
    set_ptr(packet, 0, end);
    at<u64>(start, 0) = 0x10000000ull | static_cast<u64>(static_cast<long long>((((static_cast<int>(addr(end) - addr(start))) >> 3) - 2) >> 1));
}
