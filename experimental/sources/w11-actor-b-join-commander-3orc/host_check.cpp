// Host contract for joinCommander__3Orc; not an EE input. Includes the source.
#define W11_ACTOR_B_HOST 1
#include "join_commander.cpp"
#include <cstring>
#include <initializer_list>
static unsigned char arena[0x4000];
unsigned char* w11_actor_b_host_base = arena;
unsigned w11_actor_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static void* found = nullptr;
static int links = 0;
static void *la, *lb, *lc;
static unsigned linkNode = 0;
extern "C" void* findCommander__3Orc(void*) { return found; }
extern "C" void listOnewayLink__FP8ListHeadT0Pv(void* head, void* other, void* data) {
    ++links; la = head; lb = other; lc = data;
    // The real routine puts a node at the head word whose word 12 is the data argument.
    unsigned node = linkNode;
    unsigned off = static_cast<unsigned>(static_cast<unsigned char*>(data) - arena);
    std::memcpy(arena + node + 12, &off, 4);
    std::memcpy(head, &node, 4);
}
int main() {
    const unsigned self = 0x100, cmd = 0x800;
    std::memset(arena, 0x11, sizeof arena);
    linkNode = 0x1000;
    // No commander: nothing is linked, nothing written, result zero.
    found = nullptr; links = 0;
    unsigned char before[sizeof arena]; std::memcpy(before, arena, sizeof arena);
    if (joinCommander__3Orc(arena + self) != 0 || links != 0 || std::memcmp(before, arena, sizeof arena)) return 1;
    // Commander found: link(head, C, C), count byte incremented, result 1.
    found = arena + cmd;
    for (int start : {0, 1, 5, 127, 255}) {
        arena[cmd + 1132] = static_cast<unsigned char>(start); links = 0;
        std::memcpy(before, arena, sizeof arena);
        if (joinCommander__3Orc(arena + self) != 1) return 2;
        if (links != 1 || la != arena + self + 1128 || lb != arena + cmd || lc != arena + cmd) return 3;
        if (arena[cmd + 1132] != static_cast<unsigned char>(start + 1)) return 4;
        for (unsigned i = 0; i < sizeof arena; ++i) {
            bool expected = (i >= self + 1128 && i < self + 1132) || (i >= linkNode + 12 && i < linkNode + 16) || i == cmd + 1132;
            if (!expected && before[i] != arena[i]) return 5;
        }
    }
    return 0;
}
