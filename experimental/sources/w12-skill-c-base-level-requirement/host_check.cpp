#include <climits>
#include <cstdio>
struct Skill;
extern "C" int get_level_requirement(const Skill*, int) asm("getLevelRequirement__5Skilli");
int main() {
    const Skill* receiver = reinterpret_cast<const Skill*>(static_cast<unsigned long>(0x1234));
    const int values[] = {INT_MIN, -1024, -1, 0, 1, 1024, INT_MAX};
    for (int value : values) {
        if (get_level_requirement(receiver, value) != 0) {
            std::fprintf(stderr, "nonzero requirement for %d\n", value);
            return 1;
        }
    }
    return 0;
}
