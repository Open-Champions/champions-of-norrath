// Host contract for cast__13SkillDarknessiP8CreatureG6Point3s; not an EE input.
// The trig lookups, allocation and projectile constructor are stubs that log. Build with -ffp-contract=off.
#include <cstring>
#include <string>
struct Point3 { float x, y, z; };
static std::string calls;
static void* fake_block;
static unsigned bits(float v) { unsigned u; std::memcpy(&u, &v, 4); return u; }
static float cos_of(int a) { return static_cast<float>((a * 37) % 1000) / 1000.0f - 0.4f; }
static float sin_of(int a) { return static_cast<float>((a * 53) % 1000) / 997.0f - 0.7f; }
extern "C" {
void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return fake_block; }
float icos__Fi(int angle) { calls += "cos(" + std::to_string(angle) + ");"; return cos_of(angle); }
float isin__Fi(int angle) { calls += "sin(" + std::to_string(angle) + ");"; return sin_of(angle); }
void projectile_ctor_darkness(void* self, Point3 start, Point3 direction, void* skill, int rank, void* creature) asm("__18DarknessProjectileG6Point3T1P5SkilliP10GameObject");
void projectile_ctor_darkness(void* self, Point3 start, Point3 direction, void* skill, int rank, void* creature) {
    calls += "ctor(" + std::to_string(self == fake_block) + "," + std::to_string(bits(start.x)) + "," + std::to_string(bits(start.y)) + "," + std::to_string(bits(start.z)) + "," +
             std::to_string(bits(direction.x)) + "," + std::to_string(bits(direction.y)) + "," + std::to_string(bits(direction.z)) + "," +
             std::to_string(reinterpret_cast<unsigned long>(skill)) + "," + std::to_string(rank) + "," + std::to_string(reinterpret_cast<unsigned long>(creature)) + ");";
}
int cast_darkness(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__13SkillDarknessiP8CreatureG6Point3s");
}
int main() {
    char block, skill;
    alignas(8) unsigned char creature[64];
    const float positions[][3] = {{1.5f, -2.25f, 30.0f}, {-0.0f, -0.0f, -0.0f}, {-1e30f, 7.0f, -36.0f}, {0, 0, 0}};
    for (const auto& pos : positions)
        for (int rank : {-3, 0, 99})
            for (short angle : {static_cast<short>(-32768), static_cast<short>(-1), static_cast<short>(0), static_cast<short>(1234), static_cast<short>(32767)}) {
                std::memset(creature, 0x11, sizeof creature);
                std::memcpy(creature + 16, pos, 12);
                const float dx = cos_of(angle) * 373.0f, dy = sin_of(angle) * 373.0f;
                const float sx = pos[0] + 0.0f, sz = pos[2] + 36.0f;
                fake_block = &block; calls.clear();
                if (cast_darkness(&skill, rank, creature, Point3{9, 9, 9}, angle) != 0) return 1;
                const std::string expected = "cos(" + std::to_string(angle) + ");sin(" + std::to_string(angle) + ");alloc(176);ctor(1," + std::to_string(bits(sx)) + "," +
                                             std::to_string(bits(pos[1])) + "," + std::to_string(bits(sz)) + "," + std::to_string(bits(dx)) + "," + std::to_string(bits(dy)) + ",0," +
                                             std::to_string(reinterpret_cast<unsigned long>(&skill)) + "," + std::to_string(rank) + "," + std::to_string(reinterpret_cast<unsigned long>(creature)) + ");";
                if (calls != expected) return 2;
                fake_block = nullptr; calls.clear();
                if (cast_darkness(&skill, rank, creature, Point3{0, 0, 0}, angle) != 0 || calls != "cos(" + std::to_string(angle) + ");sin(" + std::to_string(angle) + ");alloc(176);") return 3;
            }
    return 0;
}
