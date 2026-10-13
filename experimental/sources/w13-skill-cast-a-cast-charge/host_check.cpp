// Host contract for cast__11SkillChargeiP8CreatureG6Point3s; not an EE input.
#include <cstring>
#include <string>
struct Point3 { float x, y, z; };
static std::string calls;
static void* fake_block;
extern "C" {
void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return fake_block; }
void spell_ctor_charge(void* self, int rank, void* creature, void* skill) asm("__11SpellChargeiP8CreatureP5Skill");
void spell_ctor_charge(void* self, int rank, void* creature, void* skill) {
    calls += "ctor(" + std::to_string(self == fake_block) + "," + std::to_string(rank) + "," + std::to_string(reinterpret_cast<unsigned long>(creature)) + "," +
             std::to_string(reinterpret_cast<unsigned long>(skill)) + ");";
}
void* cast_charge(void* skill, int rank, void* creature, Point3 point) asm("cast__11SkillChargeiP8CreatureG6Point3s");
}
int main() {
    char block, skill;
    alignas(8) unsigned char creature[1408];
    const int flag_words[] = {0, 0x40, 0x3f, -65, -1, 0x41};
    for (int rank : {-3, 0, 99})
        for (int flags : flag_words) {
            std::memset(creature, 0x5a, sizeof creature);
            std::memcpy(creature + 12, &flags, 4);
            unsigned char before[sizeof creature];
            std::memcpy(before, creature, sizeof creature);
            fake_block = &block; calls.clear();
            void* r = cast_charge(&skill, rank, creature, Point3{1, 2, 3});
            const std::string expected = "alloc(136);ctor(1," + std::to_string(rank) + "," + std::to_string(reinterpret_cast<unsigned long>(creature)) + "," +
                                         std::to_string(reinterpret_cast<unsigned long>(&skill)) + ");";
            if (r != &block || calls != expected) return 1;
            if (flags & 0x40) { int nine = 9; std::memcpy(before + 1400, &nine, 4); }
            if (std::memcmp(before, creature, sizeof creature) != 0) return 2;
            std::memset(creature, 0x5a, sizeof creature);
            std::memcpy(creature + 12, &flags, 4);
            std::memcpy(before, creature, sizeof creature);
            fake_block = nullptr; calls.clear();
            r = cast_charge(&skill, rank, creature, Point3{0, 0, 0});
            if (r != nullptr || calls != "alloc(136);" || std::memcmp(before, creature, sizeof creature) != 0) return 3;
        }
    return 0;
}
