// Observed canCast__15SkillHolyStrikeP8Creature at 0x002b2e48, 8 bytes in .text:
// jr ra; the delay slot loads 1 into v0. Neither argument is read, so the SkillHolyStrike check never refuses.
// Inferred only from that body: the function always allows the cast. Unknown: why the class overrides the base check.
extern "C" int canCast__15SkillHolyStrikeP8Creature(void*, void*) {
    return 1;
}
