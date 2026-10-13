#include "can_cast.cpp"
#include <cstdio>

int main() {
    void* values[] = {nullptr, reinterpret_cast<void*>(1), reinterpret_cast<void*>(0x12345678)};
    for (void* skill : values) {
        for (void* creature : values) {
            if (canCast__5SkillP8Creature(skill, creature) != 1) return 1;
        }
    }
    std::puts("ok canCast returns one for all tested pointer pairs");
    return 0;
}
