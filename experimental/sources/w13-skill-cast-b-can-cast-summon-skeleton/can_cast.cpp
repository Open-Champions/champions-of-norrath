// Observed canCast__19SkillSummonSkeletonP8Creature at 0x002a91a8, 84 bytes in .text.
// The skill argument is not read. With p = the unsigned word at creature+960:
//  1. When bit 0x40 of the word at creature+12 is clear the function returns zero.
//  2. When p is four or more it returns zero.
//  3. Otherwise it calls objectFindObject__FPc with the p-th entry of the four-entry name table skelightName and
//     returns zero when that returns a non-null object, one when it returns null.
// Inferred: the skill is available to a player-controlled creature only while the object named for that player's
// index (the table's entries are per-player names) does not already exist, so one such object per player is allowed.
// Unknown: the names themselves (they are not copied here), the bit, and what the found object is.
extern "C" {
void* objectFindObject__FPc(char* name);
extern char* skelightName[4];
int can_cast_summon_skeleton(void* skill, void* creature) asm("canCast__19SkillSummonSkeletonP8Creature");
}
extern "C" int can_cast_summon_skeleton(void*, void* creature) {
    unsigned char* c = static_cast<unsigned char*>(creature);
    if (!(*reinterpret_cast<unsigned*>(c + 12) & 0x40)) return 0;
    const unsigned index = *reinterpret_cast<unsigned*>(c + 960);
    if (index >= 4) return 0;
    return objectFindObject__FPc(skelightName[index]) == nullptr;
}
