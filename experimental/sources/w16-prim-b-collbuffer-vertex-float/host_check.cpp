// Host check only; not a build input. Pointer words hold arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "vertex.cpp"
#include <initializer_list>
#include <string.h>

alignas(16) static unsigned char arena[0x2000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

int main() {
    for (int type : {0, 1, 3, 2, -1}) {
        for (int n : {0, 1, 9}) {
            memset(arena, 0xee, sizeof arena);
            unsigned char* o = arena + 0x1800;
            unsigned verts = 0x100, entries = 0x400;
            int ent = 3;
            float v[3] = {1.5f, -2.25f, 1e20f};
            memset(o, 0x13, 96);
            memcpy(o + 8, &type, 4);
            memcpy(o + 60, &n, 4);
            memcpy(o + 76, &verts, 4);
            memcpy(o + 72, &entries, 4);
            memcpy(o + 52, &ent, 4);
            unsigned short cnt = 0xffff;
            memcpy(arena + entries + ent * 28 + 2, &cnt, 2);
            unsigned char want[0x2000];
            memcpy(want, arena, sizeof want);
            int ret = -1;
            if (type == 0) {
                ret = 0;
                memcpy(want + verts + n * 12, v, 12);
                int n1 = n + 1;
                memcpy(want + 0x1800 + 60, &n1, 4);
                unsigned short c1 = 0;
                memcpy(want + entries + ent * 28 + 2, &c1, 2);
            }
            if (Vertex__12VICollBufferR7VIVect3(o, v) != ret) return 1;
            if (memcmp(arena, want, sizeof want) != 0) return 2;
        }
    }
    return 0;
}
