// Host contract for cast__17SkillShockOfFrostiP8CreatureG6Point3s; not an EE input.
#include <string>
static std::string calls;
static void* fake_block;
struct Point3 { float x, y, z; };
extern "C" {
void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return fake_block; }
void shock_of_frost_ctor(void* self, Point3 point, short angle, int rank, void* creature, void* skill) asm("__12ShockOfFrostG6Point3siP10GameObjectP5Skill");
void shock_of_frost_ctor(void* self, Point3 point, short angle, int rank, void* creature, void* skill) {
    calls += "ctor(" + std::to_string(self == fake_block) + "," + std::to_string(point.x) + "," + std::to_string(point.y) + "," + std::to_string(point.z) + "," +
             std::to_string(angle) + "," + std::to_string(rank) + "," + std::to_string(reinterpret_cast<unsigned long>(creature)) + "," +
             std::to_string(reinterpret_cast<unsigned long>(skill)) + ");";
}
int cast_shock_of_frost(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__17SkillShockOfFrostiP8CreatureG6Point3s");
}
int main() {
    char block, skill, creature;
    const int ranks[] = {-3, 0, 1, 99};
    const short angles[] = {0, 5, -7, 32767, -32768};
    for (int rank : ranks)
        for (short angle : angles) {
            fake_block = &block; calls.clear();
            int r = cast_shock_of_frost(&skill, rank, &creature, Point3{1.5f, -2.0f, 3.25f}, angle);
            const std::string expected = "alloc(208);ctor(1," + std::to_string(1.5f) + "," + std::to_string(-2.0f) + "," + std::to_string(3.25f) + "," + std::to_string(angle) + "," +
                                         std::to_string(rank) + "," + std::to_string(reinterpret_cast<unsigned long>(&creature)) + "," + std::to_string(reinterpret_cast<unsigned long>(&skill)) + ");";
            if (r != 0 || calls != expected) return 1;
            fake_block = nullptr; calls.clear();
            r = cast_shock_of_frost(&skill, rank, &creature, Point3{0, 0, 0}, angle);
            if (r != 0 || calls != "alloc(208);") return 2;
        }
    return 0;
}
