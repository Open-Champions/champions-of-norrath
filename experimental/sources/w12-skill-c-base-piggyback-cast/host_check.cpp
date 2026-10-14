struct Skill;
struct Creature;
struct Point3 { float x, y, z; };
extern "C" int piggyback_cast(const Skill*, int, Creature*, Point3) asm("piggyBackCast__5SkilliP8CreatureG6Point3s");
int main() {
    const Skill* skill = reinterpret_cast<const Skill*>(static_cast<unsigned long>(0x1000));
    Creature* creature = reinterpret_cast<Creature*>(static_cast<unsigned long>(0x2000));
    const Point3 point{1.5f, -2.0f, 9.25f};
    const int ranks[] = {-1, 0, 1, 99};
    for (int rank : ranks) if (piggyback_cast(skill, rank, creature, point) != 0) return 1;
    return 0;
}
