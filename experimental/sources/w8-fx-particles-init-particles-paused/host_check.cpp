// Host contract for _GLOBAL_$I$particlesPaused. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" {
unsigned char globalSystem[256];
unsigned char PS[16 * 312];
unsigned char invisible[80];
extern unsigned char globalSystemSave[256];
extern unsigned char particletex[16];
extern unsigned char trailTexture[16];
extern unsigned char blanktex[16];
void _GLOBAL_$I$particlesPaused();
}
static int failures = 0;
static void expect(bool ok, const char* what) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; }
}
static unsigned word(const unsigned char* b, unsigned o) { unsigned v; std::memcpy(&v, b + o, 4); return v; }
static int order = 0, rmap_at[3], def_at = 0;
static int rmap_calls = 0;
static void* rmap_objects[3];
static char rmap_group[3][16], rmap_name[3][16];
extern "C" void __11ResourceMapPcT1(void* map, const char* group, const char* name) {
    if (rmap_calls < 3) {
        rmap_objects[rmap_calls] = map;
        std::snprintf(rmap_group[rmap_calls], 16, "%s", group);
        std::snprintf(rmap_name[rmap_calls], 16, "%s", name);
        rmap_at[rmap_calls] = ++order;
    }
    ++rmap_calls;
}
static int def_calls = 0;
static int def_ints[11];
static float def_floats[3];
static void* def_self = nullptr;
extern "C" void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
                                                     float x, float y, float z, int i, int j, int k, void (*cb)(void*)) {
    ++def_calls; def_at = ++order; def_self = self;
    const int v[11] = {a, b, c, d, e, f, g, h, i, j, k};
    std::memcpy(def_ints, v, sizeof v);
    def_floats[0] = x; def_floats[1] = y; def_floats[2] = z;
    if (cb) std::fprintf(stderr, "fail: callback\n");
}
static void check_entries(const unsigned char* base, const char* what) {
    for (unsigned e = 0; e < 16; ++e) {
        expect(word(base, e * 16) == 0u && word(base, e * 16 + 4) == 0u && word(base, e * 16 + 8) == 1u &&
               word(base, e * 16 + 12) == 257u, what);
    }
}

int main() {
    std::memset(globalSystem, 0xa5, sizeof globalSystem);
    std::memset(globalSystemSave, 0xa5, sizeof globalSystemSave);
    std::memset(PS, 0xa5, sizeof PS);
    _GLOBAL_$I$particlesPaused();
    check_entries(globalSystem, "globalSystem entries");
    check_entries(globalSystemSave, "globalSystemSave entries");
    for (unsigned r = 0; r < 16; ++r) {
        check_entries(PS + r * 312, "PS record entries");
        expect(PS[r * 312 + 256] == 0xa5 && PS[r * 312 + 311] == 0xa5, "bytes past the first 256 of a record untouched");
    }
    expect(rmap_calls == 3 && def_calls == 1, "three resource maps and one definition");
    expect(rmap_objects[0] == particletex && rmap_objects[1] == trailTexture && rmap_objects[2] == blanktex, "resource map objects");
    expect(std::strcmp(rmap_name[0], "particletex.tex") == 0 && std::strcmp(rmap_name[1], "trail.tex") == 0 &&
           std::strcmp(rmap_name[2], "blank.tex") == 0, "resource map names");
    for (int n = 0; n < 3; ++n) expect(std::strcmp(rmap_group[n], "engine.lmp") == 0, "resource map group");
    expect(def_self == invisible && def_floats[0] == 0.0f && def_floats[1] == 0.0f && def_floats[2] == 0.0f, "invisible definition");
    for (int n = 0; n < 11; ++n) expect(def_ints[n] == (n == 8 ? 2 : 0), "definition integers: only the first trailing one is 2");
    expect(rmap_at[0] < rmap_at[1] && rmap_at[1] < rmap_at[2] && rmap_at[2] < def_at, "maps before the definition");
    if (failures) return 1;
    std::printf("particles-paused contract passed\n");
    return 0;
}
