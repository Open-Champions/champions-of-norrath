// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "pick.cpp"
#include <cstring>
#include <cstdint>
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }

static char trace[4096]; static int tl;
static void put(const char* tag, long v) { tl += __builtin_snprintf(trace + tl, sizeof trace - tl, "%s:%ld;", tag, v); }
static unsigned char *theView, *gSeg, *gFilter, *gScene, *gRaster;
static int results[4];
static int vf(int k, unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d, unsigned char* e) {
    put("vf", k); put("adj", a - (arena + 0x4000 + 0x100 * k));
    if (b != gSeg || c != gFilter || d != gScene || e != gRaster) put("badarg", 1);
    return results[k];
}
static int f0(unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d, unsigned char* e) { return vf(0,a,b,c,d,e); }
static int f1(unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d, unsigned char* e) { return vf(1,a,b,c,d,e); }
static int f2(unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d, unsigned char* e) { return vf(2,a,b,c,d,e); }
extern "C" void* norrath_host_function(unsigned w) { return w == 1 ? (void*)f0 : w == 2 ? (void*)f1 : w == 3 ? (void*)f2 : nullptr; }
static unsigned sum(const unsigned char* m) { unsigned s = 17; for (int i = 0; i < 64; ++i) s = s * 31 + m[i]; return s; }
extern "C" unsigned char* GetModelView__C8VIRaster(unsigned char* r) { put("getview", r == gRaster); return theView; }
extern "C" void sceVu0CopyMatrix(unsigned char* to, unsigned char* from) { put("copy", sum(from)); std::memcpy(to, from, 64); }
extern "C" void Transform__10VIMatrix44RC7VIVect3fT1(unsigned char* m, unsigned char* a, float f, unsigned char* b) {
    put("xf", word_of(a) - word_of(arena)); put("xf2", word_of(b) - word_of(arena)); put("xfm", sum(m));
    unsigned bits; std::memcpy(&bits, &f, 4); put("xff", bits);
    for (int i = 0; i < 64; ++i) m[i] = static_cast<unsigned char>(a[i % 12] + 3 * b[i % 12] + (bits >> (i & 24)) + i);
}
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(unsigned char* o, unsigned char* l, unsigned char* r) {
    put("mulo", sum(o)); put("mull", sum(l)); put("mulr", sum(r));
    for (int i = 0; i < 64; ++i) o[i] = static_cast<unsigned char>(l[i] * 5 + r[63 - i] + 1);
}
extern "C" void SetModelView__8VIRasterRC10VIMatrix44(unsigned char* r, unsigned char* m) { put("setview", r == gRaster); put("setm", sum(m)); }
static int sprIdx;
extern "C" unsigned Sprite__7VIScenei(unsigned char* s, int index) { put("sprite", index); put("scene", s == gScene); return word_of(arena + 0x4000 + 0x100 * (sprIdx++)); }
extern "C" void _$_10VIMatrix44(unsigned char* m, int f) { put("dtor", f); put("dsum", sum(m)); }
int main() {
    unsigned char* self = arena + 0x100;
    theView = arena + 0x800;
    gSeg = arena + 0x1000; gFilter = arena + 0x1010; gScene = arena + 0x1020; gRaster = arena + 0x1030;
    // Each sprite has its own table at 0x6000 + 0x40*i; the entry at the checked slot names function i+1.
    for (int count = -1; count <= 3; ++count) for (int hitAt = -1; hitAt < 3; ++hitAt) {
        for (unsigned i = 0; i < 0x8000; ++i) arena[i] = static_cast<unsigned char>(i * 7 + 3);
        *reinterpret_cast<int*>(self + 64) = count;
        for (int i = 0; i < 3; ++i) {
            unsigned char* e = self + 68 + 32 * i;
            *reinterpret_cast<int*>(e) = 100 + i;
            *reinterpret_cast<float*>(e + 16) = 1.5f * (i + 1);
            unsigned char* sprite = arena + 0x4000 + 0x100 * i;
            *reinterpret_cast<unsigned*>(sprite + 60) = word_of(arena + 0x6000 + 0x40 * i);
            unsigned char* en = arena + 0x6000 + 0x40 * i + 48;
            *reinterpret_cast<short*>(en) = static_cast<short>(8 * i - 8);
            *reinterpret_cast<unsigned*>(en + 4) = i + 1;
            // other slots call other functions so a wrong slot is seen
            *reinterpret_cast<unsigned*>(en - 8 + 4) = (i + 1) % 3 + 1;
            *reinterpret_cast<unsigned*>(en + 8 + 4) = (i + 2) % 3 + 1;
            results[i] = (i == hitAt) ? 7 : 0;
        }
        tl = 0; sprIdx = 0; trace[0] = 0;
        int got = Pick__13VIGroupSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster(self, gSeg, gFilter, gScene, gRaster);
        // reference model of the documented behaviour
        int want = 0, visited = 0;
        for (int i = 0; i < count && i < 3; ++i) { ++visited; if (i == hitAt) { want = 1; break; } }
        if (got != want) return 1;
        int vfs = 0, dtors = 0, sets = 0;
        for (const char* p = trace; *p; ++p) { if (!__builtin_strncmp(p, "vf:", 3)) ++vfs; if (!__builtin_strncmp(p, "dtor:2", 6)) ++dtors; if (!__builtin_strncmp(p, "setview:1", 9)) ++sets; }
        if (vfs != visited || dtors != 3 || sets != visited) return 2;
        if (__builtin_strstr(trace, "badarg") || __builtin_strstr(trace, "setview:0")) return 3;
        if (__builtin_strncmp(trace, "getview:1;copy:", 15) != 0) return 4;
        if (count > 0) {
            // the transform of the first entry gets the entry words at +72 and +88, and the product sees them in order
            char want0[64]; __builtin_snprintf(want0, sizeof want0, "xf:%ld;xf2:%ld;", long(0x100 + 72), long(0x100 + 88));
            if (!__builtin_strstr(trace, want0)) return 5;
        }
    }
    return 0;
}
