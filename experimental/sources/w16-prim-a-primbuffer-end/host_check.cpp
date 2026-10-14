// Host check only; not a build input. Pointer words hold arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
int PrimVQStartDef __asm__("_12VIPrimBuffer$PrimVQStart");
unsigned S1 __asm__("_8VIRaster$RStripOffset");
#include "end.cpp"
#include <initializer_list>
#include <string.h>

alignas(16) static unsigned char arena[0x4000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

// Each strip offset is its own 4-byte static.
unsigned S2 __asm__("_8VIRaster$RStrip2Offset");
unsigned S3 __asm__("_8VIRaster$RStrip3Offset");
unsigned S4 __asm__("_8VIRaster$RStrip4Offset");
unsigned S6 __asm__("_8VIRaster$RStrip6Offset");

int main() {
    for (unsigned type : {0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u}) {
        memset(arena, 0x5d, sizeof arena);
        unsigned char* o = arena + 0x3000;
        unsigned notes = 0x100, headers = 0x800, mats = 0x400;
        int n = 2, cursor = 9, m = 1, vq = 3;
        memcpy(o + 8, &type, 4);
        memcpy(o + 64, &m, 4);
        memcpy(o + 72, &n, 4);
        memcpy(o + 80, &cursor, 4);
        memcpy(o + 88, &mats, 4);
        memcpy(o + 92, &notes, 4);
        memcpy(o + 96, &headers, 4);
        unsigned count = 0x23;
        memcpy(arena + notes + n * 28, &count, 4);
        int mat1 = 40;
        memcpy(arena + mats + m * 16 + 4, &mat1, 4);
        PrimVQStartDef = vq;
        S1 = 0x1000; S2 = 0x2000; S3 = 0x3000; S4 = 0x4000; S6 = 0x6000;
        unsigned strip = type == 0 ? 0x1000 : type == 2 ? 0x2000 : type == 4 ? 0x3000 : type == 5 ? 0x4000 : 0x6000;
        unsigned char want[sizeof arena];
        memcpy(want, arena, sizeof want);
        bool ok = type == 0 || type == 2 || type == 4 || type == 5 || type == 8;
        if (ok) {
            unsigned tag = type == 0 ? (((count << 18) + 0x10000u) | 0x6c008000u) : 0x6c018000u;
            memcpy(want + headers + vq * 16 + 12, &tag, 4);
            memcpy(want + headers + (vq + 1) * 16, &count, 4);
            memset(want + headers + (vq + 1) * 16 + 4, 0, 12);
            memset(want + headers + cursor * 16, 0, 12);
            unsigned last = strip | 0x14000000u;
            memcpy(want + headers + cursor * 16 + 12, &last, 4);
            int c1 = cursor + 1, n1 = n + 1, m1 = mat1 + 1;
            memcpy(want + 0x3000 + 80, &c1, 4);
            memcpy(want + 0x3000 + 72, &n1, 4);
            memcpy(want + mats + m * 16 + 4, &m1, 4);
        }
        if (End__12VIPrimBuffer(o) != (ok ? 0 : -1)) return 1;
        if (memcmp(arena, want, sizeof want) != 0) return 2;
        if (PrimVQStartDef != vq) return 3;
    }
    return 0;
}
