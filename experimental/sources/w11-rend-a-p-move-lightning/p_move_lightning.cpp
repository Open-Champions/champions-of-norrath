#include "../w11-rend-a-common/rend.hpp"
// Observed P_MoveLightning__FiG6Point3T1ii at 0x00184be0, 244 bytes. Arguments: a bolt handle,
// two Point3 values (each arrives by address of a caller copy and is copied to a local on
// entry), and two ints (here c and w). bolt is the 20-record table (1844 bytes each) at
// 0x006f6e50. A zero handle returns. Record i = handle & 0xff must have the full handle in the
// word at +1840, a nonzero byte at +1836, and nonzero pointer words at +1824 and +1828 (the two
// trail records the bolt owns); any failure returns with no store. Otherwise:
//   word +1832 = frameNm + 2; the first Point3 is copied to +1792..1803 and the second to
//   +1804..1815; byte 1037 of each of the two trail records (header byte +13) is set to the
//   low byte of c; then, only when w >= 0, byte 1039 (header byte +15) of both records is set
//   to w >> 2 (arithmetic).
// Nothing is returned.
// Inferred: this re-aims a live lightning bolt (start and end) and refreshes its expiry frame; c
// and w are a colour byte and a width that P_StartLightning also hands to P_StartTrail.
// Unknown: the meaning of the +1832 word beyond its use as a frame number. Nothing here was
// executed on the EE.
struct Point3 {
    float x, y, z;
};
extern "C" {
extern unsigned char bolt[20 * 1844];
extern int frameNm;
void P_MoveLightning__FiG6Point3T1ii(int handle, Point3* a, Point3* b, int c, int w) {
    using namespace rend;
    Point3 p0 = *a;
    Point3 p1 = *b;
    if (handle == 0) return;
    unsigned char* record = bolt + (handle & 0xff) * 1844;
    if (at<i32>(record, 1840) != handle) return;
    if (at<i8>(record, 1836) == 0) return;
    if (at<i32>(record, 1824) == 0) return;
    if (at<i32>(record, 1828) == 0) return;
    at<i32>(record, 1832) = frameNm + 2;
    { f32* d = &at<f32>(record, 1792); d[0] = p0.x; d[1] = p0.y; d[2] = p0.z; }
    { f32* d = &at<f32>(record, 1804); d[0] = p1.x; d[1] = p1.y; d[2] = p1.z; }
    ptr(record, 1824)[1037] = static_cast<unsigned char>(c);
    ptr(record, 1828)[1037] = static_cast<unsigned char>(c);
    if (w < 0) return;
    const unsigned char shifted = static_cast<unsigned char>(w >> 2);
    ptr(record, 1824)[1039] = shifted;
    ptr(record, 1828)[1039] = shifted;
}
}
