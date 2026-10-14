// Host contract for leaveCommander__3Orc; not an EE input. Includes the source.
#define W11_ACTOR_B_HOST 1
#include "leave_commander.cpp"
#include <cstring>
#include <initializer_list>
static unsigned char arena[0x4000];
unsigned char* w11_actor_b_host_base = arena;
unsigned w11_actor_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static int destroys = 0;
static void* destroyed = nullptr;
extern "C" void listDestroy__FP8ListHead(void* head) { ++destroys; destroyed = head; }
int main() {
    const unsigned self = 0x100, node = 0x1000, cmd = 0x800;
    // Zero head word: nothing happens.
    std::memset(arena, 0x22, sizeof arena);
    std::memset(arena + self + 1128, 0, 4);
    unsigned char before[sizeof arena]; std::memcpy(before, arena, sizeof arena);
    leaveCommander__3Orc(arena + self);
    if (destroys != 0 || std::memcmp(before, arena, sizeof arena)) return 1;
    // Non-zero: decrement a positive count, never below zero or when negative, destroy, clear.
    for (int start : {0, 1, 2, 100, 127, 128, 200, 255}) {
        std::memset(arena, 0x22, sizeof arena);
        unsigned v = node; std::memcpy(arena + self + 1128, &v, 4);
        unsigned c = cmd; std::memcpy(arena + node + 12, &c, 4);
        arena[cmd + 1132] = static_cast<unsigned char>(start);
        std::memcpy(before, arena, sizeof arena);
        destroys = 0;
        leaveCommander__3Orc(arena + self);
        if (destroys != 1 || destroyed != arena + self + 1128) return 2;
        unsigned head; std::memcpy(&head, arena + self + 1128, 4);
        if (head != 0) return 3;
        int expect = (static_cast<signed char>(start) > 0) ? start - 1 : start;
        if (arena[cmd + 1132] != static_cast<unsigned char>(expect)) return 4;
        for (unsigned i = 0; i < sizeof arena; ++i) {
            bool allowed = (i >= self + 1128 && i < self + 1132) || i == cmd + 1132;
            if (!allowed && before[i] != arena[i]) return 5;
        }
    }
    return 0;
}
