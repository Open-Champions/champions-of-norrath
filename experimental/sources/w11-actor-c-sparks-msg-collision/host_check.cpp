// Host contract for msg_collision__6SparksP10GameObject; not an EE input. Pointer words are offsets from a
// host base; every callee is a recording stub and the table slot is routed through w11_actor_c_host_slots.
#define W11_ACTOR_C_HOST 1
#include "msg_collision.cpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w11_actor_c_host_base;
static std::string calls;
static void* slot_target; static int slot_result, slot_alloc_fail;
static void* slot0(void* adjusted) { calls += "slot(" + std::to_string(static_cast<unsigned char*>(adjusted) - w11_actor_c_host_base) + ");"; return slot_result ? adjusted : nullptr; }
W11ActorCHostSlot w11_actor_c_host_slots[] = {slot0};
static unsigned char blockMem[256];
static float lf[12]; static Point3 lpos; static void* lobj; static int lflags; static void* lblock;
static unsigned char desc[80]; static void *ra, *rt; static float rfa, rfb; static int ri[4]; static bool rb;
extern "C" {
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return slot_alloc_fail ? nullptr : blockMem; }
void lightEffect_construct(void* self, const Point3* position, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7,
                           float f8, float f9, float f10, float f11, void* object, int flags) {
    calls += "light;"; lblock = self; lpos = *position; const float v[12] = {f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11};
    std::memcpy(lf, v, sizeof v); lobj = object; lflags = flags;
}
void* lumpFindResource__FPcT0(const char* lump, const char* name) { calls += std::string("lump(") + lump + "," + name + ");"; return blockMem + 100; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* a, void* o, float g) {
    calls += "sound(" + std::to_string(static_cast<unsigned char*>(a) - blockMem) + "," + std::to_string(static_cast<unsigned char*>(o) - w11_actor_c_host_base) + "," + std::to_string(g) + ");";
}
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b, int c, int d, int e, int f,
                                                                    void* description, bool g) {
    calls += "attack;"; ra = attacker; rt = target; rfa = a; rfb = b; ri[0] = c; ri[1] = d; ri[2] = e; ri[3] = f; rb = g;
    std::memcpy(desc, description, sizeof desc);
}
}
int main() {
    static unsigned char base[8192];
    w11_actor_c_host_base = base;
    int failures = 0;
    for (int variant = 0; variant < 8; ++variant) {
        std::memset(base, 0, sizeof base);
        unsigned char* self = base + 64;
        unsigned char* holder = base + 1024;
        unsigned char* target = base + 2048;
        unsigned char* table = base + 3072;
        const unsigned w84 = variant == 0 ? 0 : 7;
        std::memcpy(self + 84, &w84, 4);
        const unsigned w76 = variant == 1 ? 0 : 1024;
        std::memcpy(self + 76, &w76, 4);
        const unsigned t = variant == 2 ? 0 : 2048;
        std::memcpy(holder + 12, &t, 4);
        const unsigned tbl = 3072; std::memcpy(target + 72, &tbl, 4);
        const short adjustment = 16; std::memcpy(table + 160, &adjustment, 2);
        const unsigned fn = 0; std::memcpy(table + 164, &fn, 4);
        slot_result = variant == 3 ? 0 : 1;
        slot_alloc_fail = variant == 4;
        const float pos[3] = {1.0f, 2.0f, 3.0f}; std::memcpy(self + 16, pos, 12);
        const unsigned w168 = 0x12348765u, w172 = 0xabcdfedcu; std::memcpy(self + 168, &w168, 4); std::memcpy(self + 172, &w172, 4);
        calls.clear(); std::memset(desc, 0x55, sizeof desc);
        unsigned char* other = base + 5000;
        msg_collision__6SparksP10GameObject(self, other);
        unsigned after84; std::memcpy(&after84, self + 84, 4);
        const bool runs = variant != 0 && variant != 1 && variant != 2 && variant != 3;
        if (!runs) {
            const std::string want = variant == 3 ? "slot(2064);" : "";
            if (calls != want || after84 != w84) { std::fprintf(stderr, "variant %d: calls '%s'\n", variant, calls.c_str()); ++failures; }
            continue;
        }
        std::string want = "slot(2064);alloc188;";
        if (variant != 4) want += "light;";
        want += "lump(fx.lmp,spel_ew_shockfire_hit.anm);sound(100,64,1.000000);attack;";
        if (calls != want || after84 != 0) { std::fprintf(stderr, "variant %d: calls '%s'\n", variant, calls.c_str()); ++failures; }
        if (variant != 4) {
            const float wantf[12] = {255, 200, 100, -32, 0, 0, 0, 0, 180, 120, 1, 1};
            if (std::memcmp(lf, wantf, sizeof wantf) || std::memcmp(&lpos, pos, 12) || lobj || lflags || lblock != blockMem) { std::fprintf(stderr, "light args\n"); ++failures; }
        }
        unsigned char wantDesc[80]; std::memset(wantDesc, 0x55, sizeof wantDesc);   // only the initialised parts are compared
        auto setHalf = [&](int off, short v) { std::memcpy(wantDesc + off, &v, 2); };
        for (int i = 0; i < 8; ++i) setHalf(16 + 2 * i, 0);
        wantDesc[32] = wantDesc[33] = 0xff;
        for (int e = 0; e < 4; ++e) setHalf(36 + 8 * e, -1);
        setHalf(36, 31); setHalf(38, 100); setHalf(40, static_cast<short>(w172)); setHalf(42, static_cast<short>(w168));
        // compared: halfwords 16..31, bytes 32..33, the first halfword of each entry and the four fields of entry 0
        bool descOk = std::memcmp(desc + 16, wantDesc + 16, 18) == 0 && std::memcmp(desc + 36, wantDesc + 36, 8) == 0;
        for (int e = 1; e < 4; ++e) descOk = descOk && std::memcmp(desc + 36 + 8 * e, wantDesc + 36 + 8 * e, 2) == 0;
        if (!descOk || ra != other || rt != target || rfa != 0.0f || rfb != 0.0f || ri[0] != 10000000 || ri[1] != static_cast<int>(w172) ||
            ri[2] != static_cast<int>(w168) || ri[3] != 0xc060 || rb) { std::fprintf(stderr, "variant %d: resolver args/description\n", variant); ++failures; }
    }
    if (!failures) std::puts("Sparks msg_collision ok");
    return failures != 0;
}
