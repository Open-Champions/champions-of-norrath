// Host contract for msg_collision__7CritterP10GameObject; not an EE input.
#define W10_ACTOR_D_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_d_host_base;
typedef void (*HostFn)(void*, int);
HostFn w10_actor_d_host_functions[4];
#include "msg_collision.cpp"
static std::string calls; static unsigned char* objectUnderTest;
void* listOnewayLink__FP8ListHeadT0Pv(ListHead* a, ListHead* b, void* v) {
    calls += "link(" + std::to_string(reinterpret_cast<unsigned char*>(a) - objectUnderTest) + "," + std::to_string(b == v) + "," + std::to_string(v != nullptr) + ");"; return nullptr;
}
static void setState(void* s, int v) { calls += "slot(" + std::to_string(static_cast<unsigned char*>(s) - objectUnderTest) + "," + std::to_string(v) + ");"; }
int main() {
    alignas(16) static unsigned char arena[8192], other[64];
    w10_actor_d_host_base = arena; w10_actor_d_host_functions[1] = setState;
    unsigned char* object = arena + 2048; objectUnderTest = object;
    int bad = 0;
    struct Case { short state; int word1064; bool hasOther; std::string want; } cases[] = {
        {12, 5, true, ""}, {13, 0, true, "slot(0,14);"}, {13, 5, false, "slot(0,14);"},
        {0, 5, true, "link(576,1,1);slot(0,7);"}, {1, 5, true, "link(576,1,1);slot(0,7);"},
        {1, 5, false, "slot(0,7);"}, {0, 0, true, ""}, {2, 5, true, ""}, {11, 5, true, ""}, {14, 5, true, ""},
        {-1, 5, true, ""}, {-2, 5, true, ""}, {static_cast<short>(0x8000), 5, true, ""},
    };
    for (Case& c : cases) {
        std::memset(arena, 0, sizeof arena);
        unsigned table = 4096; std::memcpy(object + 72, &table, 4);
        short adj = 0; unsigned fn = 1; std::memcpy(arena + table + 184, &adj, 2); std::memcpy(arena + table + 188, &fn, 4);
        std::memcpy(object + 572, &c.state, 2); std::memcpy(object + 1064, &c.word1064, 4);
        calls.clear();
        msg_collision__7CritterP10GameObject(object, c.hasOther ? static_cast<void*>(other) : nullptr);
        if (calls != c.want) { std::printf("fail: state %d: %s\n", c.state, calls.c_str()); bad = 1; }
    }
    return bad;
}
