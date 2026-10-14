// Host check only; not a build input. Pointer words hold arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "end_material.cpp"
#include <initializer_list>
#include <string.h>

alignas(16) static unsigned char arena[0x1000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

int main() {
    for (int index : {0, 1, 9}) {
        for (int count : {0, 100, -5}) {
            memset(arena, 0x3c, sizeof arena);
            unsigned char* o = arena + 0x800;
            unsigned records = 0x100;
            int began = 40;
            memcpy(o + 64, &index, 4);
            memcpy(o + 80, &count, 4);
            memcpy(o + 88, &records, 4);
            memcpy(arena + records + index * 16 + 8, &began, 4);
            unsigned char want[sizeof arena];
            memcpy(want, arena, sizeof want);
            int len = count - began, next = index + 1;
            memcpy(want + records + index * 16 + 12, &len, 4);
            memcpy(want + 0x800 + 64, &next, 4);
            if (EndMaterial__12VIPrimBuffer(o) != 0) return 1;
            if (memcmp(arena, want, sizeof want) != 0) return 2;
        }
    }
    return 0;
}
