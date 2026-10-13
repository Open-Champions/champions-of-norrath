// Host check only. Not an EE build input.
#define NORRATH_HOST_CHECK 1
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cmath>
#include "init_type.cpp"

static unsigned char arena[1 << 16];
extern "C" unsigned char* norrath_host_pointer(unsigned word) { return arena + word; }
static unsigned word_of(const void* p) { return unsigned(static_cast<const unsigned char*>(p) - arena); }

extern "C" float worldDifficulty = 1.0f;
extern "C" unsigned skills[64] = {};
extern "C" const unsigned norrath_rtti_entities_a_view_4818ac[19] = {
    0x3f333333u, 0x3f4cccccu, 0x3f333333u, 0x3fb33333u, 0x3fccccccu, 0x3fccccccu, 0x3f333333u,
    0x3fb33333u, 0x3e4cccccu, 0x3f4cccccu, 0x3f333333u, 0x3f4cccccu, 0x3f333333u, 0x3fccccccu,
    0x3e4cccccu, 0x3e4cccccu, 0x3e4cccccu, 0x3e4cccccu, 0x3e4cccccu};

static int rolls, skill_calls, skill_a, skill_b, skill_result, params_calls, params_flag;
static unsigned roll_value;
static float params[5];
static void* skill_self;
extern "C" unsigned eRand__10GameObject(void*) { ++rolls; return roll_value; }
extern "C" int norrath_host_call_skill(unsigned, void* self, int a, int b) {
    ++skill_calls; skill_self = self; skill_a = a; skill_b = b;
    return skill_result;
}
extern "C" unsigned norrath_actor_set_parameters(void*, float a, float b, float c, float d, float e, int flag) {
    ++params_calls; params[0] = a; params[1] = b; params[2] = c; params[3] = d; params[4] = e; params_flag = flag;
    return 0;
}
static void put_word(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static void put_half(unsigned char* p, unsigned v) { std::memcpy(p, &v, 2); }
static unsigned get_half(const unsigned char* p) { unsigned v = 0; std::memcpy(&v, p, 2); return v; }
static unsigned get_word(const unsigned char* p) { unsigned v = 0; std::memcpy(&v, p, 4); return v; }
static unsigned long long get_quad(const unsigned char* p) { unsigned long long v = 0; std::memcpy(&v, p, 8); return v; }

static int failures = 0;
static void expect(bool ok, const char* message) { if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; } }
static bool near(float a, float b) { return std::fabs(a - b) < 1e-6f; }

int main() {
    unsigned char* skill_objects[4] = {arena + 0x1000, arena + 0x1400, arena + 0x1800, arena + 0x1c00};
    unsigned char* table = arena + 0x2000;
    put_half(table + 96, 8);
    put_word(table + 100, 0x7777u);
    const unsigned indexes[4] = {12u, 13u, 14u, 26u};
    for (int i = 0; i < 4; ++i) {
        put_word(skill_objects[i] + 96, word_of(table));
        skills[indexes[i]] = word_of(skill_objects[i]);
    }
    unsigned char* object = arena + 0x4000;
    const int variants[] = {-1, 0, 1, 2, 3, 4, 5, 6};
    for (int variant : variants)
        for (int flag = 0; flag < 2; ++flag)
            for (int type = -2; type <= 6; ++type) {
                std::memset(object, 0x5a, 1300);
                put_word(object + 972, unsigned(variant));
                put_word(object + 208, unsigned(type));
                unsigned char before[1300];
                std::memcpy(before, object, sizeof before);
                rolls = skill_calls = params_calls = 0;
                roll_value = 0x00018000u;
                skill_result = 40;
                worldDifficulty = 3.9f;
                initType__6Goblinb(object, flag != 0);
                const unsigned long long start = get_quad(before + 160) & ~0x2600000000ull;
                unsigned long long want_bits = start;
                unsigned want156 = 0;
                float p[5] = {};
                bool call = true;
                switch (variant) {
                case 0: p[0] = p[1] = p[2] = p[3] = 0.7f; p[4] = 0.8f; want_bits |= 1ull << 33; break;
                case 1: p[0] = p[1] = p[3] = p[4] = 1.5f; p[2] = 0.7f; want_bits |= 1ull << 33; break;
                case 2: p[0] = 2.0f; p[1] = 1.6f; p[2] = 0.7f; p[3] = 1.4f; p[4] = 1.6f; want_bits |= 1ull << 37; break;
                case 3: p[0] = 1.6f; p[1] = 1.4f; p[2] = 1.5f; p[3] = 1.4f; p[4] = 1.5f; want_bits |= 1ull << 37; break;
                case 4: p[0] = p[1] = p[3] = p[4] = 0.8f; p[2] = 0.7f; want_bits |= 1ull << 33;
                    want156 = type == 1 ? 3 : (type > 0 && type < 4) ? 2 : 0; break;
                case 5: p[0] = 0.8f; p[1] = 1.0f; p[2] = 0.7f; p[3] = 1.0f; p[4] = 1.6f; want_bits |= 1ull << 34; want156 = 6; break;
                default: call = false; break;
                }
                expect(params_calls == (call ? 1 : 0), "one setParameters call for variants 0..5");
                if (call) {
                    expect(params_flag == (flag ? 0 : 1), "flag inverted");
                    for (int k = 0; k < 5; ++k) expect(near(params[k], p[k]), "parameter singles");
                }
                expect(get_quad(object + 160) == want_bits, "bits at +160");
                if (variant == 2 || variant == 3) expect(get_word(object + 100) == 0x3e4ccccc, "+100");
                if (variant == 3) expect(get_word(object + 28) == 0x41c80000u && get_word(object + 224) == 0x3fb33333u, "+28 and +224");
                if (variant == 5) expect(get_word(object + 104) == 0x3e4ccccc, "+104");
                // skill step
                int tag = 0, which = -1;
                if (variant != 4) {
                    if (type == 2) { tag = 22; which = 2; want156 = 3; }
                    else if (type == 1) { tag = 24; which = 0; want156 = 1; }
                    else if (type == 3) { tag = 21; which = 1; want156 = 2; }
                    else if (type == 4) { tag = 23; which = 3; want156 = 4; }
                }
                expect(object[156] == want156, "byte 156");
                expect(skill_calls == (which >= 0 ? 1 : 0) && rolls == (which >= 0 ? 1 : 0), "skill step count");
                for (int i = 1; i < 4; ++i) expect(get_half(object + 1100 + 8 * i) == 0xffffu, "later entries empty");
                if (which >= 0) {
                    expect(skill_self == skill_objects[which] + 8 && skill_a == 0 && skill_b == 3, "skill call arguments");
                    expect(get_half(object + 1100) == unsigned(tag) && get_half(object + 1102) == 40u && get_half(object + 1104) == 51u && get_half(object + 1106) == 0u, "entry written");
                } else {
                    expect(get_half(object + 1100) == 0xffffu, "first entry empty");
                }
                for (int i = 0; i < 1300; ++i) {
                    const bool written = i == 156 || (i >= 160 && i < 168) || (i >= 1100 && i < 1132) || (i >= 28 && i < 32) || (i >= 100 && i < 108) || (i >= 224 && i < 228);
                    if (!written && object[i] != before[i]) { expect(false, "stray write"); break; }
                }
            }
    if (failures == 0) std::printf("initType__6Goblinb cases passed\n");
    return failures == 0 ? 0 : 1;
}
