// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x003131c0 (40 bytes):
// For valid arguments, returns zero and performs no receiver/creature state writes.
// The Point3 value is passed by value per the retained method signature.
struct Skill;
struct Creature;
struct Point3 { float x, y, z; };
extern "C" int cast_skill(const Skill*, int, Creature*, Point3) asm("cast__5SkilliP8CreatureG6Point3s");
extern "C" int cast_skill(const Skill*, int, Creature*, Point3) {
    return 0;
}
