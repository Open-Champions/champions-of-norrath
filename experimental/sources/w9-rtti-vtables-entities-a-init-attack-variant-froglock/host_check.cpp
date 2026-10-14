// Host check only. Not an EE build input.
#define NORRATH_HOST_CHECK 1
#include <cstdint>
#include <cstdio>
#include <cstring>
#include "init_attack_variant.cpp"

static unsigned char arena[1 << 16];
extern "C" unsigned char* norrath_host_pointer(unsigned word) { return arena + word; }
static unsigned word_of(const void* p) { return unsigned(static_cast<const unsigned char*>(p) - arena); }

extern "C" float worldDifficulty = 1.0f;
extern "C" unsigned skills[64] = {};
extern "C" const unsigned norrath_rtti_entities_a_view_4810b4 = 0x3e4ccccc;
extern "C" const unsigned norrath_rtti_entities_a_view_4810b8 = 0x3e4ccccc;

static int rolls, skill_calls, skill_a, skill_b, skill_result;
static unsigned roll_value, skill_word;
static void* roll_arg;
static void* skill_self;
extern "C" unsigned eRand__10GameObject(void* self) { ++rolls; roll_arg = self; return roll_value; }
extern "C" int norrath_host_call_skill(unsigned word, void* self, int a, int b) {
    ++skill_calls; skill_word = word; skill_self = self; skill_a = a; skill_b = b;
    return skill_result;
}
static void put_word(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static void put_half(unsigned char* p, unsigned v) { std::memcpy(p, &v, 2); }
static unsigned get_half(const unsigned char* p) { unsigned v = 0; std::memcpy(&v, p, 2); return v; }

static int failures = 0;
static void expect(bool ok, const char* message) { if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; } }

int main() {
    unsigned char* skill_a_object = arena + 0x1000;
    unsigned char* skill_b_object = arena + 0x1800;
    unsigned char* table = arena + 0x2000;
    put_word(skill_a_object + 96, word_of(table));
    put_word(skill_b_object + 96, word_of(table));
    put_half(table + 96, 8);
    put_word(table + 100, 0x7777u);
    skills[26] = word_of(skill_a_object);
    skills[14] = word_of(skill_b_object);
    unsigned char* object = arena + 0x4000;
    const unsigned modes[] = {17u, 16u, 0u, 0xffffffffu};
    for (unsigned mode : modes)
        for (int variant = -3; variant <= 7; ++variant) {
            std::memset(object, 0x5a, 1200);
            put_word(object + 208, unsigned(variant));
            put_word(object + 960, mode);
            unsigned char before[1200];
            std::memcpy(before, object, sizeof before);
            rolls = skill_calls = 0;
            roll_value = 0x00018000u;
            skill_result = 40;
            worldDifficulty = 3.9f;
            initAttackVariant__8Froglock(object);
            unsigned want156 = 0;
            const bool step = mode == 17u && (variant == 1 || variant == 2);
            if (mode == 17u) want156 = variant == 1 ? 4 : variant == 2 ? 3 : 0;
            else want156 = variant == 1 ? 4 : variant == 2 ? 3 : (variant == 3 || variant == 4) ? 2 : 0;
            expect(object[156] == want156, "byte 156");
            expect(rolls == (step ? 1 : 0) && skill_calls == (step ? 1 : 0), "skill step only for mode 17 variants 1 and 2");
            for (int i = 1; i < 4; ++i) expect(get_half(object + 1068 + 8 * i) == 0xffffu, "later entries empty");
            if (step) {
                const unsigned tag = variant == 1 ? 23u : 22u;
                const unsigned char* who = variant == 1 ? skill_a_object : skill_b_object;
                expect(skill_self == who + 8 && skill_a == 0 && skill_b == 3 && roll_arg == object, "skill call arguments");
                // 40 + 40 * 0.19999998 * 1.5 = 51.999999, which rounds to the single 52.0
                expect(get_half(object + 1068) == tag && get_half(object + 1070) == 40u && get_half(object + 1072) == 52u && get_half(object + 1074) == 0u, "entry written");
            } else {
                expect(get_half(object + 1068) == 0xffffu, "first entry empty");
            }
            for (int i = 0; i < 1200; ++i) {
                const bool written = (i == 156) || (i >= 1068 && i < 1100);
                if (!written && object[i] != before[i]) { expect(false, "stray write"); break; }
            }
        }
    // a negative roll
    std::memset(object, 0x5a, 1200);
    put_word(object + 208, 1); put_word(object + 960, 17);
    roll_value = 0x80000000u; skill_result = 100; worldDifficulty = 7.0f;
    initAttackVariant__8Froglock(object);
    const float whole = 100.0f;
    const int total = int(whole + whole * 0.19999998807907104f * (float(int(0x80000000u)) * 1.52587890625e-05f));
    expect(get_half(object + 1072) == (unsigned(total) & 0xffffu), "negative roll");
    if (failures == 0) std::printf("initAttackVariant__8Froglock cases passed\n");
    return failures == 0 ? 0 : 1;
}
