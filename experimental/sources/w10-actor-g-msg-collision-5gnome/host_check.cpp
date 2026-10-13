// Host contract for msg_collision__5GnomeP10GameObject; not an EE input.
#define W10_ACTOR_G_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10g_host_base;
static std::string calls;
static int setterArg; static void* setterObject;
static void setter(void* o, int v) { calls += "set;"; setterObject = o; setterArg = v; }
static unsigned char nodeA[4], nodeB[4];
static void* nodeFn() { calls += "type;"; return nodeB; }
void* w10g_host_fn(unsigned w) { return w == 1 ? reinterpret_cast<void*>(nodeFn) : reinterpret_cast<void*>(setter); }
#include "msg_collision.cpp"
signed char cutMode;
static bool eqResult; static unsigned rollValue = 1000007;
static void *listA, *listB, *listC, *baseSelf, *baseOther;
extern "C" void* __tf5Gnome() { calls += "tf;"; return nodeA; }
extern "C" int __eq__C9type_infoRC9type_info(const void* a, const void* b) {
    calls += "eq;"; if (a != nodeB || b != nodeA) calls += "BADARGS;"; return eqResult;
}
extern "C" void listDestroy__FP8ListHead(void* h) { calls += "destroy;"; listA = h; }
extern "C" void listOnewayLink__FP8ListHeadT0Pv(void* h, void* o, void* d) { calls += "link;"; listB = h; listC = o; baseOther = d; }
extern "C" unsigned eRand__10GameObject(void*) { calls += "rand;"; return rollValue; }
extern "C" int msg_collision__8CreatureP10GameObject(void* s, void* o) { calls += "base;"; baseSelf = s; baseOther = o; return 4321; }
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static int bad;
static void check(bool ok, const char* what) { if (!ok) { std::printf("fail: %s (calls %s)\n", what, calls.c_str()); bad = 1; } }
static void w32(unsigned char* base, int off, int v) { std::memcpy(base + off, &v, 4); }
static unsigned r32(unsigned char* base, int off) { unsigned v; std::memcpy(&v, base + off, 4); return v; }
int main() {
    w10g_host_base = arena;
    unsigned char* self = arena + 0x1000; unsigned char* other = arena + 0x2000;
    auto reset = [&]() {
        std::memset(arena, 0, sizeof arena); calls.clear();
        unsigned tab = 0x200, otab = 0x300, one = 1, two = 2;
        std::memcpy(self + 72, &tab, 4); std::memcpy(other + 72, &otab, 4);
        std::memcpy(arena + otab + 4, &one, 4);
        short adj = 24; std::memcpy(arena + tab + 184, &adj, 2); std::memcpy(arena + tab + 188, &two, 4);
        w32(self, 972, 10); w32(self, 808, 50); w32(other, 972, 1); w32(other, 808, 7);
        eqResult = true; cutMode = 0; setterObject = nullptr; setterArg = 0; rollValue = 1000007;
    };
    reset();
    check(msg_collision__5GnomeP10GameObject(self, other) == 4321, "returns the base result");
    check(calls == "type;tf;eq;destroy;link;set;rand;base;", "full path call order");
    check(listA == self + 968 && listB == self + 968 && listC == other && baseSelf == self, "list head +968, linked to the other object");
    check(setterObject == self + 24 && setterArg == 13, "setter via table entry +184 with 13");
    check(r32(self, 972) == 50u + 1000007u % 180u + 180u, "word +972 = +808 + 180 + roll % 180");
    reset(); w32(self, 972, 0x80000000); w32(other, 808, 0);
    msg_collision__5GnomeP10GameObject(self, other);
    check(calls == "type;tf;eq;base;", "signed comparison on the other object's words");
    reset(); w32(self, 972, 50); msg_collision__5GnomeP10GameObject(self, other);
    check(calls == "base;", "own +972 not below +808: base only");
    reset(); cutMode = 3; msg_collision__5GnomeP10GameObject(self, other);
    check(calls == "base;", "nonzero cutMode: base only");
    reset(); msg_collision__5GnomeP10GameObject(self, nullptr);
    check(calls == "base;", "null other: base only");
    reset(); eqResult = false; msg_collision__5GnomeP10GameObject(self, other);
    check(calls == "type;tf;eq;base;", "other type differs: base only");
    reset(); w32(other, 972, 7); msg_collision__5GnomeP10GameObject(self, other);
    check(calls == "type;tf;eq;base;", "other not below: base only");
    if (!bad) std::puts("gnome msg_collision ok");
    return bad;
}
