#include "../w11-rend-a-common/rend.hpp"
// Observed P_StartLightning__FG6Point3T0iiffUi at 0x001848c8, 684 bytes. Arguments: two Point3
// values (each by address of a caller copy; copied to locals on entry, here p0 and p1), ints
// i (a2) and n (a3), floats f1 and f2, and a 32-bit colour whose bytes from lowest to highest
// are c0..c3. bolt is the 20-record table (1844 bytes each) at 0x006f6e50.
//  1. The first record whose byte +1836 is zero is chosen (index s); with all twenty in use the
//     call returns 0 at once.
//  2. Byte +1836 is set to 1, then P_StartTrail__Fiifiiiii(0, i, 1.0f, 5, c0, c1, c2, c3) is called.
//     A zero result clears byte +1836 and returns 0.
//  3. The word +1824 receives the address of that trail's record (trail + (handle & 0xff)*1040);
//     P_ExtendTrail__FiG6Point3(the signed halfword at record+1024, &p0) is called. A second
//     P_StartTrail with the same arguments follows; a zero result calls P_DeleteTrail with the
//     halfword at the first record+1024, clears +1824 and +1836 and returns 0. Otherwise +1828
//     receives the second record's address and P_ExtendTrail is called on it with &p0.
//  4. p0 is copied to +1792..1803, p1 to +1804..1815, f1 to +1816 and f2 to +1820; byte +1837 is
//     min(n, 5) (signed); linePos is set to 0 and startFractalLine__FP4Boltfi(record, f1,
//     (signed char) the stored byte) is called; word +1832 = frameNm + 10; byte +1838 is 1 when
//     the signed byte netTrueStep is nonzero, else 0.
//  5. The handle is s | (trailSequenceNm << 8), trailSequenceNm (the counter P_StartTrail also
//     uses) is incremented, the handle is stored at +1840 and returned.
// Inferred: this starts a jagged lightning bolt from p0 to p1 drawn as two coloured trails.
// Unknown: the roles of i, f1 and f2 beyond their use above, the byte at +1838's meaning, and
// whether the second trail is a glow. Nothing here was executed on the EE.
struct Point3 {
    float x, y, z;
};
extern "C" {
extern unsigned char bolt[20 * 1844];
extern unsigned char trail[66560];
extern int frameNm;
extern int linePos;
extern int trailSequenceNm;
extern signed char netTrueStep;
int P_StartTrail__Fiifiiiii(int a0, int a1, float time, int a2, int a3, int t0, int t1, int t2);
void P_ExtendTrail__FiG6Point3(int handle, Point3* point);
void P_DeleteTrail__Fi(int handle);
void startFractalLine__FP4Boltfi(unsigned char* record, float length, int count);
int P_StartLightning__FG6Point3T0iiffUi(Point3* a, Point3* b, int i, int n, float f1, float f2, unsigned color) {
    using namespace rend;
    Point3 p0 = *a;
    Point3 p1 = *b;
    const int c0 = color & 0xff;
    const int c1 = (color >> 8) & 0xff;
    const int c2 = (color >> 16) & 0xff;
    const int c3 = color >> 24;
    int s = 0;
    if (at<i8>(bolt, 1836) != 0) {
        ++s;
        while (s < 20 && at<i8>(bolt + s * 1844, 1836) != 0) ++s;
    }
    if (s == 20) return 0;
    unsigned char* record = bolt + s * 1844;
    at<i8>(record, 1836) = 1;
    int first = P_StartTrail__Fiifiiiii(0, i, 1.0f, 5, c0, c1, c2, c3);
    if (first == 0) {
        at<i8>(record, 1836) = 0;
        return 0;
    }
    unsigned char* firstRecord = trail + (first & 0xff) * 1040;
    set_ptr(record, 1824, firstRecord);
    P_ExtendTrail__FiG6Point3(at<i16>(firstRecord, 1024), &p0);
    int second = P_StartTrail__Fiifiiiii(0, i, 1.0f, 5, c0, c1, c2, c3);
    if (second == 0) {
        P_DeleteTrail__Fi(at<i16>(ptr(record, 1824), 1024));
        at<i32>(record, 1824) = 0;
        at<i8>(record, 1836) = 0;
        return 0;
    }
    unsigned char* secondRecord = trail + (second & 0xff) * 1040;
    set_ptr(record, 1828, secondRecord);
    P_ExtendTrail__FiG6Point3(at<i16>(secondRecord, 1024), &p0);
    {
        f32* d = &at<f32>(record, 1792);
        d[0] = p0.x; d[1] = p0.y; d[2] = p0.z;
        f32* e = &at<f32>(record, 1804);
        e[0] = p1.x; e[1] = p1.y; e[2] = p1.z;
    }
    int count = n > 5 ? 5 : n;
    at<f32>(record, 1816) = f1;
    at<f32>(record, 1820) = f2;
    at<i8>(record, 1837) = static_cast<signed char>(count);
    linePos = 0;
    startFractalLine__FP4Boltfi(record, f1, at<i8>(record, 1837));
    at<i32>(record, 1832) = frameNm + 10;
    at<i8>(record, 1838) = netTrueStep != 0 ? 1 : 0;
    const int sequence = trailSequenceNm;
    const int handle = s | (sequence << 8);
    trailSequenceNm = sequence + 1;
    at<i32>(record, 1840) = handle;
    return handle;
}
}
