// Host contract for msg_moveCollision__5GnomePP10GameObject; not an EE input.
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
#include "msg_move_collision.cpp"
static bool eqResult; static void *listA, *listB, *listC, *listD, *baseSelf, *baseOthers;
extern "C" void* __tf5Gnome() { calls += "tf;"; return nodeA; }
extern "C" int __eq__C9type_infoRC9type_info(const void* a, const void* b) {
    calls += "eq;"; if (a != nodeB || b != nodeA) calls += "BADARGS;"; return eqResult;
}
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned linkToSet;
extern "C" void listDestroy__FP8ListHead(void* h) { calls += "destroy;"; listA = h; }
extern "C" void listOnewayLink__FP8ListHeadT0Pv(void* h, void* o, void* d) {
    calls += "link;"; listB = h; listC = o; listD = d;
    std::memcpy(h, &linkToSet, 4);   // the real routine leaves the first link in the head
}
extern "C" int msg_moveCollision__8CreaturePP10GameObject(void* s, void** o) { calls += "base;"; baseSelf = s; baseOthers = o; return 99; }
static int bad;
static void check(bool ok, const char* what) { if (!ok) { std::printf("fail: %s (calls %s)\n", what, calls.c_str()); bad = 1; } }
static void w32(unsigned char* base, int off, int v) { std::memcpy(base + off, &v, 4); }
static unsigned r32(unsigned char* base, int off) { unsigned v; std::memcpy(&v, base + off, 4); return v; }
int main() {
    w10g_host_base = arena;
    unsigned char* self = arena + 0x1000; unsigned char* other = arena + 0x2000; unsigned char* third = arena + 0x2800;
    void* others[3] = {other, nullptr, nullptr};
    auto reset = [&]() {
        std::memset(arena, 0, sizeof arena); calls.clear();
        unsigned tab = 0x200, otab = 0x300, one = 1, two = 2;
        std::memcpy(self + 72, &tab, 4); std::memcpy(other + 72, &otab, 4);
        std::memcpy(arena + otab + 4, &one, 4);
        short adj = 24; std::memcpy(arena + tab + 184, &adj, 2); std::memcpy(arena + tab + 188, &two, 4);
        w32(self, 972, 10); w32(self, 808, 50); short thirteen = 13; std::memcpy(other + 572, &thirteen, 2);
        w32(third, 972, 400); unsigned linkAt = 0x3000; 
        linkToSet = linkAt; unsigned thirdOff = static_cast<unsigned>(third - arena); std::memcpy(arena + linkAt + 12, &thirdOff, 4);
        eqResult = true; setterObject = nullptr; setterArg = 0; others[0] = other;
    };
    reset();
    check(msg_moveCollision__5GnomePP10GameObject(self, others) == 99, "returns the base result");
    check(calls == "type;tf;eq;destroy;link;set;base;", "full path call order");
    check(listA == self + 968 && listB == self + 968 && listC == other && listD == other && baseOthers == others, "list head +968, entry linked twice");
    check(setterObject == self + 24 && setterArg == 13, "setter via table entry +184 with 13");
    check(r32(self, 972) == 400u + 60u, "word +972 = linked object's +972 plus 60");
    reset(); linkToSet = 0; msg_moveCollision__5GnomePP10GameObject(self, others);
    check(r32(self, 972) == 10u && calls == "type;tf;eq;destroy;link;set;base;", "empty list: +972 untouched");
    reset(); short twelve = 12; std::memcpy(other + 572, &twelve, 2); msg_moveCollision__5GnomePP10GameObject(self, others);
    check(calls == "type;tf;eq;base;", "other state not 13: base only");
    reset(); eqResult = false; msg_moveCollision__5GnomePP10GameObject(self, others);
    check(calls == "type;tf;eq;base;", "type differs: base only");
    reset(); others[0] = nullptr; msg_moveCollision__5GnomePP10GameObject(self, others);
    check(calls == "base;", "empty array: base only");
    reset(); w32(self, 972, 50); msg_moveCollision__5GnomePP10GameObject(self, others);
    check(calls == "base;", "own +972 not below +808: base only");
    reset(); w32(self, 972, -5); w32(self, 808, -4); msg_moveCollision__5GnomePP10GameObject(self, others);
    check(calls.find("set;") != std::string::npos, "signed comparison");
    if (!bad) std::puts("gnome msg_moveCollision ok");
    return bad;
}
