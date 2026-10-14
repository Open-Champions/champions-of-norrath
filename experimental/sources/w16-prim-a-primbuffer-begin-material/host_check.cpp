// Host check only; not a build input. Pointer words hold arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "begin_material.cpp"
#include <initializer_list>
#include <string.h>

alignas(16) static unsigned char arena[0x1000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

int main() {
    for (int index : {0, 1, 9}) {
        for (int material : {0, 7, -3}) {
            memset(arena, 0x3c, sizeof arena);
            unsigned char* o = arena + 0x800;
            unsigned records = 0x100;
            int count = 123;
            memcpy(o + 64, &index, 4);
            memcpy(o + 80, &count, 4);
            memcpy(o + 88, &records, 4);
            unsigned char want[sizeof arena];
            memcpy(want, arena, sizeof want);
            int rec[4] = {material, 0, count, 0};
            memcpy(want + records + index * 16, rec, 16);
            if (BeginMaterial__12VIPrimBufferi(o, material) != 0) return 1;
            if (memcmp(arena, want, sizeof want) != 0) return 2;
        }
    }
    return 0;
}
