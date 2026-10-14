// Host contract for commanderValid__3Orc; not an EE input. Words that hold addresses are offsets from a
// host base. Returns non-zero when wrong.
#define W11_ACTOR_A_HOST 1
unsigned char* w11_actor_a_host_base;
#include "commander_valid.cpp"
#include <cstring>
#include <initializer_list>
int main() {
    alignas(16) static unsigned char arena[0x2000];
    w11_actor_a_host_base = arena;
    int bad = 0;
    unsigned char* obj = arena + 0x100;
    unsigned char* record = arena + 0x400;
    unsigned char* target = arena + 0x800;
    unsigned objw = 0x100, recw = 0x400, tgtw = 0x800;
    (void)objw;
    std::memset(arena, 0, sizeof arena);
    if (commanderValid__3Orc(obj) != 0) bad = 1;           // no commander
    std::memcpy(obj + 1128, &recw, 4);
    std::memcpy(record + 12, &tgtw, 4);
    for (unsigned flags : {0u, 8u, 0xfffffff7u, 0xffffffffu, 7u, 0x10u}) {
        std::memcpy(target + 12, &flags, 4);
        if (commanderValid__3Orc(obj) != ((flags & 8) == 0 ? 1 : 0)) bad = 1;
    }
    return bad;
}
