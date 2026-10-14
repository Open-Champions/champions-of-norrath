// Host contract for msg_moveCollision__6CthuluPP10GameObject; not an EE input.
#define W10_ACTOR_D_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_d_host_base;
typedef void (*HostFn)(void*, int);
HostFn w10_actor_d_host_functions[4];
#include "move_collision.cpp"
static std::string calls; static unsigned char* objectUnderTest;
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* t, void* a, float f1, float f2, int c, int d, int e, int f, void* desc, bool g) {
    calls += "atk(" + std::to_string(static_cast<unsigned char*>(t) - w10_actor_d_host_base) + "," + std::to_string(a == objectUnderTest) + "," + std::to_string(f1) + "," + std::to_string(f2) + "," +
             std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(e) + "," + std::to_string(f) + "," + std::to_string(desc == nullptr) + "," + std::to_string(g) + ");";
}
void msg_moveCollision__8CreaturePP10GameObject(void* s, void** l) { calls += "base(" + std::to_string(s == objectUnderTest) + "," + std::to_string(l != nullptr) + ");"; }
static void setState(void* s, int v) { calls += "slot(" + std::to_string(static_cast<unsigned char*>(s) - objectUnderTest) + "," + std::to_string(v) + ");"; }
int main() {
    alignas(16) static unsigned char arena[8192];
    w10_actor_d_host_base = arena;
    w10_actor_d_host_functions[2] = setState;
    unsigned char* object = arena + 2048; objectUnderTest = object;
    unsigned table = 4096, list = 1024;
    int bad = 0;
    struct Case { short state; float f; unsigned a_flags, b_flags; int listLength; std::string want; } cases[] = {
        {15, 0.75f, 0x40, 0x00, 2, "atk(5000,1,-7.000000,9.000000,100000,0,0,262209,1,0);slot(0,3);"},
        {15, 0.75f, 0x41, 0xffffff40, 2, "atk(5000,1,-7.000000,9.000000,100000,0,0,262209,1,0);atk(5100,1,-7.000000,9.000000,100000,0,0,262209,1,0);slot(0,3);"},
        {15, 0.75f, 0x00, 0x3f, 2, "slot(0,3);"},
        {15, 0.75f, 0x40, 0x40, 0, "slot(0,3);"},
        {15, 0.5f, 0x40, 0x40, 2, "base(1,1);"},
        {15, 0.25f, 0x40, 0x40, 2, "base(1,1);"},
        {14, 0.75f, 0x40, 0x40, 2, "base(1,1);"},
        {-15, 0.75f, 0x40, 0x40, 2, "base(1,1);"},
    };
    for (Case& c : cases) {
        std::memset(arena, 0, sizeof arena);
        std::memcpy(object + 572, &c.state, 2); std::memcpy(object + 224, &c.f, 4);
        short s200 = -7, s202 = 9; std::memcpy(object + 200, &s200, 2); std::memcpy(object + 202, &s202, 2);
        std::memcpy(object + 72, &table, 4);
        short adj = 0; unsigned fn = 2; std::memcpy(arena + table + 184, &adj, 2); std::memcpy(arena + table + 188, &fn, 4);
        unsigned a = 5000, b = 5100;
        std::memcpy(arena + a + 12, &c.a_flags, 4); std::memcpy(arena + b + 12, &c.b_flags, 4);
        unsigned* words = reinterpret_cast<unsigned*>(arena + list);
        words[0] = c.listLength > 0 ? a : 0; words[1] = c.listLength > 1 ? b : 0; words[2] = 0;
        calls.clear();
        msg_moveCollision__6CthuluPP10GameObject(object, words);
        if (calls != c.want) { std::printf("fail: state %d f %f: %s\n", c.state, c.f, calls.c_str()); bad = 1; }
    }
    return bad;
}
