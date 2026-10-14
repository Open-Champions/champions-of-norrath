// Host check only; not a build input. Pointer words hold arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include <initializer_list>
#include <string.h>
float PrimUV0[2] __asm__("_12VIPrimBuffer$PrimUV0");
float PrimNormal[3] __asm__("_12VIPrimBuffer$PrimNormal");
unsigned char PrimColor[4] __asm__("_12VIPrimBuffer$PrimColor");
#include "vertex.cpp"

alignas(16) static unsigned char arena[0x2000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

int main() {
    unsigned scalebits = 0x3b808080u;
    float scale;
    memcpy(&scale, &scalebits, 4);
    for (int type : {0, 1, 4, -1}) {
        for (int idx : {0, 3, 10}) {
            memset(arena, 0xee, sizeof arena);
            unsigned char* o = arena + 0x1800;
            unsigned entries = 0x400, slots = 0x100;
            int ent = 2;
            float v[3] = {1.5f, -2.25f, 77.0f};
            PrimUV0[0] = 0.25f; PrimUV0[1] = 0.75f;
            PrimNormal[0] = 0.5f; PrimNormal[1] = -0.5f; PrimNormal[2] = 1.0f;
            PrimColor[0] = 255; PrimColor[1] = 0; PrimColor[2] = 128; PrimColor[3] = 7;
            memset(o, 0x13, 128);
            memcpy(o + 8, &type, 4);
            memcpy(o + 72, &ent, 4);
            memcpy(o + 80, &idx, 4);
            memcpy(o + 92, &entries, 4);
            memcpy(o + 96, &slots, 4);
            int cnt = 41;
            memcpy(arena + entries + ent * 28, &cnt, 4);
            unsigned char want[0x2000];
            memcpy(want, arena, sizeof want);
            int ret = -1;
            if (type == 0) {
                ret = 0;
                int c1 = 42, i1 = idx + 4;
                memcpy(want + entries + ent * 28, &c1, 4);
                memcpy(want + 0x1800 + 80, &i1, 4);
                float s[16] = {1.5f, -2.25f, 77.0f, 1.0f, 0.25f, 0.75f, 1.0f, 0.0f,
                               0.5f, -0.5f, 1.0f, 0.0f, 255 * scale, 0 * scale, 128 * scale, 7 * scale};
                memcpy(want + slots + idx * 16, s, sizeof s);
            }
            if (Vertex__12VIPrimBufferR7VIVect3(o, v) != ret) return 1;
            if (memcmp(arena, want, sizeof want) != 0) return 2;
        }
    }
    return 0;
}
