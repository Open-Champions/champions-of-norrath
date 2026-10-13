// Host contract for canCast__15SkillHolyStrikeP8Creature; not an EE input.
extern "C" int canCast__15SkillHolyStrikeP8Creature(void*, void*);
int main() {
    char a, b;
    void* const skills[] = {nullptr, &a, reinterpret_cast<void*>(0x1000)};
    void* const creatures[] = {nullptr, &b, reinterpret_cast<void*>(0x2000)};
    for (void* s : skills)
        for (void* c : creatures)
            if (canCast__15SkillHolyStrikeP8Creature(s, c) != 1) return 1;
    return 0;
}
