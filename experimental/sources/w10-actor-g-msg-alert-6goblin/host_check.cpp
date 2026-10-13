// Host contract for msg_alert__6GoblinifG6Point3P10GameObject; not an EE input.
#define W10_ACTOR_G_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10g_host_base;
static std::string calls; static void* setterObject; static int setterArg;
static void setter(void* o, int v) { calls += "set;"; setterObject = o; setterArg = v; }
static unsigned char nodeA[4], nodeB[4];
static void* nodeFn() { calls += "type;"; return nodeB; }
void* w10g_host_fn(unsigned w) { return w == 1 ? reinterpret_cast<void*>(nodeFn) : reinterpret_cast<void*>(setter); }
#include "msg_alert.cpp"
static bool eqResult; static int angle; static float angY, angX;
extern "C" void* __tf6Goblin() { calls += "tf;"; return nodeA; }
extern "C" int __eq__C9type_infoRC9type_info(const void* a, const void* b) { calls += "eq;"; if (a != nodeB || b != nodeA) calls += "BADARGS;"; return eqResult; }
extern "C" int iatan2__Fff(float y, float x) { calls += "atan;"; angY = y; angX = x; return angle; }
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static int bad;
static void check(bool ok, const char* what) { if (!ok) { std::printf("fail: %s (calls %s)\n", what, calls.c_str()); bad = 1; } }
static void setf(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }
int main() {
    w10g_host_base = arena;
    unsigned char* self = arena + 0x1000; unsigned char* other = arena + 0x2000;
    auto reset = [&](short state) {
        std::memset(arena, 0, sizeof arena); calls.clear();
        unsigned tab = 0x200, otab = 0x300, one = 1, two = 2; short adj = 24;
        std::memcpy(self + 72, &tab, 4); std::memcpy(other + 72, &otab, 4); std::memcpy(arena + otab + 4, &one, 4);
        std::memcpy(arena + tab + 184, &adj, 2); std::memcpy(arena + tab + 188, &two, 4);
        std::memcpy(self + 572, &state, 2);
        for (int i = 272; i < 284; ++i) self[i] = 0xee;
        eqResult = true; setterObject = nullptr; setterArg = 0; angle = 0;
    };
    Point3 p{1.5f, 2.5f, -3.5f};
    auto copied = [&]() { Point3 q; std::memcpy(&q, self + 272, 12); return q.x == 1.5f && q.y == 2.5f && q.z == -3.5f; };
    // kind 0
    reset(0); msg_alert__6GoblinifG6Point3P10GameObject(self, 0, 0, p, other);
    check(calls == "type;tf;eq;set;" && setterObject == self + 24 && setterArg == 2 && copied(), "kind 0: set 2 and copy position");
    reset(2); msg_alert__6GoblinifG6Point3P10GameObject(self, 0, 0, p, other); check(calls == "type;tf;eq;" && !copied(), "kind 0: state 2 rejected");
    reset(0); eqResult = false; msg_alert__6GoblinifG6Point3P10GameObject(self, 0, 0, p, other); check(calls == "type;tf;eq;" && !copied(), "kind 0: other type differs");
    reset(0); msg_alert__6GoblinifG6Point3P10GameObject(self, 0, 0, p, nullptr); check(calls.empty(), "kind 0: null other");
    reset(0); self[570] = 1; msg_alert__6GoblinifG6Point3P10GameObject(self, 0, 0, p, other); check(calls.empty() && !copied(), "byte +570 blocks");
    reset(0); self[570] = 0x80; msg_alert__6GoblinifG6Point3P10GameObject(self, 0, 0, p, other); check(calls.empty(), "byte +570 tested for nonzero");
    // kind 2
    auto k2 = [&](short state, int diffFrom56) {
        reset(state); unsigned w = 5; std::memcpy(self + 992, &w, 4);
        short h56 = 1000; std::memcpy(self + 56, &h56, 2); angle = 1000 + diffFrom56;
        setf(self, 16, 1.0f); setf(self, 20, 2.0f); setf(other, 16, 11.0f); setf(other, 20, 7.0f);
        msg_alert__6GoblinifG6Point3P10GameObject(self, 2, 0, p, other);
    };
    k2(3, 0); check(calls == "atan;set;" && angY == 5.0f && angX == 10.0f && setterArg == 15 && setterObject == self + 24, "kind 2: angle arguments (dy from +20, dx from +16), set 15");
    k2(5, 9999); check(calls == "atan;set;", "kind 2: +9999 accepted");
    k2(5, -9999); check(calls == "atan;set;", "kind 2: -9999 accepted");
    k2(5, 10000); check(calls == "atan;", "kind 2: +10000 rejected");
    k2(5, -10000); check(calls == "atan;", "kind 2: -10000 rejected");
    k2(4, 0); check(calls.empty(), "kind 2: only states 3 and 5");
    k2(8, 0); check(calls.empty(), "kind 2: state 8 rejected");
    reset(3); msg_alert__6GoblinifG6Point3P10GameObject(self, 2, 0, p, other); check(calls.empty(), "kind 2: needs word +992");
    reset(3); unsigned w = 1; std::memcpy(self + 992, &w, 4); msg_alert__6GoblinifG6Point3P10GameObject(self, 2, 0, p, nullptr); check(calls.empty(), "kind 2: null other");
    reset(0); msg_alert__6GoblinifG6Point3P10GameObject(self, 1, 0, p, other); check(calls.empty(), "kind 1 does nothing");
    reset(0); msg_alert__6GoblinifG6Point3P10GameObject(self, 3, 0, p, other); check(calls.empty(), "kind 3 does nothing");
    if (!bad) std::puts("goblin msg_alert ok");
    return bad;
}
