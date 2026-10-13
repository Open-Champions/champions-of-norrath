#include <climits>
#include <cstdio>

struct Skill;
extern "C" int get_skill_point_cost(const Skill*, int) asm("getSkillPointCost__5Skilli");

int main() {
    const Skill* receiver = reinterpret_cast<const Skill*>(static_cast<unsigned long>(0x1234));
    const int values[] = {INT_MIN, -1024, -1, 0, 1, 2, 17, 1024, INT_MAX};
    for (int value : values) {
        if (get_skill_point_cost(receiver, value) != value) {
            std::fprintf(stderr, "argument not preserved: %d\n", value);
            return 1;
        }
    }
    return 0;
}
