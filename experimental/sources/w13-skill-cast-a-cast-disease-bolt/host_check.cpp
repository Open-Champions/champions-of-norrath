// Host contract for cast__16SkillDiseaseBoltiP8CreatureG6Point3s; not an EE input.
#include <cstring>
#include <string>
struct Point3 { float x, y, z; };
static std::string calls;
static void* fake_block;
extern "C" {
void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return fake_block; }
void projectile_ctor_disease_bolt(void* self, short angle, int rank, Point3 point, void* creature, void* skill) asm("__11DiseaseBoltsiG6Point3P10GameObjectP5Skill");
void projectile_ctor_disease_bolt(void* self, short angle, int rank, Point3 point, void* creature, void* skill) {
    calls += "ctor(" + std::to_string(self == fake_block) + "," + std::to_string(angle) + "," + std::to_string(rank) + "," + std::to_string(point.x) + "," +
             std::to_string(point.y) + "," + std::to_string(point.z) + "," + std::to_string(reinterpret_cast<unsigned long>(creature)) + "," +
             std::to_string(reinterpret_cast<unsigned long>(skill)) + ");";
}
int cast_disease_bolt(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__16SkillDiseaseBoltiP8CreatureG6Point3s");
}
int main() {
    char block, skill;
    alignas(8) unsigned char creature[256];
    const struct { int offset; short value; } stores[] = {{152, -100}, {148, -100}, {150, -100}, {154, 0}, };
    for (int rank : {-3, 0, 99})
        for (short angle : {static_cast<short>(-32768), static_cast<short>(-1), static_cast<short>(0), static_cast<short>(7), static_cast<short>(32767)}) {
            unsigned char before[sizeof creature];
            std::memset(creature, 0x5a, sizeof creature);
            std::memcpy(before, creature, sizeof creature);
            for (const auto& s : stores) std::memcpy(before + s.offset, &s.value, 2);
            fake_block = &block; calls.clear();
            if (cast_disease_bolt(&skill, rank, creature, Point3{1.5f, -2.0f, 3.25f}, angle) != 0) return 1;
            const std::string expected = "alloc(124);ctor(1," + std::to_string(angle) + "," + std::to_string(rank) + ",1.500000,-2.000000,3.250000," +
                                         std::to_string(reinterpret_cast<unsigned long>(creature)) + "," + std::to_string(reinterpret_cast<unsigned long>(&skill)) + ");";
            if (calls != expected || std::memcmp(before, creature, sizeof creature) != 0) return 2;
            std::memset(creature, 0x5a, sizeof creature);
            fake_block = nullptr; calls.clear();
            if (cast_disease_bolt(&skill, rank, creature, Point3{0, 0, 0}, angle) != 0 || calls != "alloc(124);" || std::memcmp(before, creature, sizeof creature) != 0) return 3;
        }
    return 0;
}
