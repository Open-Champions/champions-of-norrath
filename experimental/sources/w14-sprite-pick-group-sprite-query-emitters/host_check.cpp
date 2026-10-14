// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "query.cpp"
#include <cstring>
#include <cstdint>
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }

static char trace[4096]; static int tl;
static void put(const char* tag, long v) { tl += __builtin_snprintf(trace + tl, sizeof trace - tl, "%s:%ld;", tag, v); }
static unsigned char *gScene, *gQuery;
static unsigned lastProductSum;
static int results[4];
static unsigned sum(const unsigned char* m) { unsigned s = 17; for (int i = 0; i < 64; ++i) s = s * 31 + m[i]; return s; }
static int vf(int k, unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d) {
    put("vf", k); put("adj", a - (arena + 0x4000 + 0x100 * k));
    if (b != gScene || d != gQuery) put("badarg", 1);
    put("prod", sum(c) == lastProductSum);
    put("q12", *reinterpret_cast<int*>(gQuery + 12));
    return results[k];
}
static int f0(unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d) { return vf(0,a,b,c,d); }
static int f1(unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d) { return vf(1,a,b,c,d); }
static int f2(unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d) { return vf(2,a,b,c,d); }
extern "C" void* norrath_host_function(unsigned w) { return w == 1 ? (void*)f0 : w == 2 ? (void*)f1 : w == 3 ? (void*)f2 : nullptr; }
static unsigned char* gMat;
extern "C" void Transform__10VIMatrix44RC7VIVect3fT1(unsigned char* m, unsigned char* a, float f, unsigned char* b) {
    put("xf", word_of(a) - word_of(arena)); put("xf2", word_of(b) - word_of(arena));
    unsigned bits; std::memcpy(&bits, &f, 4); put("xff", bits);
    for (int i = 0; i < 64; ++i) m[i] = static_cast<unsigned char>(a[i % 12] + 3 * b[i % 12] + (bits >> (i & 24)) + i);
}
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(unsigned char* o, unsigned char* l, unsigned char* r) {
    put("mulr", r == gMat);
    for (int i = 0; i < 64; ++i) o[i] = static_cast<unsigned char>(l[i] * 5 + r[63 - i] + 1);
    lastProductSum = sum(o);
}
static int sprIdx;
extern "C" unsigned Sprite__7VIScenei(unsigned char* s, int index) { put("sprite", index); put("scene", s == gScene); return word_of(arena + 0x4000 + 0x100 * (sprIdx++)); }
extern "C" void _$_10VIMatrix44(unsigned char* m, int f) { put("dtor", f); put("dsum", sum(m)); }
int main() {
    unsigned char* self = arena + 0x100;
    gScene = arena + 0x1000; gMat = arena + 0x1020; gQuery = arena + 0x1100;
    const int resultSets[][3] = {{0, 0, 0}, {5, 0, 0}, {0, -3, 0}, {0, 0, -1}, {9, 9, -2}};
    for (int count = -1; count <= 3; ++count) for (int rs = 0; rs < 5; ++rs) {
        for (unsigned i = 0; i < 0x8000; ++i) arena[i] = static_cast<unsigned char>(i * 7 + 3);
        *reinterpret_cast<int*>(self + 64) = count;
        for (int i = 0; i < 3; ++i) {
            unsigned char* e = self + 68 + 32 * i;
            *reinterpret_cast<int*>(e) = 100 + i;
            *reinterpret_cast<float*>(e + 16) = 1.5f * (i + 1);
            unsigned char* sprite = arena + 0x4000 + 0x100 * i;
            *reinterpret_cast<unsigned*>(sprite + 60) = word_of(arena + 0x6000 + 0x100 * i);
            unsigned char* en = arena + 0x6000 + 0x100 * i + 56;
            *reinterpret_cast<short*>(en) = static_cast<short>(8 * i - 8);
            *reinterpret_cast<unsigned*>(en + 4) = i + 1;
            *reinterpret_cast<unsigned*>(en - 8 + 4) = (i + 1) % 3 + 1;
            *reinterpret_cast<unsigned*>(en + 8 + 4) = (i + 2) % 3 + 1;
            results[i] = resultSets[rs][i];
        }
        tl = 0; sprIdx = 0; trace[0] = 0; lastProductSum = 0;
        int got = QueryEmitters__13VIGroupSpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery(self, gScene, gMat, gQuery);
        int want = 0, visited = 0;
        for (int i = 0; i < count && i < 3; ++i) { ++visited; if (resultSets[rs][i] < 0) { want = -1; break; } }
        if (got != want) return 1;
        int vfs = 0, dtors = 0;
        for (const char* p = trace; *p; ++p) { if (!__builtin_strncmp(p, "vf:", 3)) ++vfs; if (!__builtin_strncmp(p, "dtor:2", 6)) ++dtors; }
        if (vfs != visited || dtors != 2) return 2;
        if (__builtin_strstr(trace, "badarg") || __builtin_strstr(trace, "prod:0") || __builtin_strstr(trace, "mulr:0")) return 3;
        // the query word at +12 holds the entry word of the entry being asked
        for (int i = 0; i < visited; ++i) {
            char needle[64]; __builtin_snprintf(needle, sizeof needle, "q12:%d;", 100 + i);
            if (!__builtin_strstr(trace, needle)) return 4;
        }
    }
    return 0;
}
