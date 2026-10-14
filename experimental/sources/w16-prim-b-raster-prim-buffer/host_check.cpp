// Host check only; not a build input. Pointer words hold arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "prim_buffer.cpp"
#include <initializer_list>
#include <string.h>

alignas(16) static unsigned char arena[0x8000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

int main() {
    memset(arena, 0xee, sizeof arena);
    unsigned char* o = arena + 0x1000;
    unsigned table = 0x200;
    memcpy(o + 19328, &table, 4);
    for (int i : {0, 1, 5, 40}) {
        int v = 0x1000 + i * 3;
        memcpy(arena + table + i * 12, &v, 4);
    }
    for (int i : {0, 1, 5, 40})
        if (PrimBuffer__8VIRasteri(o, i) != 0x1000 + i * 3) return 1 + i;
    return 0;
}
