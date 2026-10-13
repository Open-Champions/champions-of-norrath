// Observed canCast__5SkillP8Creature at 0x003131b8, 8 bytes in .text:
// jr ra; delay slot loads 1 into v0. The creature argument is not read.
// The return is always one; no other behavior is inferred.
extern "C" int canCast__5SkillP8Creature(void*, void*) {
    return 1;
}
