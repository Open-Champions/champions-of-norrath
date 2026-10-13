// Host contract for msg_trigger__14WoodElfSoldieri; not an EE input. Pointer words are offsets from a host base
// and every callee is a recording stub.
#define W11_ACTOR_C_HOST 1
#include "msg_trigger.cpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w11_actor_c_host_base;
W11ActorCHostSlot w11_actor_c_host_slots[1];
unsigned char globalItem[64];
static std::string calls;
static bool tagFound, allocFails;
static unsigned long long chunksValue = 0x1122334455667788ull;
static unsigned rollValue;
static unsigned lastFlags;
static float lastPos[3], lastVel[3];
static int argsInt[8];
extern "C" {
void* findTagString__10GameObjectPci(void*, const char* tag, int occurrence) { calls += std::string("tag(") + tag + "," + std::to_string(occurrence) + ");"; return tagFound ? globalItem : nullptr; }
void msg_trigger__8Creaturei(void*, int trigger) { calls += "creature(" + std::to_string(trigger) + ");"; }
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int requested) {
    calls += "chunks(" + std::to_string(static_cast<unsigned char*>(model) - w11_actor_c_host_base) + "," + std::to_string(requested) + ");"; return chunksValue; }
float icos__Fi(int a) { calls += "cos(" + std::to_string(a) + ");"; return 0.5f; }
float isin__Fi(int a) { calls += "sin(" + std::to_string(a) + ");"; return -0.25f; }
int eRand__10GameObject(void*) { calls += "rand;"; return static_cast<int>(rollValue); }
int sprintf(char* d, const char* f, ...) { __builtin_va_list ap; __builtin_va_start(ap, f); int v = __builtin_va_arg(ap, int); __builtin_va_end(ap);
    std::string s = f; s.replace(s.find("%d"), 2, std::to_string(v)); std::memcpy(d, s.c_str(), s.size() + 1); return static_cast<int>(s.size()); }
void* lumpFindResource__FPcT0(char* lump, char* name) { calls += std::string("lump(") + lump + "," + name + ");"; return globalItem + 40; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* a, void* o, float g) {
    calls += "sound(" + std::to_string(static_cast<unsigned char*>(a) - globalItem) + "," + std::to_string(static_cast<unsigned char*>(o) - w11_actor_c_host_base) + "," + std::to_string(g) + ");"; }
void* blockAlloc__Fi(int n) { calls += "alloc" + std::to_string(n) + ";"; return allocFails ? nullptr : globalItem + 48; }
void __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
    void* self, int zero, void* model, void* texture, const float* position, const float* velocity, int angle, void* creature, void* sound_a, void* sound_b,
    int a, int b, int c, int d, int e, int f, void* description) {
    calls += "missile(" + std::to_string(zero) + "," + std::to_string(angle) + ");";
    std::memcpy(lastPos, position, 12); std::memcpy(lastVel, velocity, 12);
    argsInt[0] = a; argsInt[1] = b; argsInt[2] = c; argsInt[3] = d; argsInt[4] = e; argsInt[5] = f;
    if (model != w11_actor_c_host_base + 7000 || texture != w11_actor_c_host_base + 7100 || creature == nullptr || sound_a || sound_b || description) calls += "badargs;";
}
}
int main() {
    static unsigned char base[16384];
    w11_actor_c_host_base = base;
    int failures = 0;
    unsigned char* self = base + 64;
    for (int variant = 0; variant < 5; ++variant) {
        std::memset(base, 0, sizeof base);
        const unsigned res = 1024, w12 = 2048; std::memcpy(self + 36, &res, 4); std::memcpy(base + 1024 + 12, &w12, 4);
        const unsigned flags = 0xfffffffu | 0x400u; std::memcpy(self + 244, &flags, 4);
        const float pos[3] = {1.0f, 2.0f, 3.0f}; std::memcpy(self + 16, pos, 12);
        const short heading = static_cast<short>(0x7123); std::memcpy(self + 56, &heading, 2);
        const short h200 = 300, h202 = -7, h204 = 55; std::memcpy(self + 200, &h200, 2); std::memcpy(self + 202, &h202, 2); std::memcpy(self + 204, &h204, 2);
        const unsigned item8 = 7000, item12 = 7100; std::memcpy(globalItem + 8, &item8, 4); std::memcpy(globalItem + 12, &item12, 4);
        tagFound = variant == 0; allocFails = variant == 1; rollValue = variant == 4 ? 0x1237u : 2u; calls.clear();
        msg_trigger__14WoodElfSoldieri(self, 91);
        unsigned flagsAfter; std::memcpy(&flagsAfter, self + 244, 4);
        unsigned long long stored; std::memcpy(&stored, self + 552, 8);
        if (variant == 0) {
            if (calls != "tag(dialogScript,0);creature(91);" || flagsAfter != flags || stored != 0) { std::fprintf(stderr, "tag path: %s\n", calls.c_str()); ++failures; }
            continue;
        }
        const std::string roll = std::to_string((variant == 4 ? 0x1237 & 3 : 2) + 1);
        std::string want = "tag(dialogScript,0);chunks(2048," + std::to_string(flags & ~0x400u) + ");cos(" + std::to_string(0x7123) + ");sin(" + std::to_string(0x7123) + ");rand;lump(fx.lmp,swsh_arrow" + roll + ".anm);sound(40,64,1.000000);alloc452;";
        if (variant != 1) want += "missile(0," + std::to_string(0x7123 - 16384) + ");";
        if (calls != want || flagsAfter != (flags & ~0x400u) || stored != chunksValue) { std::fprintf(stderr, "variant %d:\n got  %s\n want %s\n", variant, calls.c_str(), want.c_str()); ++failures; }
        if (variant != 1) {
            const float wantPos[3] = {1.0f, 2.0f, 61.0f}, wantVel[3] = {200.0f, -100.0f, 0.0f};
            if (std::memcmp(lastPos, wantPos, 12) || std::memcmp(lastVel, wantVel, 12) || argsInt[0] != 300 || argsInt[1] != -3 || argsInt[2] != 0 || argsInt[3] != 0 || argsInt[4] != 55 || argsInt[5] != 4) {
                std::fprintf(stderr, "variant %d: missile arguments\n", variant); ++failures; }
        }
    }
    if (!failures) std::puts("WoodElfSoldier msg_trigger ok");
    return failures != 0;
}
