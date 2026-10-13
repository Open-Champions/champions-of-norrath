// Host contract for canCast__17SkillShockOfFrostP8Creature; not an EE input.
extern "C" int can_cast_shock_of_frost(void* skill, void* creature) asm("canCast__17SkillShockOfFrostP8Creature");
int main() {
    char a, b;
    void* const values[] = {nullptr, &a, &b, reinterpret_cast<void*>(0x1234)};
    for (void* s : values)
        for (void* c : values)
            if (can_cast_shock_of_frost(s, c) != 1) return 1;
    return 0;
}
