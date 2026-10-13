#include <cstdio>
#include <cstring>
#include "../level-anim-load-common/host_check.hpp"
struct Point3 { float x, y, z; };
extern "C" {
unsigned char bolt[20 * 1844];
// Target words are offsets from bolt, so that bolt and trail pointers both survive the round trip.
unsigned char* norrath_host_pointer(unsigned address) { return bolt + static_cast<int>(address - 0x10000000u); }
unsigned norrath_host_address(const unsigned char* p) { return 0x10000000u + static_cast<unsigned>(static_cast<int>(p - bolt)); }
unsigned char trail[66560];
int frameNm, linePos, trailSequenceNm;
signed char netTrueStep;
static int startCalls, extendCalls, deleteCalls, fractalCalls;
static int startArgs[4][8];
static float startTime[4];
static int nextHandles[4];
static int extendHandles[4];
static Point3 extendPoint[4];
static int deleted;
static int fractalCount, linePosSeen;
static float fractalLength;
static unsigned char* fractalRecord;
int P_StartTrail__Fiifiiiii(int a0, int a1, float time, int a2, int a3, int t0, int t1, int t2) {
    int* a = startArgs[startCalls];
    a[0] = a0; a[1] = a1; a[2] = a2; a[3] = a3; a[4] = t0; a[5] = t1; a[6] = t2;
    startTime[startCalls] = time;
    int h = nextHandles[startCalls++];
    if (h) { trail[(h & 0xff) * 1040 + 1024] = h; trail[(h & 0xff) * 1040 + 1025] = h >> 8; }
    return h;
}
void P_ExtendTrail__FiG6Point3(int handle, Point3* p) { extendHandles[extendCalls] = handle; extendPoint[extendCalls++] = *p; }
void P_DeleteTrail__Fi(int handle) { ++deleteCalls; deleted = handle; }
void startFractalLine__FP4Boltfi(unsigned char* record, float length, int count) {
    ++fractalCalls; fractalRecord = record; fractalLength = length; fractalCount = count; linePosSeen = linePos;
}
int P_StartLightning__FG6Point3T0iiffUi(Point3* a, Point3* b, int i, int n, float f1, float f2, unsigned color);
}
static void reset() {
    std::memset(bolt, 0, sizeof bolt);
    startCalls = extendCalls = deleteCalls = fractalCalls = 0;
    linePos = 99; frameNm = 500; trailSequenceNm = 7; netTrueStep = 0;
}
int main() {
    Point3 a = {1, 2, 3}, b = {4, 5, 6};
    // First free record is used; trail handles are recorded as record addresses; result is s | seq << 8.
    reset();
    nextHandles[0] = 0x0309; nextHandles[1] = 0x040a;
    bolt[1836] = 1; bolt[1844 + 1836] = 1;  // records 0 and 1 busy
    netTrueStep = 3;
    int h = P_StartLightning__FG6Point3T0iiffUi(&a, &b, 200, 9, 4.0f, 4.4f, 0x5e808080u);
    unsigned char* rec = bolt + 2 * 1844;
    if (h != (2 | (7 << 8)) || trailSequenceNm != 8) return 1;
    if (hw(rec + 1840) != (unsigned)h || rec[1836] != 1) return 2;
    if (startCalls != 2 || startArgs[0][0] != 0 || startArgs[0][1] != 200 || startTime[0] != 1.0f || startArgs[0][2] != 5) return 3;
    if (startArgs[0][3] != 0x80 || startArgs[0][4] != 0x80 || startArgs[0][5] != 0x80 || startArgs[0][6] != 0x5e) return 4;
    if (std::memcmp(startArgs[0], startArgs[1], sizeof startArgs[0]) != 0) return 5;
    if (hw(rec + 1824) != norrath_host_address(trail + 9 * 1040) || hw(rec + 1828) != norrath_host_address(trail + 10 * 1040)) return 6;
    if (extendCalls != 2 || extendHandles[0] != 0x0309 || extendHandles[1] != 0x040a) return 7;
    if (extendPoint[0].x != 1 || extendPoint[1].z != 3) return 8;
    if (lf_(rec + 1792) != 1 || lf_(rec + 1804) != 4 || lf_(rec + 1812) != 6) return 9;
    if (lf_(rec + 1816) != 4.0f || lf_(rec + 1820) != 4.4f) return 10;
    if (rec[1837] != 5 || fractalCount != 5 || fractalLength != 4.0f || linePosSeen != 0 || linePos != 0) return 11;
    if (hw(rec + 1832) != 510 || rec[1838] != 1) return 12;
    if (fractalRecord != rec) return 13;
    // All records busy: nothing happens and 0 is returned.
    reset();
    for (int k = 0; k != 20; ++k) bolt[k * 1844 + 1836] = 1;
    if (P_StartLightning__FG6Point3T0iiffUi(&a, &b, 1, 1, 1, 1, 1) != 0 || startCalls != 0 || trailSequenceNm != 7) return 14;
    // First trail fails: the record is released again.
    reset();
    nextHandles[0] = 0;
    if (P_StartLightning__FG6Point3T0iiffUi(&a, &b, 1, 1, 1, 1, 1) != 0 || bolt[1836] != 0 || startCalls != 1 || extendCalls != 0) return 15;
    // Second trail fails: the first one is deleted by its header handle, and the record is released.
    reset();
    nextHandles[0] = 0x0305; nextHandles[1] = 0;
    if (P_StartLightning__FG6Point3T0iiffUi(&a, &b, 1, 1, 1, 1, 1) != 0 || deleteCalls != 1 || deleted != 0x0305) return 16;
    if (bolt[1836] != 0 || hw(bolt + 1824) != 0 || fractalCalls != 0 || trailSequenceNm != 7) return 17;
    // A small count is kept and a negative one too.
    reset();
    nextHandles[0] = 0x0301; nextHandles[1] = 0x0302;
    P_StartLightning__FG6Point3T0iiffUi(&a, &b, 1, -3, 1, 1, 1);
    if (bolt[1837] != 0xfd || fractalCount != -3 || bolt[1838] != 0) return 18;
    std::puts("P_StartLightning contract passed");
    return 0;
}
