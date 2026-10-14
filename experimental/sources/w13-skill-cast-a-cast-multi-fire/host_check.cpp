// Host contract for cast__14SkillMultiFireiP8CreatureG6Point3s; not an EE input.
// 32-bit words (table words, item words, model and texture words) are offsets from a host base arena. The player table,
// the global item object and the two generator states are host objects. The table calls, trig, allocation, constructors,
// attack bonus, resource lookup and sound call are stubs that log. Build with -ffp-contract=off.
#define W13_SKILL_CAST_A_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_skill_cast_a_host_base;
static std::string calls;
static int tweak_result;
static int ramps[3];
static long off(const void* p) { return p ? static_cast<long>(static_cast<const unsigned char*>(p) - w13_skill_cast_a_host_base) : -1; }
static unsigned bits(float v) { unsigned u; std::memcpy(&u, &v, 4); return u; }
int w13_skill_cast_a_host_call(unsigned function_word, void* adjusted_skill, int a1, int a2) {
    calls += "t120(" + std::to_string(function_word) + "," + std::to_string(off(adjusted_skill)) + "," + std::to_string(a1) + "," + std::to_string(a2) + ");";
    return tweak_result;
}
int w13_skill_cast_a_host_call3(unsigned function_word, void* adjusted_skill, int a1, int a2, void* a3) {
    calls += "t104(" + std::to_string(function_word) + "," + std::to_string(off(adjusted_skill)) + "," + std::to_string(a1) + "," + std::to_string(a2) + "," + std::to_string(off(a3)) + ");";
    return ramps[a1];
}
extern "C" {
unsigned char player[23984 * 3];
unsigned char global_item[64] asm("_4item$globalItem");
int eRandState;
int syncRandState;
}
#include "cast.cpp"
static float cos_of(int a) { return static_cast<float>((a * 37) % 1000) / 1000.0f - 0.4f; }
static float sin_of(int a) { return static_cast<float>((a * 53) % 1000) / 997.0f - 0.7f; }
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned char* blocks[16];
static int block_index;
extern "C" {
void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return blocks[block_index++]; }
float icos__Fi(int angle) { calls += "cos(" + std::to_string(angle) + ");"; return cos_of(angle); }
float isin__Fi(int angle) { calls += "sin(" + std::to_string(angle) + ");"; return sin_of(angle); }
int getAttackBonus__4itemPQ24item11description(void* d) { calls += "attack(" + std::to_string(off(d)) + ");"; return 77; }
void* lumpFindResource__FPcT0(char* lump, char* name) { calls += std::string("lump(") + lump + "," + name + ");"; return arena + 0x3800; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume) {
    calls += "sound(" + std::to_string(off(sequence)) + "," + std::to_string(off(object)) + "," + std::to_string(bits(volume)) + ");";
}
void light_effect_ctor(void* self, Point3 position, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11,
                       float f12, void* object, int number) asm("__11LightEffectG6Point3ffffffffffffP10GameObjecti");
void light_effect_ctor(void* self, Point3 position, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11,
                       float f12, void* object, int number) {
    calls += "light(" + std::to_string(off(self)) + "," + std::to_string(bits(position.x)) + "," + std::to_string(bits(position.y)) + "," + std::to_string(bits(position.z));
    for (float f : {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12}) calls += "," + std::to_string(bits(f));
    calls += "," + std::to_string(off(object)) + "," + std::to_string(number) + ");";
}
void missile_weapon_ctor(void* self, int a, void* model, void* texture, Point3 position, Point3 velocity, int angle, void* creature, void* vag1, void* vag2, int low, int high,
                         int bonus, int item_word, int constant, int flags, void* description)
    asm("__13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description");
