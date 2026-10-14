// Host check only; not a build input. Pointer words hold arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "end.cpp"
#include <initializer_list>
#include <string.h>

alignas(16) static unsigned char arena[0x2000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

int main() {
    for (int type : {-1, 0, 1, 2, 3, 4}) {
        for (int group : {0, 3}) {
            for (int entry : {0, 2, 7}) {
                for (short count : {0, 5, -9, 0x7fff}) {
                    memset(arena, 0x6b, sizeof arena);
                    unsigned char* o = arena + 0x1800;
                    unsigned groups = 0x100, entries = 0x400;
                    memcpy(o + 8, &type, 4);
                    memcpy(o + 44, &group, 4);
                    memcpy(o + 52, &entry, 4);
                    memcpy(o + 68, &groups, 4);
                    memcpy(o + 72, &entries, 4);
                    int g0 = 10, g1 = 100;
                    memcpy(arena + groups + group * 32, &g0, 4);
                    memcpy(arena + groups + group * 32 + 4, &g1, 4);
                    memcpy(arena + entries + entry * 28 + 2, &count, 2);
                    unsigned char want[sizeof arena];
                    memcpy(want, arena, sizeof want);
                    bool ok = type == 0 || type == 1 || type == 3;
                    if (ok) {
                        int a = g0 + 1, b = g1 + count, e = entry + 1;
                        memcpy(want + groups + group * 32, &a, 4);
                        memcpy(want + groups + group * 32 + 4, &b, 4);
                        memcpy(want + 0x1800 + 52, &e, 4);
                    }
                    if (End__12VICollBuffer(o) != (ok ? 0 : -1)) return 1;
                    if (memcmp(arena, want, sizeof want) != 0) return 2;
                }
            }
        }
    }
    return 0;
}
