// Host contract for cast__10SkillShockiP8CreatureG6Point3s; not an EE input.
#include <string>
struct Point3 { float x, y, z; };
static std::string calls;
static void* fake_block;
extern "C" {
void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return fake_block; }
void projectile_ctor_shock(void* self, Point3 point, short angle, void* creature, void* skill, int rank) asm("__15ShockProjectileG6Point3sP10GameObjectP5Skilli");
void projectile_ctor_shock(void* self, Point3 point, short angle, void* creature, void* skill, int rank) {
    calls += "ctor(" + std::to_string(self == fake_block) + "," + std::to_string(point.x) + "," + std::to_string(point.y) + "," + std::to_string(point.z) + "," +
             std::to_string(angle) + "," + std::to_string(reinterpret_cast<unsigned long>(creature)) + "," +
             std::to_string(reinterpret_cast<unsigned long>(skill)) + "," + std::to_string(rank) + ");";
}
int cast_shocki(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__10SkillShockiP8CreatureG6Point3s");
}
int main() {
    char block, skill, creature;
    const int ranks[] = {-3, 0, 1, 99};
    const short angles[] = {-32768, -1, 0, 7, 32767};
    for (int rank : ranks)
        for (short angle : angles) {
            fake_block = &block; calls.clear();
            if (cast_shocki(&skill, rank, &creature, Point3{1.5f, -2.0f, 3.25f}, angle) != 0) return 1;
            const std::string expected = "alloc(408);ctor(1,1.500000,-2.000000,3.250000," + std::to_string(angle) + "," +
                                         std::to_string(reinterpret_cast<unsigned long>(&creature)) + "," +
                                         std::to_string(reinterpret_cast<unsigned long>(&skill)) + "," + std::to_string(rank) + ");";
            if (calls != expected) return 2;
            fake_block = nullptr; calls.clear();
            if (cast_shocki(&skill, rank, &creature, Point3{0, 0, 0}, angle) != 0 || calls != "alloc(408);") return 3;
        }
    return 0;
}
