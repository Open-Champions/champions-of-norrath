// Host contract for cast__15SkillFireFlaresiP8CreatureG6Point3s; not an EE input.
#include <string>
struct Point3 { float x, y, z; };
static std::string calls;
static void* fake_block;
extern "C" {
void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return fake_block; }
void spell_ctor_fire_flares(void* self, int rank, void* creature, Point3 point, void* skill) asm("__15SpellFireFlaresiP8CreatureG6Point3P5Skill");
void spell_ctor_fire_flares(void* self, int rank, void* creature, Point3 point, void* skill) {
    calls += "ctor(" + std::to_string(self == fake_block) + "," + std::to_string(rank) + "," + std::to_string(reinterpret_cast<unsigned long>(creature)) + "," +
             std::to_string(point.x) + "," + std::to_string(point.y) + "," + std::to_string(point.z) + "," + std::to_string(reinterpret_cast<unsigned long>(skill)) + ");";
}
void* cast_fire_flares(void* skill, int rank, void* creature, Point3 point) asm("cast__15SkillFireFlaresiP8CreatureG6Point3s");
}
int main() {
    char block, skill, creature;
    for (int rank : {-3, 0, 1, 99}) {
        fake_block = &block; calls.clear();
        void* r = cast_fire_flares(&skill, rank, &creature, Point3{1.5f, -2.0f, 3.25f});
        const std::string expected = "alloc(120);ctor(1," + std::to_string(rank) + "," + std::to_string(reinterpret_cast<unsigned long>(&creature)) +
                                     ",1.500000,-2.000000,3.250000," + std::to_string(reinterpret_cast<unsigned long>(&skill)) + ");";
        if (r != &block || calls != expected) return 1;
        fake_block = nullptr; calls.clear();
        r = cast_fire_flares(&skill, rank, &creature, Point3{0, 0, 0});
        if (r != nullptr || calls != "alloc(120);") return 2;
    }
    return 0;
}
