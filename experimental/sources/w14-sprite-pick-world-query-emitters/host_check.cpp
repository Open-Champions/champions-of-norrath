// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "query.cpp"
#include <cstring>
#include <cstdint>
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }

static unsigned char *gW, *gVars;
static char trace[4096]; static int tl, zoneRet[8];
static int zoneOf(unsigned char* z) { return static_cast<int>((z - (arena + 0x6000)) / 592); }
extern "C" int QueryEmitters__6VIZoneR23VISceneEmitterQueryVars(unsigned char* z, unsigned char* v) {
    tl += __builtin_snprintf(trace + tl, sizeof trace - tl, "z%d%s;", zoneOf(z), v == gVars ? "" : "!");
    return zoneRet[zoneOf(z)];
}
// reference model of the documented walk, written over the same data
static unsigned char* planes() { return NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(gW + 56)); }
static int model(int node) {
    if (node & 0x40000000) {
        int zi = *reinterpret_cast<int*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(gW + 64)) + 4 * (node & 0x0fffffff));
        return QueryEmitters__6VIZoneR23VISceneEmitterQueryVars(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(gW + 76)) + 592 * zi, gVars);
    }
    node &= 0x0fffffff;
    const float* p = reinterpret_cast<const float*>(planes() + 24 * node);
    const float* v = reinterpret_cast<const float*>(gVars + 12);
    float dist = v[0] * p[0] + v[1] * p[1] + v[2] * p[2] + p[3];
    float r = *reinterpret_cast<float*>(gVars + 24);
    int F = *reinterpret_cast<const int*>(p + 4), K = *reinterpret_cast<const int*>(p + 5);
    if (r < dist) return model(F) >= 0 ? 0 : -1;
    if (dist < -r) return model(K) >= 0 ? 0 : -1;
    int a = dist > 0 ? F : K, b = dist > 0 ? K : F;
    if (model(a) < 0) return -1;
    return model(b) >= 0 ? 0 : -1;
}
int main() {
    gW = arena + 0x100; gVars = arena + 0x1000;
    // tree: node 0 splits on x (A=1, D=-1): front 1, back 2; node 1 on y: zones 0|1 (via zone words 0x40000000 + slot); node 2 on z: zone 2 | node 3 ; node 3 -> zones
    const float centres[][4] = {{3, 0, 0, 0.5f}, {-3, 0, 0, 0.5f}, {1, 0, 0, 0.5f}, {1, 5, 0, 3}, {1, -5, 0, 3}, {1, 0, 4, 6}, {0.5f, 0, 0, 0}, {1, 0, 0, 0}, {1, 2, 3, 100}};
    for (int c = 0; c < 9; ++c) for (int zr = 0; zr < 8; ++zr) for (int hi = 0; hi < 2; ++hi) {
        for (unsigned i = 0; i < 0x8000; ++i) arena[i] = static_cast<unsigned char>(i * 7 + 3);
        *reinterpret_cast<unsigned*>(gW + 56) = word_of(arena + 0x2000);   // planes
        *reinterpret_cast<unsigned*>(gW + 64) = word_of(arena + 0x4000);   // zone index words
        *reinterpret_cast<unsigned*>(gW + 76) = word_of(arena + 0x6000);   // zones
        for (int s = 0; s < 8; ++s) *reinterpret_cast<int*>(arena + 0x4000 + 4 * s) = (s * 3) % 8 ? (s * 3) % 8 : 1;
        float* n0 = reinterpret_cast<float*>(arena + 0x2000);
        n0[0] = 1; n0[1] = 0; n0[2] = 0; n0[3] = -1; *reinterpret_cast<int*>(n0 + 4) = 1; *reinterpret_cast<int*>(n0 + 5) = 2;
        float* n1 = n0 + 6;
        n1[0] = 0; n1[1] = 1; n1[2] = 0; n1[3] = 0; *reinterpret_cast<int*>(n1 + 4) = 0x40000000 | 1; *reinterpret_cast<int*>(n1 + 5) = 0x40000000 | 2;
        float* n2 = n0 + 12;
        n2[0] = 0; n2[1] = 0; n2[2] = 1; n2[3] = -2; *reinterpret_cast<int*>(n2 + 4) = (hi ? 0x10000000 : 0) | 3; *reinterpret_cast<int*>(n2 + 5) = 0x40000000 | 3;
        float* n3 = n0 + 18;
        n3[0] = 1; n3[1] = 1; n3[2] = 0; n3[3] = 0; *reinterpret_cast<int*>(n3 + 4) = 0x40000000 | 4; *reinterpret_cast<int*>(n3 + 5) = 0x40000000 | 5;
        float* v = reinterpret_cast<float*>(gVars + 12);
        v[0] = centres[c][0]; v[1] = centres[c][1]; v[2] = centres[c][2]; v[3] = centres[c][3];
        for (int z = 0; z < 8; ++z) zoneRet[z] = ((zr >> (z % 3)) & 1) ? -2 : z;
        tl = 0; trace[0] = 0;
        int got = QueryEmitters__7VIWorldiR23VISceneEmitterQueryVars(gW, 0, gVars);
        char got_trace[4096]; std::memcpy(got_trace, trace, sizeof trace); int got_len = tl;
        tl = 0; trace[0] = 0;
        int want = model(0) >= 0 ? 0 : -1;
        if (got != want) return 1;
        if (got_len != tl || std::memcmp(got_trace, trace, tl) != 0) return 2;
        if (__builtin_strchr(got_trace, '!')) return 3;
    }
    // a zone node is passed straight through and its result is returned unchanged
    for (int r = -3; r <= 6; r += 3) {
        zoneRet[3] = r; tl = 0; trace[0] = 0;
        *reinterpret_cast<unsigned*>(gW + 56) = word_of(arena + 0x2000); *reinterpret_cast<unsigned*>(gW + 64) = word_of(arena + 0x4000); *reinterpret_cast<unsigned*>(gW + 76) = word_of(arena + 0x6000);
        *reinterpret_cast<int*>(arena + 0x4000 + 4 * 5) = 3;
        int got = QueryEmitters__7VIWorldiR23VISceneEmitterQueryVars(gW, static_cast<int>(0x40000000u | 5u), gVars);
        if (got != r) return 4;
        if (__builtin_strcmp(trace, "z3;") != 0) return 5;
        got = QueryEmitters__7VIWorldiR23VISceneEmitterQueryVars(gW, static_cast<int>(0x50000000u | 5u), gVars);   // other top-nibble bits with 0x4 still select the zone path
        if (got != r) return 6;
    }
    return 0;
}
