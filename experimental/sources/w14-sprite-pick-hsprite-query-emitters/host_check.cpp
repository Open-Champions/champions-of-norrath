// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "query.cpp"
#include <cstring>
#include <cstdlib>
#include <cstdint>
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }

static unsigned off(const void* p) { return word_of(p) - word_of(arena); }
static char trace[8192]; static int tl;
static void put(const char* tag, long a = 0, long b = 0, long c = 0) { tl += __builtin_snprintf(trace + tl, sizeof trace - tl, "%s:%ld,%ld,%ld;", tag, a, b, c); }
static unsigned sum(const unsigned char* m) { unsigned s = 17; for (int i = 0; i < 64; ++i) s = s * 31 + m[i]; return s; }
static unsigned char *gScene, *gQuery, *gMat, *gSelf;
static int lodValue, processRet, poolSize, results[8];
static unsigned lastProduct;
static int vf(int k, unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d) {
    put("vf", k, off(a), (b == gScene) + 2 * (sum(c) == lastProduct) + 4 * (d == gQuery));
    put("q12", *reinterpret_cast<int*>(gQuery + 12));
    return results[k];
}
#define VF(n) static int f##n(unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d) { return vf(n, a, b, c, d); }
VF(0) VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7)
extern "C" void* norrath_host_function(unsigned w) { static void* t[8] = {(void*)f0, (void*)f1, (void*)f2, (void*)f3, (void*)f4, (void*)f5, (void*)f6, (void*)f7}; return t[w - 1]; }
extern "C" int CalcLODLevelIndex__9VIHSpriteRC10VIMatrix44(unsigned char* s, unsigned char* m) { put("lod", s == gSelf, m == gMat); return lodValue; }
extern "C" int Process__9VIHSpriteiR7VIScene(unsigned char* s, int v, unsigned char* sc) { put("process", s == gSelf, v, sc == gScene); return processRet; }
extern "C" void CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(unsigned char* s, unsigned char* w, unsigned char* sc, unsigned char* out) {
    put("calc", s == gSelf, w == gMat, sc == gScene); put("calcin", sum(out));
    for (int i = 0; i < 64; ++i) out[i] = static_cast<unsigned char>(w[i] ^ 0x5a);
}
extern "C" int Begin__Ct6VIPool1Z19VIHSpriteAttachment(unsigned char* p) { put("begin", p == gSelf + 96); return 0; }
extern "C" int Next__Ct6VIPool1Z19VIHSpriteAttachmenti(unsigned char* p, int k) { put("next", p == gSelf + 96, k); return k + 1; }
extern "C" int End__Ct6VIPool1Z19VIHSpriteAttachment(unsigned char* p) { put("end", p == gSelf + 96); return poolSize; }
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(unsigned char* o, unsigned char* l, unsigned char* r) {
    put("mul", sum(o), off(l), sum(r));
    for (int i = 0; i < 64; ++i) o[i] = static_cast<unsigned char>(l[i] * 5 + r[63 - i] + 1);
    lastProduct = sum(o);
}
extern "C" unsigned Sprite__7VIScenei(unsigned char* s, int index) { put("sprite", s == gScene, index); return word_of(arena + 0x4000 + 0x100 * (index & 7)); }
extern "C" void _$_10VIMatrix44(unsigned char* m, int f) { put("dtor", f, sum(m)); }
int main() {
    unsigned char* self = arena + 0x100;
    gSelf = self;
    gScene = arena + 0x1000; gMat = arena + 0x1020; gQuery = arena + 0x1100;
    for (int pr = -1; pr <= 1; pr += 2) for (int n = 0; n <= 4; ++n) for (int negMask = 0; negMask < 16; negMask += 3) for (int skipMask = 0; skipMask < 3; ++skipMask) {
        for (unsigned i = 0; i < 0x8000; ++i) arena[i] = static_cast<unsigned char>(i * 7 + 3);
        processRet = pr; poolSize = n; lastProduct = 0; lodValue = 40 + n;
        *reinterpret_cast<unsigned*>(self + 104) = word_of(arena + 0x2000);   // records
        *reinterpret_cast<unsigned*>(self + 72) = word_of(arena + 0x3000);    // transforms
        for (int k = 0; k < 4; ++k) {
            unsigned char* rec = arena + 0x2000 + 16 * k;
            *reinterpret_cast<int*>(rec) = 40 + k;
            *reinterpret_cast<int*>(rec + 4) = ((skipMask >> (k & 1)) & 1) ? -1 : (k % 3);
            unsigned char* sprite = arena + 0x4000 + 0x100 * ((40 + k) & 7);
            *reinterpret_cast<unsigned*>(sprite + 60) = word_of(arena + 0x6000 + 0x80 * (((40 + k) & 7)));
            unsigned char* en = arena + 0x6000 + 0x80 * (((40 + k) & 7)) + 56;
            *reinterpret_cast<short*>(en) = static_cast<short>(4 * k - 4);
            *reinterpret_cast<unsigned*>(en + 4) = 1 + ((40 + k) & 7);
            *reinterpret_cast<unsigned*>(en - 8 + 4) = 1 + ((41 + k) & 7);
            *reinterpret_cast<unsigned*>(en + 8 + 4) = 1 + ((42 + k) & 7);
            results[(40 + k) & 7] = ((negMask >> k) & 1) ? -2 : (k & 1);
        }
        tl = 0; trace[0] = 0;
        int got = QueryEmitters__9VIHSpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery(self, gScene, gMat, gQuery);
        if (pr < 0) {
            if (got != -1) return 1;
            if (__builtin_strstr(trace, "begin")) return 2;
            int dt = 0; for (const char* p = trace; *p; ++p) if (!__builtin_strncmp(p, "dtor:2,", 7)) ++dt;
            char pn[64]; __builtin_snprintf(pn, sizeof pn, "lod:1,1,0;process:1,%d,1;", 40 + n);
            if (dt != 2 || !__builtin_strstr(trace, pn) || __builtin_strstr(trace, "calc:")) return 3;
            continue;
        }
        int want = 0, vfs = 0, muls = 0;
        for (int k = 0; k < n; ++k) {
            int t = *reinterpret_cast<int*>(arena + 0x2000 + 16 * k + 4);
            if (t == -1) continue;
            ++vfs; ++muls;
            if (results[(40 + k) & 7] < 0) { want = -1; break; }
        }
        if (got != want) return 4;
        int seenVf = 0, seenMul = 0, dt = 0;
        for (const char* p = trace; *p; ++p) {
            if (!__builtin_strncmp(p, "vf:", 3)) ++seenVf;
            if (!__builtin_strncmp(p, "mul:", 4)) ++seenMul;
            if (!__builtin_strncmp(p, "dtor:2,", 7)) ++dt;
        }
        if (seenVf != vfs || seenMul != muls || dt != 2) return 5;
        if (!__builtin_strstr(trace, "calc:1,1,1;")) return 6;
        for (int k = 0, at = 0; k < n; ++k) {
            int t = *reinterpret_cast<int*>(arena + 0x2000 + 16 * k + 4);
            if (t == -1) continue;
            char needle[64]; __builtin_snprintf(needle, sizeof needle, ",%d,", 0x3000 + 256 * t + 48);
            const char* p = __builtin_strstr(trace + at, needle);
            if (!p) return 7;
            at = static_cast<int>(p - trace) + 1;
            char qn[64]; __builtin_snprintf(qn, sizeof qn, "q12:%d,0,0;", 40 + k);
            if (!__builtin_strstr(p, qn)) return 8;
            if (results[(40 + k) & 7] < 0) break;
        }
        // every virtual call gets the scene, the fresh product and the query (mask 7)
        for (const char* p = trace; (p = __builtin_strstr(p, "vf:")) != nullptr; ++p) {
            const char* q = p; int commas = 0; while (*q && commas < 2) if (*q++ == ',') ++commas;
            if (std::atoi(q) != 7) return 9;
        }
    }
    return 0;
}
