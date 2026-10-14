// Host check only; not a build input. Pointer words hold arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "begin.cpp"
#include <initializer_list>
#include <string.h>

alignas(16) static unsigned char arena[0x1000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

int main() {
    for (int type : {-1, 0, 1, 2, 3, 4}) {
        for (int index : {0, 1, 6}) {
            for (int value : {0, 5, 0x12345, -2}) {
                memset(arena, 0xa5, sizeof arena);
                unsigned char* o = arena + 0x800;
                unsigned entries = 0x100;
                memcpy(o + 8, &type, 4);
                memcpy(o + 52, &index, 4);
                memcpy(o + 72, &entries, 4);
                unsigned char want[sizeof arena];
                memcpy(want, arena, sizeof want);
                bool ok = type == 0 || type == 1 || type == 3;
                if (ok) {
                    short lo = static_cast<short>(value);
                    memcpy(want + entries + index * 28, &lo, 2);
                    memset(want + entries + index * 28 + 2, 0, 2);
                }
                if (Begin__12VICollBufferi(o, value) != (ok ? 0 : -1)) return 1;
                if (memcmp(arena, want, sizeof want) != 0) return 2;
            }
        }
    }
    return 0;
}
