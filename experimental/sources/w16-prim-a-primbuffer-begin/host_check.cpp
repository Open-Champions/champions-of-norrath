// Host check only; not a build input. Pointer words hold arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
int PrimVQStartDef __asm__("_12VIPrimBuffer$PrimVQStart");
#include "begin.cpp"
#include <initializer_list>
#include <string.h>

alignas(16) static unsigned char arena[0x2000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

int main() {
    for (unsigned type : {0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u, 0xffffffffu}) {
        for (int cursor : {0, 3, 10}) {
            memset(arena, 0x5d, sizeof arena);
            unsigned char* o = arena + 0x1800;
            unsigned counts = 0x100, records = 0x400;
            int entry = 2;
            memcpy(o + 8, &type, 4);
            memcpy(o + 72, &entry, 4);
            memcpy(o + 80, &cursor, 4);
            memcpy(o + 92, &counts, 4);
            memcpy(o + 96, &records, 4);
            PrimVQStartDef = -77;
            unsigned char want[sizeof arena];
            memcpy(want, arena, sizeof want);
            bool ok = type == 0 || type == 2 || type == 4 || type == 5 || type == 8;
            int expect_vq = -77;
            if (ok) {
                memset(want + counts + entry * 28, 0, 4);
                memset(want + records + cursor * 16, 0, 32);
                int next = cursor + 2;
                memcpy(want + 0x1800 + 80, &next, 4);
                expect_vq = cursor;
            }
            if (Begin__12VIPrimBufferi(o, 12345) != (ok ? 0 : -1)) return 1;
            if (memcmp(arena, want, sizeof want) != 0) return 2;
            if (PrimVQStartDef != expect_vq) return 3;
        }
    }
    return 0;
}
