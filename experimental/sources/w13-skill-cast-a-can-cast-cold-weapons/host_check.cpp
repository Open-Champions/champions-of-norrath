// Host contract for canCast__16SkillColdWeaponsP8Creature; not an EE input.
// The type-info function and the lookup are stubs that log their calls.
#include <string>
static std::string calls;
static int node;
static void* running;
extern "C" {
const void* spell_type_info() asm("__tf16SpellColdWeapons");
const void* spell_type_info() { calls += "tf;"; return &node; }
void* checkIfSpellIsRunning__8CreatureRC9type_info(void* creature, const void* typeinfo) {
    calls += "check(" + std::to_string(reinterpret_cast<unsigned long>(creature)) + "," + (typeinfo == &node ? "node" : "?") + ");";
    return running;
}
int can_cast_cold_weapons(void* skill, void* creature) asm("canCast__16SkillColdWeaponsP8Creature");
}
int main() {
    char other;
    void* const creatures[] = {nullptr, &other, reinterpret_cast<void*>(0x1234)};
    for (void* c : creatures) {
        const std::string expected = "tf;check(" + std::to_string(reinterpret_cast<unsigned long>(c)) + ",node);";
        calls.clear(); running = nullptr;
        if (can_cast_cold_weapons(&other, c) != 1 || calls != expected) return 1;
        calls.clear(); running = &other;
        if (can_cast_cold_weapons(&other, c) != 0 || calls != expected) return 2;
        calls.clear(); running = reinterpret_cast<void*>(1);
        if (can_cast_cold_weapons(nullptr, c) != 0 || calls != expected) return 3;
    }
    return 0;
}
