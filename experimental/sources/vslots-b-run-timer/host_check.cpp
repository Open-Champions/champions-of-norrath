#define NORRATH_VSLOTS_B_HOST 1
#include "run_timer.cpp"
#include <cstdio>
#include <cstring>
static int actions = 0;
static int removals = 0;
static int seen_value = 0;
static void* seen_self = nullptr;
extern "C" void objectDoMyActions__FP10GameObjecti(void* self, int value) {
    ++actions;
    seen_self = self;
    seen_value = value;
    vslots_b::write_word(vslots_b::address(self) + 80u, 0x55u);
}
extern "C" void objectRemoveFromRunList__FP10GameObject(void* self) {
    ++removals;
    if (self != seen_self) removals = -100;
}
int main() {
    unsigned char object[88];
    std::memset(object, 0x17, sizeof object);
    vslots_b::write_word(vslots_b::address(object) + 76u, 2u);
    vslots_b::write_word(vslots_b::address(object) + 80u, 9u);
    msg_run__5Timer(object);
    if (vslots_b::read_word(vslots_b::address(object) + 76u) != 1u || actions != 0) return 1;
    vslots_b::write_word(vslots_b::address(object) + 76u, 1u);
    msg_run__5Timer(object);
    if (actions != 1 || removals != 1 || seen_value != 9) return 1;
    if (vslots_b::read_word(vslots_b::address(object) + 76u) != 0u) return 1;
    vslots_b::write_word(vslots_b::address(object) + 76u, 0x80000000u);
    int before = actions;
    msg_run__5Timer(object);
    if (actions != before) return 1;
    if (vslots_b::read_word(vslots_b::address(object) + 76u) != 0x7fffffffu) return 1;
    return 0;
}
