// Host check only. Not an EE build input.
#define NORRATH_HOST_CHECK 1
#include <cstdint>
#include <initializer_list>
#include <cstdio>
#include <cstring>
#include <cmath>
#include "init_type.cpp"

static unsigned char arena[1 << 16];
extern "C" unsigned char* norrath_host_pointer(unsigned word) { return arena + word; }
static unsigned word_of(const void* p) { return unsigned(static_cast<const unsigned char*>(p) - arena); }

extern "C" float worldDifficulty = 1.0f;
extern "C" unsigned skills[64] = {};
extern "C" const unsigned norrath_rtti_entities_a_view_482424 = 0x3f999999;
extern "C" const unsigned norrath_rtti_entities_a_view_482428 = 0x3fb33333;
extern "C" const unsigned norrath_rtti_entities_a_view_48242c = 0x3f999999;
extern "C" const unsigned norrath_rtti_entities_a_view_482430 = 0x3e4ccccc;
extern "C" const unsigned norrath_rtti_entities_a_view_482434 = 0x3e4ccccc;
extern "C" const unsigned norrath_rtti_entities_a_view_482438 = 0x3e4ccccc;

static int rolls, skill_calls, skill_a, skill_b, skill_result, params_calls, params_flag;
static unsigned roll_value;
static float params[5];
static void* params_object;
static void* skill_self;
extern "C" unsigned eRand__10GameObject(void*) { ++rolls; return roll_value; }
extern "C" int norrath_host_call_skill(unsigned, void* self, int a, int b) {
    ++skill_calls; skill_self = self; skill_a = a; skill_b = b;
    return skill_result;
}
extern "C" unsigned norrath_actor_set_parameters(void* object, float a, float b, float c, float d, float e, int flag) {
    ++params_calls; params_object = object; params[0] = a; params[1] = b; params[2] = c; params[3] = d; params[4] = e; params_flag = flag;
    return 0;
}
static void put_word(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static void put_half(unsigned char* p, unsigned v) { std::memcpy(p, &v, 2); }
static unsigned get_half(const unsigned char* p) { unsigned v = 0; std::memcpy(&v, p, 2); return v; }

static int failures = 0;
static void expect(bool ok, const char* message) { if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; } }

int main() {
    unsigned char* skill_objects[3] = {arena + 0x1000, arena + 0x1800, arena + 0x1c00};
    unsigned char* table = arena + 0x2000;
    put_half(table + 96, 8);
    put_word(table + 100, 0x7777u);
    const unsigned indexes[3] = {14u, 25u, 26u};
    for (int i = 0; i < 3; ++i) {
        put_word(skill_objects[i] + 96, word_of(table));
        skills[indexes[i]] = word_of(skill_objects[i]);
    }
    unsigned char* object = arena + 0x4000;
    for (int b960 : {0, 1, 2, -1})
        for (bool flag : {false, true})
            for (int variant = -2; variant <= 5; ++variant) {
                std::memset(object, 0x5a, 1200);
                put_word(object + 208, unsigned(variant));
                object[960] = static_cast<unsigned char>(b960);
                unsigned char before[1200];
                std::memcpy(before, object, sizeof before);
                rolls = skill_calls = params_calls = 0;
                roll_value = 0x00018000u;
                skill_result = 40;
                worldDifficulty = 3.9f;
                initType__13FemaleDarkElfb(object, flag);
                expect(params_calls == 1 && params_object == object && params_flag == (flag ? 0 : 1), "setParameters once with the flag inverted");
                if (b960 == 1) {
                    expect(params[0] == 1.0f && std::fabs(params[1] - 1.2f) < 1e-6f && params[2] == 1.0f && params[3] == 1.0f && params[4] == 1.0f, "parameters for byte 960 == 1");
                } else {
                    expect(std::fabs(params[0] - 1.4f) < 1e-6f && std::fabs(params[1] - 1.2f) < 1e-6f && params[2] == 1.0f && params[3] == 1.0f && params[4] == 1.0f, "parameters otherwise");
                }
                const bool step = variant >= 1 && variant <= 3;
                expect(rolls == (step ? 1 : 0) && skill_calls == (step ? 1 : 0), "skill step for variants 1..3");
                unsigned want156 = b960 == 1 ? 6 : 0;
                if (variant == 1) want156 = 4; else if (variant == 2) want156 = 3; else if (variant == 3) want156 = 7;
                expect(object[156] == want156, "byte 156");
                for (int i = 1; i < 4; ++i) expect(get_half(object + 1068 + 8 * i) == 0xffffu, "later entries empty");
                if (step) {
                    const unsigned tag = variant == 1 ? 23u : variant == 2 ? 22u : 38u;
                    const unsigned char* who = variant == 1 ? skill_objects[2] : variant == 2 ? skill_objects[0] : skill_objects[1];
                    expect(skill_self == who + 8 && skill_a == 0 && skill_b == 3, "skill call arguments");
                    // 40 + (int)(40 * 0.19999998 * 1.5) = 40 + 11
                    expect(get_half(object + 1068) == tag && get_half(object + 1070) == 40u && get_half(object + 1072) == 51u && get_half(object + 1074) == 0u, "entry written");
                } else {
                    expect(get_half(object + 1068) == 0xffffu, "first entry empty");
                }
                for (int i = 0; i < 1200; ++i) {
                    const bool written = (i == 156) || (i >= 1068 && i < 1100);
                    if (!written && object[i] != before[i]) { expect(false, "stray write"); break; }
                }
            }
    if (failures == 0) std::printf("initType__13FemaleDarkElfb cases passed\n");
    return failures == 0 ? 0 : 1;
}
