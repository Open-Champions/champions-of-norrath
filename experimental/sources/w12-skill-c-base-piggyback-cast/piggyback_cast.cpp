// Observed in SLUS-20565-sha256-9a60f13c5c31 at 0x003131e8 (40 bytes):
// For valid arguments, return zero and perform no receiver/creature state writes.
// Point3 is passed by value according to the retained method signature.
struct Skill;
struct Creature;
struct Point3 { float x, y, z; };
extern "C" int piggyback_cast(const Skill*, int, Creature*, Point3) asm("piggyBackCast__5SkilliP8CreatureG6Point3s");
extern "C" int piggyback_cast(const Skill*, int, Creature*, Point3) {
    return 0;
}
