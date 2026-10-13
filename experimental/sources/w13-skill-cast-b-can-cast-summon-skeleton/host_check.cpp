// Host contract for canCast__19SkillSummonSkeletonP8Creature; not an EE input.
#include <cstring>
#include <string>
static std::string calls;
static char names[4][8] = {"n0", "n1", "n2", "n3"};
extern "C" { char* skelightName[4] = {names[0], names[1], names[2], names[3]}; }
static char found_object;
static void* found;
extern "C" void* objectFindObject__FPc(char* name) { calls += std::string("find(") + name + ");"; return found; }
#include "can_cast.cpp"
int main() {
    unsigned char creature[1024];
    char skill;
    const unsigned indexes[] = {0, 1, 2, 3, 4, 5, 0xffffffffu, 0x80000000u};
    for (unsigned index : indexes)
        for (int flag_set = 0; flag_set < 2; ++flag_set)
            for (int exists = 0; exists < 2; ++exists) {
                std::memset(creature, 0, sizeof creature);
                const unsigned flags = flag_set ? 0x40 : 0x00;
                std::memcpy(creature + 12, &flags, 4);
                std::memcpy(creature + 960, &index, 4);
                found = exists ? &found_object : nullptr;
                calls.clear();
                const int r = can_cast_summon_skeleton(&skill, creature);
                const bool queried = flag_set && index < 4;
                const int want = queried ? (exists ? 0 : 1) : 0;
                const std::string expected = queried ? "find(n" + std::to_string(index) + ");" : "";
                if (r != want || calls != expected) return 1;
            }
    return 0;
}
