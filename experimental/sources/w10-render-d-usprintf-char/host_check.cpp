// Host check: a stub of the 16-bit formatter records its arguments; the wrapper must widen bytes
// (including values above 0x7f, zero-extended), terminate the buffer and forward all eight floats.
#include <cstdio>
#include <cstdlib>
#include <cstring>
typedef unsigned short u16;
static u16 seen[300];
static int seenLen;
static u16* seenDst;
static float seenF[8];
extern "C" void usprintf__FPUsT0ffffffff(u16* dst, u16* format, float f0, float f1, float f2, float f3,
                                         float f4, float f5, float f6, float f7) {
    seenDst = dst;
    for (seenLen = 0; format[seenLen] != 0; ++seenLen) seen[seenLen] = format[seenLen];
    seen[seenLen] = 0;
    // the terminator unit must be a zero immediately after the last widened byte
    float f[8] = {f0, f1, f2, f3, f4, f5, f6, f7};
    memcpy(seenF, f, sizeof f);
}
extern "C" void usprintf__FPUsPcffffffff(u16*, char*, float, float, float, float, float, float, float, float);
static int fails;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
int main() {
    u16 dst[4];
    char fmt[] = {'a', '%', 'd', (char)0xe9, (char)0x80, 0};
    usprintf__FPUsPcffffffff(dst, fmt, 1, 2, 3, 4, 5, 6, 7, 8);
    CHECK(seenDst == dst);
    CHECK(seenLen == 5);
    CHECK(seen[0] == 'a' && seen[1] == '%' && seen[2] == 'd');
    CHECK(seen[3] == 0xe9 && seen[4] == 0x80);
    for (int i = 0; i < 8; ++i) CHECK(seenF[i] == (float)(i + 1));
    char empty[] = {0};
    usprintf__FPUsPcffffffff(dst, empty, 0, 0, 0, 0, 0, 0, 0, 0);
    CHECK(seenLen == 0);
    char big[256];
    memset(big, 'x', 255); big[255] = 0;
    usprintf__FPUsPcffffffff(dst, big, 0, 0, 0, 0, 0, 0, 0, 0);
    CHECK(seenLen == 255);
    if (fails) return 1;
    puts("usprintf char wrapper ok");
    return 0;
}
