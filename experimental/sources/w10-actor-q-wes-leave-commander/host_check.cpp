// Host check only; not an EE build input.
#define W10_ACTOR_Q_HOST 1
#include "leave_commander.cpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_q_host_base;
static std::string calls; static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s (%s)\n", m, calls.c_str()); ++failures; } }
extern "C" void listDestroy__FP8ListHead(void* head) {
    const long off = static_cast<unsigned char*>(head) - w10_actor_q_host_base;
    calls += "destroy(" + std::to_string(off) + "," + std::to_string(*reinterpret_cast<unsigned*>(head) != 0) + ");";
}
int main() {
    static unsigned char base[8192];
    w10_actor_q_host_base = base;
    unsigned char* self = base + 1024; unsigned char* node = base + 6000; unsigned char* other = base + 3000;
    *reinterpret_cast<unsigned*>(self + 960) = 0;
    leaveCommander__14WoodElfSoldier(self);
    expect(calls.empty(), "zero word does nothing");
    *reinterpret_cast<unsigned*>(self + 960) = 6000;
    *reinterpret_cast<unsigned*>(node + 12) = 3000;
    *reinterpret_cast<unsigned*>(other + 960) = 77;
    leaveCommander__14WoodElfSoldier(self);
    expect(calls == "destroy(3960,1);destroy(1984,1);", "other head first, then own, each destroyed before clearing");
    expect(*reinterpret_cast<unsigned*>(other + 960) == 0 && *reinterpret_cast<unsigned*>(self + 960) == 0, "both words cleared");
    if (!failures) std::puts("leaveCommander ok");
    return failures ? 1 : 0;
}
