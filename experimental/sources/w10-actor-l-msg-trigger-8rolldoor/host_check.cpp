// Host contract for msg_trigger__8RollDoori; not an EE input. The table entry call is logged:
// the adjusted object, the argument count and the argument must match, the entry's result is
// returned, and the incoming integer must not matter.
#define W10_ACTOR_L_HOST 1
#include <cstring>
#include <initializer_list>
#include <string>
unsigned char* w10_actor_l_host_base;
static std::string calls;
unsigned w10_actor_l_host_call(unsigned fn, void* adjusted, int n, unsigned long a1, unsigned long) {
    calls += std::to_string(fn) + "@" + std::to_string(static_cast<unsigned char*>(adjusted) - w10_actor_l_host_base) + "/" + std::to_string(n) + "/" + std::to_string(a1) + ";";
    return 0x1234u;
}
#include "msg_trigger.cpp"
alignas(16) static unsigned char arena[0x2000];
int main() {
    w10_actor_l_host_base = arena;
    int bad = 0;
    for (int arg : {0, 1, -1, 99}) for (short adj : {short(0), short(16), short(-16)}) {
        std::memset(arena, 0, sizeof arena); calls.clear();
        unsigned t = 0x1000; std::memcpy(arena + 0x100 + 72, &t, 4);
        std::memcpy(arena + t + 112, &adj, 2);
        unsigned fn = 0x4242; std::memcpy(arena + t + 112 + 4, &fn, 4);
        unsigned decoy = 0x9999; std::memcpy(arena + t + 104 + 4, &decoy, 4); std::memcpy(arena + t + 120 + 4, &decoy, 4);
        if (msg_trigger__8RollDoori(arena + 0x100, arg) != 0x1234) bad = 1;
        if (calls != "16962@" + std::to_string(0x100 + adj) + "/1/0;") bad = 1;
    }
    return bad;
}
