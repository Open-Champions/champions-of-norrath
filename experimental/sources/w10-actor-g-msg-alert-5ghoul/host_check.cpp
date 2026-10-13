// Host contract for msg_alert__5GhoulifG6Point3P10GameObject; not an EE input.
#define W10_ACTOR_G_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10g_host_base;
static std::string calls;
static int setterArg; static void* setterObject;
static void setter(void* o, int v) { calls += "set;"; setterObject = o; setterArg = v; }
static int nodeFnCalls;
static unsigned char nodeA[4], nodeB[4];
static void* nodeFn() { ++nodeFnCalls; calls += "type;"; return nodeB; }
void* w10g_host_fn(unsigned w) { return w == 1 ? reinterpret_cast<void*>(nodeFn) : reinterpret_cast<void*>(setter); }
#include "msg_alert.cpp"
static bool eqResult;
extern "C" void* __tf5Ghoul() { calls += "tf;"; return nodeA; }
extern "C" int __eq__C9type_infoRC9type_info(const void* a, const void* b) {
    calls += "eq;"; if (a != nodeB || b != nodeA) calls += "BADARGS;"; return eqResult;
}
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static int bad;
static void check(bool ok, const char* what) { if (!ok) { std::printf("fail: %s (calls %s)\n", what, calls.c_str()); bad = 1; } }
int main() {
    w10g_host_base = arena;
    unsigned char* self = arena + 0x1000; unsigned char* other = arena + 0x2000;
    auto reset = [&](unsigned short state) {
        std::memset(arena, 0, sizeof arena); calls.clear();
        unsigned tab = 0x200, otab = 0x300;
        std::memcpy(self + 72, &tab, 4); std::memcpy(other + 72, &otab, 4);
        unsigned one = 1, two = 2; std::memcpy(arena + otab + 4, &one, 4);       // type function word 1
        short adj = 24; std::memcpy(arena + tab + 184, &adj, 2); std::memcpy(arena + tab + 188, &two, 4);
        std::memcpy(self + 572, &state, 2);
        for (int i = 272; i < 284; ++i) self[i] = 0xee;
        eqResult = true; setterObject = nullptr; setterArg = 0;
    };
    Point3 p{1.5f, 2.5f, -3.5f};
    auto copied = [&]() { Point3 q; std::memcpy(&q, self + 272, 12); return q.x == 1.5f && q.y == 2.5f && q.z == -3.5f; };
    reset(0); msg_alert__5GhoulifG6Point3P10GameObject(self, 0, 0.0f, p, other);
    check(calls == "type;tf;eq;set;" && setterObject == self + 24 && setterArg == 13 && copied(), "match with state 0: set 13, copy position");
    reset(1); msg_alert__5GhoulifG6Point3P10GameObject(self, 0, 0.0f, p, other);
    check(calls == "type;tf;eq;set;" && copied(), "state 1 still accepted");
    reset(2); msg_alert__5GhoulifG6Point3P10GameObject(self, 0, 0.0f, p, other);
    check(calls == "type;tf;eq;" && !copied(), "state 2 rejected after the type test");
    reset(0xffff); msg_alert__5GhoulifG6Point3P10GameObject(self, 0, 0.0f, p, other);
    check(calls == "type;tf;eq;" && !copied(), "halfword compared unsigned");
    reset(0); eqResult = false; msg_alert__5GhoulifG6Point3P10GameObject(self, 0, 0.0f, p, other);
    check(calls == "type;tf;eq;" && !copied(), "different type does nothing");
    reset(0); msg_alert__5GhoulifG6Point3P10GameObject(self, 1, 0.0f, p, other);
    check(calls.empty() && !copied(), "nonzero kind returns at once");
    reset(0); msg_alert__5GhoulifG6Point3P10GameObject(self, 0, 0.0f, p, nullptr);
    check(calls.empty() && !copied(), "null other returns at once");
    if (!bad) std::puts("ghoul msg_alert ok");
    return bad;
}