void missile_weapon_ctor(void* self, int a, void* model, void* texture, Point3 position, Point3 velocity, int angle, void* creature, void* vag1, void* vag2, int low, int high,
                         int bonus, int item_word, int constant, int flags, void* description) {
    calls += "missile(" + std::to_string(off(self)) + "," + std::to_string(a) + "," + std::to_string(off(model)) + "," + std::to_string(off(texture)) + "," +
             std::to_string(bits(position.x)) + "," + std::to_string(bits(position.y)) + "," + std::to_string(bits(position.z)) + "," + std::to_string(bits(velocity.x)) + "," +
             std::to_string(bits(velocity.y)) + "," + std::to_string(bits(velocity.z)) + "," + std::to_string(angle) + "," + std::to_string(off(creature)) + "," +
             std::to_string(off(vag1)) + "," + std::to_string(off(vag2)) + "," + std::to_string(low) + "," + std::to_string(high) + "," + std::to_string(bonus) + "," +
             std::to_string(item_word) + "," + std::to_string(constant) + "," + std::to_string(flags) + "," + std::to_string(off(description)) + ");";
}
}
int main() {
    int bad = 0;
    w13_skill_cast_a_host_base = arena;
    unsigned char* const skill = arena + 0x40;
    unsigned char* const creature = arena + 0x2000;
    unsigned char* const table = arena + 0x300;
    unsigned char* const item = arena + 0x800;
    unsigned word = static_cast<unsigned>(table - arena);
    std::memcpy(skill + 96, &word, 4);
    short adjust = -8;
    std::memcpy(table + 120, &adjust, 2);
    word = 0x4321; std::memcpy(table + 124, &word, 4);
    adjust = 12;
    std::memcpy(table + 104, &adjust, 2);
    word = 0x5432; std::memcpy(table + 108, &word, 4);
    for (int i = 0; i < 16; ++i) blocks[i] = arena + 0x1000 + 0x40 * i;
    const unsigned model_word = 0x1008, texture_word = 0x100c;
    std::memcpy(global_item + 8, &model_word, 4);
    std::memcpy(global_item + 12, &texture_word, 4);
    const struct { int rank, tweak; short angle; int flags; int index; int r[3]; int rec212, rec216; signed char slot; unsigned er, sr; bool light_ok; int unused; Point3 p; } cases[] = {
        {5, 9, 0, 0x40, 1, {1000, 300, 4}, 50, 120, 2, 0x12345678u, 0x9abcdef0u, true, 99, {1.5f, -2.25f, 30.0f}},
        {0, 3, -5, 0, 0, {500, 30, 3}, 0, 0, 0, 1u, 2u, true, 99, {0, 0, 0}},
        {7, -2, 32767, 0x40, 2, {-1000, -300, 5}, -50, -120, -3, 0xffffffffu, 0xfffffffeu, true, 99, {-1000.125f, 77.5f, 5.0f}},
        {-1, 4, 1234, 0, 1, {-700, 500, 2}, 5, 5, 1, 0x80000000u, 0x7fffffffu, true, 99, {1.5f, -2.25f, 30.0f}},
        {3, 4, -32768, 0x40, 0, {250, 100, 0}, 50, 120, 2, 5u, 6u, true, 99, {0, 0, 0}},
        {3, 4, 100, 0x40, 1, {250, 100, -3}, 50, 120, 2, 5u, 6u, true, 99, {-1000.125f, 77.5f, 5.0f}},
        {3, 4, 100, 0, 2, {250, 100, 1}, 50, 120, 2, 5u, 6u, false, 99, {1.5f, -2.25f, 30.0f}},
        {9, 1, 77, 0x40, 0, {33, 44, 3}, 100000, -100000, 127, 7u, 8u, true, 1, {0, 0, 0}},
        {2, 1, -77, 0, 0, {1, 44, 4}, 1, 1, -128, 9u, 10u, true, 99, {-1000.125f, 77.5f, 5.0f}},
        {1, 6, 5, 0x41, 2, {12345, 2000, 6}, 333, 444, 5, 11u, 12u, true, 99, {1.5f, -2.25f, 30.0f}},
        {6, 2, 20, 0, 0, {-1, 50, 3}, 1, 1, 1, 0x3c6ef35fu, 0u, true, 99, {0, 0, 0}},
        {6, 2, 20, 0, 0, {-300, 50, 3}, 1, 1, 1, 0xb1a5f1e9u, 3u, true, 99, {-1000.125f, 77.5f, 5.0f}},
        {6, 2, 20, 0, 0, {300, 50, 3}, 1, 1, 1, 0xb1a5f1e9u, 3u, true, 99, {1.5f, -2.25f, 30.0f}},
        {6, 2, 20, 0x40, 0, {130, 50, 4}, 2, -2, 1, 1u, 1u, true, 0, {0, 0, 0}},
        {6, 2, 20, 0x40, 1, {130, 50, 4}, -2, 2, 1, 1u, 1u, true, 0, {0, 0, 0}},
        {6, 2, 20, 0, 0, {-7, 50, 3}, 1, 1, 1, 0x1000u, 3u, true, 0, {0, 0, 0}},
        {6, 2, 20, 0, 0, {-7, 50, 3}, 1, 1, 1, 0x2000u, 3u, true, 0, {0, 0, 0}},
        {6, 2, 20, 0, 0, {7, 50, 3}, 1, 1, 1, 0x4000u, 3u, true, 0, {0, 0, 0}},
        {6, 2, 20, 0, 0, {7, 50, 3}, 1, 1, 1, 0x5000u, 3u, true, 0, {0, 0, 0}},
        {6, 2, 20, 0, 0, {-7, 50, 3}, 1, 1, 1, 0x3000u, 3u, true, 0, {0, 0, 0}},
        {6, 2, 20, 0, 0, {-9, 50, 3}, 1, 1, 1, 0x6000u, 3u, true, 0, {0, 0, 0}},
        {6, 2, 20, 0, 0, {9, 50, 3}, 1, 1, 1, 0x7000u, 3u, true, 0, {0, 0, 0}}};
    for (const auto& c : cases) {
        std::memset(creature, 0x11, 0x400);
        std::memcpy(creature + 12, &c.flags, 4);
        std::memcpy(creature + 960, &c.index, 4);
        std::memset(player, 0, sizeof player);
        std::memcpy(player + c.index * 23984 + 212, &c.rec212, 4);
        std::memcpy(player + c.index * 23984 + 216, &c.rec216, 4);
        player[c.index * 23984 + 23572] = static_cast<unsigned char>(c.slot);
        const unsigned item_word = static_cast<unsigned>(item - arena);
        std::memcpy(player + c.index * 23984 + 21960 + 4 * c.slot, &item_word, 4);
        const unsigned item_second = 0x5a5a1234u;
        std::memcpy(item + 8, &item_second, 4);
        tweak_result = c.tweak;
        ramps[0] = c.r[0]; ramps[1] = c.r[1]; ramps[2] = c.r[2];
        eRandState = static_cast<int>(c.er); syncRandState = static_cast<int>(c.sr);
        block_index = 0;
        calls.clear();
        if (!c.light_ok) blocks[0] = nullptr; else blocks[0] = arena + 0x1000;
        const Point3 p = c.p;
        if (cast_multi_fire(skill, c.rank, creature, p, c.angle) != 0) bad = 1;
        // independent expectation
        std::string e = "t120(17185," + std::to_string(off(skill - 8)) + "," + std::to_string(c.rank) + ",20);alloc(188);";
        if (c.light_ok) {
            e += "light(" + std::to_string(off(blocks[0])) + "," + std::to_string(bits(p.x + 0.0f)) + "," + std::to_string(bits(p.y)) + "," + std::to_string(bits(p.z + 20.0f));
            for (float f : {124.0f, 115.0f, 204.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 130.09157f, 60.0f, 0.5f, 1.0f}) e += "," + std::to_string(bits(f));
            e += ",-1,0);";
        }
        const int count = c.r[2];
        const float count_f = static_cast<float>(count);
        const short start = static_cast<short>(c.angle - static_cast<int>(count_f * 0.5f * 534.5f));
        const float speed = static_cast<float>(c.r[1]);
        e += "t104(21554," + std::to_string(off(skill + 12)) + ",2," + std::to_string(c.tweak) + "," + std::to_string(off(creature)) + ");";
        e += "t104(21554," + std::to_string(off(skill + 12)) + ",1," + std::to_string(c.tweak) + "," + std::to_string(off(creature)) + ");";
        e += "t104(21554," + std::to_string(off(skill + 12)) + ",0," + std::to_string(c.rank) + "," + std::to_string(off(creature)) + ");";
        int low, high, bonus = 0, item_w = 0, flags;
        auto rounded = [](float v) { return static_cast<int>(0.0f <= v ? v + 0.5f : v - 0.5f); };
        if (c.flags & 0x40) {
            const float scale = static_cast<float>(c.r[0]) * 0.01f / count_f;
            low = rounded(static_cast<float>(c.rec212) * scale);
            high = rounded(static_cast<float>(c.rec216) * scale);
            e += "attack(" + std::to_string(off(item)) + ");";
            bonus = 77; item_w = static_cast<int>(item_second); flags = 0x40040000;
        } else {
            const float x = static_cast<float>(c.r[0]) * 0.2f;
            unsigned s1 = c.er * 0x19660du + 0x3c6ef35fu;
            const float r1 = static_cast<float>(s1 >> 16) * (1.0f / 65536.0f);
            unsigned s2 = s1 * 0x19660du + 0x3c6ef35fu;
            const float r2 = static_cast<float>(s2 >> 16) * (1.0f / 65536.0f);
            low = c.r[0];
            const float delta = 0.0f <= x * r1 ? x * r2 + 0.5f : x * r2 - 0.5f;
            high = c.r[0] + static_cast<int>(delta);
            flags = 0x40000000;
            if (static_cast<unsigned>(eRandState) != s2) { std::puts("eRandState"); bad = 3; }
        }
        unsigned sync = c.sr;
        short a = start;
        if (count > 0)
            for (int i = 0; i < count; ++i) {
                sync = sync * 0x19660du + 0x3c6ef35fu;
                const float s = static_cast<float>(sync >> 16) * (1.0f / 65536.0f);
                Point3 q{p.x, p.y, (p.z - s * 6.0f) + -6.0f};
                Point3 v{cos_of(a) * speed, sin_of(a) * speed, 0.0f};
                e += "cos(" + std::to_string(a) + ");sin(" + std::to_string(a) + ");alloc(452);missile(" + std::to_string(off(arena + 0x1000 + 0x40 * (i + 1))) + ",0,4104,4108," +
                     std::to_string(bits(q.x)) + "," + std::to_string(bits(q.y)) + "," + std::to_string(bits(q.z)) + "," + std::to_string(bits(v.x)) + "," + std::to_string(bits(v.y)) + "," +
                     std::to_string(bits(v.z)) + "," + std::to_string(a - 16384) + "," + std::to_string(off(creature)) + ",-1,-1," + std::to_string(low) + "," + std::to_string(high) + "," +
                     std::to_string(bonus) + "," + std::to_string(item_w) + ",10000," + std::to_string(flags) + ",-1);";
                a = static_cast<short>(a + 1069);
            }
        if (static_cast<unsigned>(syncRandState) != sync) { std::puts("syncRandState"); bad = 4; }
        e += "lump(fx.lmp,spel_we_firearrow.anm);sound(" + std::to_string(off(arena + 0x3800)) + "," + std::to_string(off(creature)) + "," + std::to_string(bits(1.0f)) + ");";
        if (calls != e) { std::puts(calls.c_str()); std::puts(e.c_str()); bad = 2; }
    }
    return bad;
}
