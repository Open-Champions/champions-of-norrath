// Host contract for the ItemProp constructor; not an EE input. Words are offsets from a host base.
#define W10_ACTOR_H_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10h_host_base;
void* w10h_host_fn(unsigned) { return nullptr; }
extern "C" unsigned char globalItem[16 * 8] __asm__("_4item$globalItem");
extern "C" unsigned char vt_ItemProp[8] __asm__("_vt$8ItemProp");
unsigned char globalItem[16 * 8];
unsigned char vt_ItemProp[8];
#include "ctor.cpp"

static unsigned char arena[0x2000] __attribute__((aligned(16)));
static std::string calls; static int ids[2], idIndex; static void* proxyArg[2];
static unsigned char propPoint[12]; static char *pLump, *pA, *pB; static long pNumber; static int pFlag; static void* pSelf;
static unsigned char matPoint[12]; static void* matSelf; static short matYaw; static unsigned flagsAtGrid;
extern "C" {
int getProxyMeshId__4itemPQ24item11description(void* d) { calls += "M"; proxyArg[idIndex % 2] = d; return ids[idIndex++ % 2]; }
void prop_construct(void* s, const void* p, char* l, char* a, char* b, long n, int f) { calls += "P"; pSelf = s; std::memcpy(propPoint, p, 12); pLump = l; pA = a; pB = b; pNumber = n; pFlag = f;
    std::memset(static_cast<unsigned char*>(s) + 328, 0x5a, 80); }
void objectUpdateInGrid__FP10GameObject(void* o) { calls += "G"; std::memcpy(&flagsAtGrid, static_cast<unsigned char*>(o) + 12, 4); }
void matMakeTransYaw__FP8Matrix34G6Point3s(void* m, const void* p, short y) { calls += "X"; matSelf = m; std::memcpy(matPoint, p, 12); matYaw = y; }
}
int main() {
    int bad = 0;
    w10h_host_base = arena;
    for (int variant = 0; variant < 4; ++variant) {
        unsigned char* object = arena + 0x400;
        unsigned char* desc = arena + 0x1000;
        std::memset(arena, 0x11, sizeof arena); calls.clear(); idIndex = 0;
        for (int i = 0; i < 8; ++i) { unsigned w[2] = {0x100u + 16 * i, 0x200u + 16 * i}; std::memcpy(globalItem + 16 * i, w, 8); }
        ids[0] = 2; ids[1] = 5;
        for (int i = 0; i < 80; ++i) desc[i] = static_cast<unsigned char>(0xa0 + i);
        desc[8 + 4] = (variant & 1) ? 0x80 : 0x00;           // bit 39 of the 64-bit value at object+336 is bit 7 of byte +340
        unsigned pre = (variant & 2) ? 0xffffffffu : 0u; std::memcpy(object + 12, &pre, 4);
        unsigned short yawField = static_cast<unsigned short>(variant == 3 ? 0xc000 : 0x1234); std::memcpy(object + 56, &yawField, 2);
        unsigned char position[12]; for (int i = 0; i < 12; ++i) position[i] = static_cast<unsigned char>(0x70 + i);
        for (int i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(0x90 + i);
        itemprop_construct(object, position, desc, 7 + variant);
        if (calls != "MMPGX") { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; }
        if (proxyArg[0] != desc || proxyArg[1] != desc || pSelf != object) { std::puts("fail: proxy or self"); bad = 1; }
        if (std::memcmp(propPoint, position, 12) != 0 || std::strcmp(pLump, "equipmnt.lmp") != 0) { std::puts("fail: prop position or text"); bad = 1; }
        if (pA != reinterpret_cast<char*>(arena + 0x120) || pB != reinterpret_cast<char*>(arena + 0x250)) { std::puts("fail: entry words (entry 2 word 0, entry 5 word 1)"); bad = 1; }
        if (pNumber != -1 || pFlag != 7 + variant) { std::puts("fail: number or flag"); bad = 1; }
        unsigned w; std::memcpy(&w, object + 72, 4);
        if (w != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_ItemProp))) { std::puts("fail: table pointer"); bad = 1; }
        // the stores of -1 and 0 into 360..388 are overwritten by the 72-byte copy, which covers 328..399
        if (std::memcmp(object + 328, desc, 72) != 0) { std::puts("fail: description copy"); bad = 1; }
        if (object[400] != 0x5a) { std::puts("fail: copy ran past 72 bytes"); bad = 1; }
        unsigned f; std::memcpy(&f, object + 12, 4);
        unsigned want = ((pre | 0x102000u) & 0xffffe7ffu);
        if (variant & 1) want &= 0xffffcfffu;
        if (f != want || flagsAtGrid != want) { std::printf("fail: flags %08x want %08x\n", f, want); bad = 1; }
        if (matSelf != object + 272 || std::memcmp(matPoint, object + 16, 12) != 0 || matYaw != static_cast<short>(yawField + 16384)) { std::puts("fail: matrix call"); bad = 1; }
    }
    if (!bad) std::puts("itemprop ctor ok");
    return bad;
}
