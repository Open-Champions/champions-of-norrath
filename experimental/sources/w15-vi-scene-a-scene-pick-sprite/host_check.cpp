// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "pick.cpp"
#include <cstdint>
#include <cstring>
alignas(16) static unsigned char arena[0x8000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }
static unsigned W(unsigned off) { return word_of(arena + off); }
static void setu(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned getu(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
static void seth(unsigned off, short v) { std::memcpy(arena + off, &v, 2); }
static int calls, which; static unsigned char *gA0, *gA1, *gA2, *gA3, *gA4; static int results[3];
static int slotFn(int k, unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d, unsigned char* e) {
    ++calls; which = k; gA0 = a; gA1 = b; gA2 = c; gA3 = d; gA4 = e; return results[k];
}
static int f0(unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d, unsigned char* e) { return slotFn(0, a, b, c, d, e); }
static int f1(unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d, unsigned char* e) { return slotFn(1, a, b, c, d, e); }
static int f2(unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d, unsigned char* e) { return slotFn(2, a, b, c, d, e); }
extern "C" void* norrath_host_function(unsigned w) { return w == 1 ? (void*)f0 : w == 2 ? (void*)f1 : w == 3 ? (void*)f2 : nullptr; }
int main() {
    const int adj[3] = {0, 24, -8};
    const int rv[] = {0, 1, -7, 5};
    for (int index = 0; index < 3; ++index) {
        for (int r : rv) {
            std::memset(arena, 0, sizeof arena);
            unsigned scene = 0x100, filter = 0x700, segment = 0x600;
            setu(scene + 1004, W(0x1000));
            setu(scene + 19356, W(0x5000));
            for (int k = 0; k < 3; ++k) {
                setu(0x1000 + 12 * k, W(0x2000 + 0x100 * k));
                setu(0x2000 + 0x100 * k + 60, W(0x3000 + 0x40 * k));
                seth(0x3000 + 0x40 * k + 48, static_cast<short>(adj[k]));
                setu(0x3000 + 0x40 * k + 52, k + 1);
            }
            results[index] = r; calls = 0;
            for (int i = 0; i < 16; ++i) arena[filter + i] = static_cast<unsigned char>(0xc0 + i);
            unsigned char before[16]; std::memcpy(before, arena + filter, 16);
            int got = PickSprite__7VISceneR13VIPickSegmentR13VISceneFilteri(arena + scene, arena + segment, arena + filter, index);
            if (got != r) return 1;
            if (calls != 1 || which != index) return 2;
            if (gA0 != arena + 0x2000 + 0x100 * index + adj[index] || gA1 != arena + segment || gA2 != arena + filter || gA3 != arena + scene) return 3;
            if (gA4 != arena + 0x5000) return 4;
            for (int i = 0; i < 16; ++i) {
                unsigned char expect = before[i];
                if (r != 0) {
                    if (i >= 4 && i < 8) expect = 0xff;
                    if (i >= 12 && i < 16) expect = 0xff;
                    if (i >= 8 && i < 12) expect = static_cast<unsigned char>(index >> (8 * (i - 8)));
                }
                if (arena[filter + i] != expect) return 5;
            }
        }
    }
    return 0;
}
