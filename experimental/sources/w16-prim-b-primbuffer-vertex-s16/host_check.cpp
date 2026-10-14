// Host check only; not a build input. Pointer words hold arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include <initializer_list>
#include <string.h>
unsigned short PrimUV0S16[2] __asm__("_12VIPrimBuffer$PrimUV0S16");
unsigned char PrimNormalS8[3] __asm__("_12VIPrimBuffer$PrimNormalS8");
unsigned char PrimColor[4] __asm__("_12VIPrimBuffer$PrimColor");
unsigned char PrimBones[4] __asm__("_12VIPrimBuffer$PrimBones");
unsigned char PrimWeightsS8[4] __asm__("_12VIPrimBuffer$PrimWeightsS8");
unsigned short PrimVertexGroup __asm__("_12VIPrimBuffer$PrimVertexGroup");
#include "vertex.cpp"

alignas(16) static unsigned char arena[0x2000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

static void put32(unsigned char* p, unsigned v) { memcpy(p, &v, 4); }
static void put16(unsigned char* p, unsigned v) { unsigned short s = static_cast<unsigned short>(v); memcpy(p, &s, 2); }

int main() {
    for (int type : {0, 1, 2, 3, 4, 5, 6, 8, -1}) {
        for (int idx : {0, 3}) {
            for (unsigned char bone : {0, 255, 9}) {
                memset(arena, 0xee, sizeof arena);
                unsigned char* o = arena + 0x1800;
                unsigned entries = 0x400, slots = 0x100;
                int ent = 2;
                unsigned short p[3] = {0x1234, 0xfffe, 7};
                PrimUV0S16[0] = 0x0abc; PrimUV0S16[1] = 0xf00d;
                PrimNormalS8[0] = 0x81; PrimNormalS8[1] = 0x7f; PrimNormalS8[2] = 0x22;
                PrimColor[0] = 255; PrimColor[1] = 1; PrimColor[2] = 128; PrimColor[3] = 7;
                PrimBones[0] = bone; PrimBones[1] = 255; PrimBones[2] = 3; PrimBones[3] = bone;
                PrimWeightsS8[0] = 10; PrimWeightsS8[1] = 20; PrimWeightsS8[2] = 30; PrimWeightsS8[3] = 40;
                PrimVertexGroup = 0xbeef;
                memset(o, 0x13, 128);
                float s48 = 0.125f, s52 = 0.0009765625f;
                memcpy(o + 48, &s48, 4);
                memcpy(o + 52, &s52, 4);
                memcpy(o + 8, &type, 4);
                memcpy(o + 72, &ent, 4);
                memcpy(o + 80, &idx, 4);
                memcpy(o + 92, &entries, 4);
                memcpy(o + 96, &slots, 4);
                unsigned E = 41;
                put32(arena + entries + ent * 28, E);
                unsigned char want[0x2000];
                memcpy(want, arena, sizeof want);
                int ret = -1;
                unsigned char* v = want + slots + idx * 16;
                int adv = 0;
                if (type == 8) {
                    adv = 1;
                    put32(v, (E * 2 + 1) | 0x69028000u);
                    memcpy(v + 4, p, 6);
                    put16(v + 10, 0x0abc); put16(v + 12, 0xf00d); put16(v + 14, 1024);
                } else if (type == 2 || type == 4) {
                    adv = 2;
                    put32(v, (E * 4 + 1) | 0x6d028000u);
                    memcpy(v + 4, p, 6);
                    put16(v + 10, type == 2 ? 8 : 0xbeef);
                    put16(v + 12, 0x0abc); put16(v + 14, 0xf00d); put16(v + 16, 1024); put16(v + 18, 0);
                    put32(v + 20, (E * 4 + 3) | 0x6e028000u);
                    v[24] = 0x81; v[25] = 0x7f; v[26] = 0x22; v[27] = 0;
                    v[28] = 127; v[29] = 0; v[30] = 64; v[31] = 3;
                } else if (type == 5) {
                    adv = 2;
                    put32(v, (E * 5 + 1) | 0x69028000u);
                    memcpy(v + 4, p, 6);
                    put16(v + 10, 0x0abc); put16(v + 12, 0xf00d); put16(v + 14, 1024);
                    put32(v + 16, (E * 5 + 3) | 0x6e038000u);
                    v[20] = 0x81; v[21] = 0x7f; v[22] = 0x22; v[23] = 0;
                    v[24] = 10; v[25] = 20; v[26] = 30; v[27] = 40;
                    v[28] = bone == 255 ? 0 : bone; v[29] = 0; v[30] = 3; v[31] = v[28];
                }
                if (adv) {
                    ret = 0;
                    put32(want + entries + ent * 28, E + 1);
                    put32(want + 0x1800 + 80, idx + adv);
                }
                if (Vertex__12VIPrimBufferR10VIVect3S16(o, p) != ret) return 1;
                if (memcmp(arena, want, sizeof want) != 0) return 2;
            }
        }
    }
    return 0;
}
