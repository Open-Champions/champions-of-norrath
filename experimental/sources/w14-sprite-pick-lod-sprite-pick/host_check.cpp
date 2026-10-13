// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "pick.cpp"
#include <cstdint>
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }

#include <cstring>
static unsigned char *ga[5]; static int calls, which, ret;
static int hit(int k, unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d, unsigned char* e) { ga[0]=a; ga[1]=b; ga[2]=c; ga[3]=d; ga[4]=e; ++calls; which = k; return ret + k; }
template <int K> static int fn(unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d, unsigned char* e) { return hit(K, a, b, c, d, e); }
extern "C" void* norrath_host_function(unsigned w) {
    switch (w) {
    case 1: return reinterpret_cast<void*>(fn<0>); case 2: return reinterpret_cast<void*>(fn<1>);
    case 3: return reinterpret_cast<void*>(fn<2>); case 4: return reinterpret_cast<void*>(fn<3>);
    default: return nullptr;
    }
}
static unsigned char* viewMatrix; static int viewCalls, sceneIndex, sceneCalls;
extern "C" unsigned char* GetModelView__C8VIRaster(unsigned char*) { ++viewCalls; return viewMatrix; }
static unsigned spriteWords[4];
extern "C" unsigned Sprite__7VIScenei(unsigned char*, int index) { sceneIndex = index; ++sceneCalls; return spriteWords[index - 100]; }
int main() {
    unsigned char* self = arena + 0x100; unsigned char* seg = arena + 0x800; unsigned char* filt = arena + 0x900; unsigned char* scene = arena + 0xa00;
    unsigned char* raster = arena + 0xb00; unsigned char* sprites[4]; unsigned char* tabs[4];
    viewMatrix = arena + 0xc00;
    for (int i = 0; i < 4; ++i) { sprites[i] = arena + 0x1000 + 0x100 * i; tabs[i] = arena + 0x2000 + 0x100 * i; spriteWords[i] = word_of(sprites[i]); }
    const float sets[][4] = {{1, 2, 3, 4}, {3, 4, 5, 6}, {0.5f, 1, 2, 2.9f}, {-4, -5, -6, 1}, {4, 1, 3, 3}};
    for (int count = -1; count <= 4; ++count) for (int s = 0; s < 5; ++s) for (int adj = -16; adj <= 16; adj += 16) {
        for (unsigned i = 0; i < 0x4000; ++i) arena[i] = static_cast<unsigned char>(i * 7 + 3);
        float* v = reinterpret_cast<float*>(viewMatrix); v[12] = 1.5f; v[13] = 3.5f; v[14] = 1.0f;
        float* o = reinterpret_cast<float*>(seg); o[0] = 0.5f; o[1] = 1.5f; o[2] = -1.0f;   // squared distance 9
        *reinterpret_cast<int*>(self + 68) = count;
        for (int i = 0; i < 4; ++i) {
            *reinterpret_cast<float*>(self + 72 + 8 * i) = sets[s][i];
            *reinterpret_cast<int*>(self + 76 + 8 * i) = 100 + i;
            *reinterpret_cast<unsigned*>(sprites[i] + 60) = word_of(tabs[i]);
            for (int slot = 0; slot < 8; ++slot) *reinterpret_cast<unsigned*>(tabs[i] + 8 * slot + 4) = 1 + (slot + i) % 4;
            *reinterpret_cast<short*>(tabs[i] + 48) = static_cast<short>(adj);
        }
        int expect = 0;
        while (expect < count && sets[s][expect] * sets[s][expect] < 9.0f) ++expect;
        calls = 0; viewCalls = 0; sceneCalls = 0; ret = 1000 + count;
        int got = Pick__11VILODSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster(self, seg, filt, scene, raster);
        if (viewCalls != 1) return 1;
        if (expect == count) { if (got != 0 || calls != 0 || sceneCalls != 0) return 2; continue; }
        if (sceneCalls != 1 || sceneIndex != 100 + expect) return 3;
        if (calls != 1 || which != (6 + expect) % 4 || got != ret + which) return 4;
        if (ga[0] != sprites[expect] + adj || ga[1] != seg || ga[2] != filt || ga[3] != scene || ga[4] != raster) return 5;
    }
    return 0;
}
