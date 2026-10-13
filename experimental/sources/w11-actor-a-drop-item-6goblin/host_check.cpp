// Host contract for dropItem__6Goblini; not an EE input. 32-bit words are offsets from a host base; the resource
// map is a host array of such words; callees log. The authored function was also compared with the original in a
// private R5900 run. Returns non-zero when wrong.
#define W11_ACTOR_A_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w11_actor_a_host_base;
extern "C" { unsigned char goblinWeaponResourceMap[512] = {0}; }
#include "drop_item.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static void* allocResult;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* m, Point3 p, short a, void* s, int b, Point3* first, Point3* second, float f) {
    calls += "attach(" + num(off(m)) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(a) + "," + num(off(s) - 0x1000) + "," + num(b) + "," + num(f) + ",first=" +
             num(first->x) + ");";
    *first = Point3{7, 8, 9}; (void)second;
}
void* blockAlloc__Fi(int n) { calls += "alloc" + num(n) + ";"; return allocResult; }
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, Point3 p, char* a, char* b, char* c, long n, int flags, void* d, float f) {
    calls += "prop(" + num(off(self)) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(off(a)) + "," + num(off(b)) + "," + num(off(c)) + "," +
             num(n) + "," + num(flags) + "," + (d ? "desc" : "null") + "," + num(f) + ");";
}
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
int main() {
    int bad = 0;
    for (int allocated = 0; allocated < 2; ++allocated)
        for (int kind : {0, 1, 2, 3, -1})
            for (int n : {0, 1, 3, 5}) {
                std::memset(arena, 0, sizeof arena); w11_actor_a_host_base = arena; calls.clear();
                allocResult = allocated ? arena + 0x6000 : nullptr;
                for (int i = 0; i < 64; ++i) { unsigned v = 0x2000 + i * 16; std::memcpy(goblinWeaponResourceMap + i * 8, &v, 4); }
                put32(36, 0x300); putA(0x300 + 12, 0x400);
                float pt[3] = {4, 5, 6}; std::memcpy(obj + 16, pt, 12);
                short h = 100; std::memcpy(obj + 56, &h, 2);
                put32(988, n); put32(992, n + 40);
                dropItem__6Goblini(obj, kind);
                int used = kind == 1 ? n : kind == 2 ? n + 40 : 0;
                int flags = (kind == 1 ? 257 : 1) | (used == 3 ? 0x2000 : 0x4000);
                std::string want;
                if (used != 0) {
                    want = "attach(1024,4:5:6,-16285,288," + num(kind == 2) + ",1,first=4);alloc584;";
                    if (allocated) want += "prop(" + num(0x6000) + ",7:8:9," + num(0x2000) + "," + num(0x2000 + used * 16) + "," + num(0x2000 + (used + 7) * 16) + ",1," + num(flags) + ",null,1);";
                }
                unsigned w988, w992; std::memcpy(&w988, obj + 988, 4); std::memcpy(&w992, obj + 992, 4);
                const unsigned e988 = kind == 1 ? 0 : n, e992 = kind == 2 ? 0 : n + 40;
                if (calls != want || w988 != e988 || w992 != e992) { std::printf("fail alloc=%d kind=%d n=%d\n  want %s\n  got  %s\n", allocated, kind, n, want.c_str(), calls.c_str()); bad = 1; }
            }
    return bad;
}
