// Host check only. Not an EE build input.
#define NORRATH_HOST_CHECK 1
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cmath>
#include "init_attack_variant.cpp"

// Host memory: 32-bit "addresses" are offsets into one arena.
static unsigned char arena[1 << 16];
extern "C" unsigned char* norrath_host_pointer(unsigned word) { return arena + word; }
static unsigned word_of(const void* p) { return unsigned(static_cast<const unsigned char*>(p) - arena); }

extern "C" float worldDifficulty = 1.0f;
extern "C" unsigned skills[64];
extern "C" unsigned skills[64] = {};
extern "C" const unsigned norrath_rtti_entities_a_view_480bb8 = 0x3e4ccccc;

static int rolls, skill_calls, skill_a, skill_b, skill_result;
static unsigned roll_value;
static void* roll_arg;
static void* skill_self;
extern "C" unsigned eRand__10GameObject(void* self) { ++rolls; roll_arg = self; return roll_value; }
extern "C" int norrath_host_call_skill(unsigned word, void* self, int a, int b) {
    ++skill_calls; skill_self = self; skill_a = a; skill_b = b;
    return word == 0x7777u ? skill_result : -999;
}
static void put_word(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static void put_half(unsigned char* p, unsigned v) { std::memcpy(p, &v, 2); }
static unsigned get_half(const unsigned char* p) { unsigned v = 0; std::memcpy(&v, p, 2); return v; }

static int failures = 0;
static void expect(bool ok, const char* message) { if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; } }

int main() {
    // The skill object: pointer table at +96 -> a table whose slot at +96 is {adjustment 8, function 0x7777}.
    unsigned char* skill = arena + 0x1000;
    unsigned char* table = arena + 0x2000;
    put_word(skill + 96, word_of(table));
    put_half(table + 96, 8);
    put_word(table + 100, 0x7777u);
    skills[13] = word_of(skill);
    unsigned char* object = arena + 0x4000;
    for (int variant = -3; variant <= 7; ++variant) {
        std::memset(object, 0x5a, 1200);
        put_word(object + 208, unsigned(variant));
        unsigned char before[1200];
        std::memcpy(before, object, sizeof before);
        rolls = skill_calls = 0;
        roll_value = 0x00018000u;
        skill_result = 40;
        worldDifficulty = 3.9f;
        initAttackVariant__5Demon(object);
        for (int i = 0; i < 4; ++i) {
            // entries that the variant-1 path may rewrite are checked separately below
            if (variant != 1) expect(get_half(object + 1068 + 8 * i) == 0xffffu, "empty tags");
        }
        const unsigned want156 = (variant == 2 || variant == 3) ? 8 : variant == 4 ? 6 : variant == 1 ? 2 : 0;
        expect(object[156] == want156, "byte 156");
        expect(rolls == (variant == 1 ? 1 : 0) && skill_calls == (variant == 1 ? 1 : 0), "calls only for variant 1");
        if (variant == 1) {
            expect(roll_arg == object, "eRand receives the object");
            expect(skill_self == skill + 8 && skill_a == 0 && skill_b == 3, "skill call: adjusted object, 0, truncated difficulty");
            // 40 * 0.19999998807907104f = 7.9999995f; roll 0x18000 * 2^-16 = 1.5; product 11.99999.. -> 11
            expect(get_half(object + 1068) == 21u && get_half(object + 1070) == 40u && get_half(object + 1072) == 51u && get_half(object + 1074) == 0u, "first entry rewritten");
            expect(get_half(object + 1076) == 0xffffu && get_half(object + 1084) == 0xffffu && get_half(object + 1092) == 0xffffu, "other entries stay empty");
        }
        // bytes outside the written ones are untouched
        for (int i = 0; i < 1200; ++i) {
            const bool written = (i == 156) || (i >= 1068 && i < 1100);
            if (!written && object[i] != before[i]) { expect(false, "stray write"); break; }
        }
    }
    // variant 1 with prior entries: tag 21 is reused, other tags are skipped
    std::memset(object, 0x5a, 1200);
    put_word(object + 208, 1);
    // initialisation resets all four tags, so the scan always lands on the first entry
    roll_value = 0x80000000u;  // negative as a signed value: scaled by -0.5 * 2^15
    skill_result = 100;
    worldDifficulty = 7.0f;
    initAttackVariant__5Demon(object);
    // 100 * 0.19999998f = 19.999998; roll = -32768; product -655359.9 -> -655359; stored (short)(100 - 655359) = 0x...
    const int expected = int((100 * 0.19999998807907104f) * (float(int(0x80000000u)) * 1.52587890625e-05f));
    expect(get_half(object + 1072) == (unsigned(100 + expected) & 0xffffu), "negative roll wraps through a halfword");
    expect(skill_b == 7, "difficulty truncated");
    if (failures == 0) std::printf("initAttackVariant__5Demon cases passed\n");
    return failures == 0 ? 0 : 1;
}
