// Host contract for cast__17SkillDiseaseTrailiP8CreatureG6Point3s; not an EE input.
#include <cstring>
#include <string>
static std::string calls;
static void* fake_block;
struct Point3 { float x, y, z; };
extern "C" {
void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return fake_block; }
void spell_ctor_disease_trail(void* self, Point3 point, int rank, void* creature, void* skill) asm("__17SpellDiseaseTrailG6Point3iP8CreatureP5Skill");
void spell_ctor_disease_trail(void* self, Point3 point, int rank, void* creature, void* skill) {
    calls += "ctor(" + std::to_string(self == fake_block) + "," + std::to_string(point.x) + "," + std::to_string(point.y) + "," + std::to_string(point.z) + "," +
             std::to_string(rank) + "," + std::to_string(reinterpret_cast<unsigned long>(creature)) + "," + std::to_string(reinterpret_cast<unsigned long>(skill)) + ");";
}
void* cast_disease_trail(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__17SkillDiseaseTrailiP8CreatureG6Point3s");
}
int main() {
    char block, skill;
    alignas(8) unsigned char creature[256];
    const int ranks[] = {-3, 0, 1, 99};
    for (int rank : ranks) {
        for (int alloc_ok = 0; alloc_ok < 2; ++alloc_ok) {
            std::memset(creature, 0x5a, sizeof creature);
            fake_block = alloc_ok ? &block : nullptr; calls.clear();
            void* r = cast_disease_trail(&skill, rank, creature, Point3{1.5f, -2.0f, 3.25f}, 7);
            std::string expected = "alloc(48);";
            if (alloc_ok) expected += "ctor(1," + std::to_string(1.5f) + "," + std::to_string(-2.0f) + "," + std::to_string(3.25f) + "," + std::to_string(rank) + "," +
                                      std::to_string(reinterpret_cast<unsigned long>(creature)) + "," + std::to_string(reinterpret_cast<unsigned long>(&skill)) + ");";
            if (r != fake_block || calls != expected) return 1;
            short group[4];
            std::memcpy(group, creature + 148, 8);
            if (group[0] != -100 || group[1] != -100 || group[2] != -100 || group[3] != 0) return 2;
            for (int i = 0; i < 256; ++i)
                if ((i < 148 || i >= 156) && creature[i] != 0x5a) return 3;
        }
    }
    return 0;
}
