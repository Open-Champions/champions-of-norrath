// Host contract for runSpells__5MummyG6Point3; not an EE input. 32-bit words are offsets from a host
// base. The list iterator is a stub over a fixed node array; callees log. The authored function was
// also compared with the original in a private R5900 run.
#define W10_ACTOR_J_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_j_host_base;
#include "run_spells.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static unsigned nodes[4]; static int nodeCount, nodeIndex;

extern "C" {
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* m, Point3 p, short a, void* s, int b, Point3* first, Point3* second, float f) {
    calls += "attach(" + num(off(m)) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(a) + "," + num(off(s) - 0x1000) + "," + num(b) + "," + (second ? "2nd" : "null") + "," + num(f) + ");";
    *first = Point3{1, 2, 3};
}
void __16ListSafeIteratorP8ListHeadi(void* it, void* head, int mask) {
    calls += "iter(" + num(off(head) - 0x1000) + "," + num(mask) + ");"; nodeIndex = 0;
    unsigned first = nodeCount ? nodes[0] : 0; std::memcpy(static_cast<unsigned char*>(it) + 4, &first, 4);
}
void next__16ListSafeIterator(void* it) {
    calls += "next;"; ++nodeIndex; unsigned n = nodeIndex < nodeCount ? nodes[nodeIndex] : 0; std::memcpy(static_cast<unsigned char*>(it) + 4, &n, 4);
}
void _$_16ListSafeIterator(void*, int f) { calls += "done" + num(f) + ";"; }
}
unsigned w10_actor_j_host_call(unsigned fw, void* adjusted, int n, unsigned long, unsigned long a2) {
    calls += "v" + num(fw) + "(" + num(off(adjusted)) + "," + num(n) + "," + num(static_cast<int>(a2)) + ");"; return 0;
}
static void put32(unsigned char* base, unsigned o, unsigned v) { std::memcpy(base + o, &v, 4); }
int main() {
    int bad = 0;
    std::memset(arena, 0, sizeof arena); w10_actor_j_host_base = arena;
    put32(obj, 36, 0x300); put32(arena + 0x300, 12, 0x400);
    float pt[3] = {4, 5, 6}; std::memcpy(obj + 16, pt, 12);
    short h = -3; std::memcpy(obj + 56, &h, 2);
    // Node 0x500 holds spell 0x600 (table word at +20 -> 0x700, entry +16: adjustment -16, function 0x55);
    // node 0x510 has no object; node 0x520 holds spell 0x640 (adjustment 8, function 0x66).
    put32(arena + 0x500, 12, 0x600); put32(arena + 0x600, 20, 0x700); put32(arena + 0x700, 16, 0xfff0); put32(arena + 0x700, 20, 0x55);
    put32(arena + 0x510, 12, 0); put32(arena + 0x520, 12, 0x640); put32(arena + 0x640, 20, 0x720); put32(arena + 0x720, 16, 8); put32(arena + 0x720, 20, 0x66);
    nodes[0] = 0x500; nodes[1] = 0x510; nodes[2] = 0x520; nodeCount = 3;
    runSpells__5MummyG6Point3(obj, Point3{9, 9, 9});
    const std::string want = "attach(1024,4:5:6,-3,288,0,null,1);iter(888,0);v85(" + num(0x600 - 16) + ",2,-3);next;next;v102(" + num(0x640 + 8) + ",2,-3);next;done2;";
    if (calls != want) { std::printf("fail\n  want %s\n  got  %s\n", want.c_str(), calls.c_str()); bad = 1; }
    return bad;
}
