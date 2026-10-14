#include "../level-anim-load-common/mem.hpp"
// Observed P_StartTrail__Fiifiiiii at 0x001828d8, 256 bytes. Arguments: integers a0, a1,
// a float, then integers a2, a3, t0, t1, t2. The table trail (0x00780b00) holds 64
// records of 1040 bytes; a 16-byte header sits at record offset 1024. The routine scans
// the headers for the first whose halfword at +0 is zero. With none free it returns 0.
// Otherwise with slot i and the counter trailSequenceNm (word 0x00484310) it stores:
// +0 halfword (counter << 8) | i; +2 halfword 0; +4 byte 0; +5 byte 0; +6 byte a0;
// +7 byte a2; +8 halfword (int)(float * 60.0f) truncated, low 16 bits; +10..+13 bytes
// a3, t0, t1, t2; +15 byte a1 >> 2 (arithmetic). The counter is incremented and the
// halfword at +0 is returned sign-extended.
// Inferred: the +0 halfword is a trail handle (sequence number plus slot) that P_ExtendTrail
// compares; +4/+5 are the read and write positions of a ring of points. Unknown: the
// meaning of the other header fields and what the float (scaled by 60, so a time in
// seconds at 60 Hz) represents.
extern "C" {
extern unsigned char trail[66560];
extern int trailSequenceNm;
int P_StartTrail__Fiifiiiii(int a0, int a1, float time, int a2, int a3, int t0, int t1, int t2) {
    using namespace norrath_lal;
    int slot = 0;
    while (slot < 64 && load_signed_half(trail + slot * 1040 + 1024) != 0) ++slot;
    if (slot == 64) return 0;
    unsigned char* header = trail + slot * 1040 + 1024;
    const int sequence = trailSequenceNm;
    const int frames = static_cast<int>(time * 60.0f);
    header[10] = static_cast<unsigned char>(a3);
    header[11] = static_cast<unsigned char>(t0);
    header[12] = static_cast<unsigned char>(t1);
    header[13] = static_cast<unsigned char>(t2);
    store_half(header + 8, static_cast<unsigned>(frames));
    store_half(header + 0, static_cast<unsigned>((sequence << 8) | slot));
    header[15] = static_cast<unsigned char>(a1 >> 2);
    header[6] = static_cast<unsigned char>(a0);
    header[7] = static_cast<unsigned char>(a2);
    header[5] = 0;
    header[4] = 0;
    store_half(header + 2, 0);
    trailSequenceNm = sequence + 1;
    return load_signed_half(header + 0);
}
}
