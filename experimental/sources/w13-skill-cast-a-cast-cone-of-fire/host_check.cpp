// Host contract for cast__15SkillConeOfFireiP8CreatureG6Point3s; not an EE input.
#include <string>
static std::string calls;
static void* fake_block;
extern "C" {
void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return fake_block; }
void spell_ctor_cone_of_fire(void* self, int rank, void* creature, void* skill) asm("__15SpellConeOfFireiP8CreatureP5Skill");
void spell_ctor_cone_of_fire(void* self, int rank, void* creature, void* skill) {
    calls += "ctor(" + std::to_string(self == fake_block) + "," + std::to_string(rank) + "," + std::to_string(reinterpret_cast<unsigned long>(creature)) + "," +
             std::to_string(reinterpret_cast<unsigned long>(skill)) + ");";
}
struct Point3 { float x, y, z; };
void* cast_cone_of_fire(void* skill, int rank, void* creature, Point3 point) asm("cast__15SkillConeOfFireiP8CreatureG6Point3s");
}
int main() {
    char block, skill, creature;
    const int ranks[] = {-3, 0, 1, 99};
    for (int rank : ranks) {
        fake_block = &block; calls.clear();
        void* r = cast_cone_of_fire(&skill, rank, &creature, Point3{1.0f, 2.0f, 3.0f});
        const std::string expected = "alloc(88);ctor(1," + std::to_string(rank) + "," + std::to_string(reinterpret_cast<unsigned long>(&creature)) + "," +
                                     std::to_string(reinterpret_cast<unsigned long>(&skill)) + ");";
        if (r != &block || calls != expected) return 1;
        fake_block = nullptr; calls.clear();
        r = cast_cone_of_fire(&skill, rank, &creature, Point3{0, 0, 0});
        if (r != nullptr || calls != "alloc(88);") return 2;
    }
    return 0;
}
