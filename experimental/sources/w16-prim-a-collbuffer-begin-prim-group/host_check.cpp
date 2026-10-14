// Host check only; not a build input. Pointer words hold arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "begin_prim_group.cpp"
#include <initializer_list>
#include <string.h>

alignas(16) static unsigned char arena[0x1000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

int main() {
    for (int index : {0, 1, 5, 30}) {
        memset(arena, 0xee, sizeof arena);
        unsigned char* o = arena + 0x800;
        unsigned groups = 0x100;
        memset(o, 0x13, 96);
        memcpy(o + 68, &groups, 4);
        memcpy(o + 44, &index, 4);
        unsigned char want[0x1000];
        memcpy(want, arena, sizeof want);
        memset(want + groups + index * 32, 0, 8);
        if (BeginPrimGroup__12VICollBuffer(o) != 0) return 1;
        if (memcmp(arena, want, sizeof want) != 0) return 2;
    }
    return 0;
}
