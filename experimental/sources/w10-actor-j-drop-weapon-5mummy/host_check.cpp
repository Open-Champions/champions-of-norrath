// Host contract for dropWeapon__5Mummy; not an EE input. 32-bit words are offsets from a host base;
// callees log. The authored function was also compared with the original in a private R5900 run.
#define W10_ACTOR_J_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_j_host_base;
#include "drop_weapon.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static void* allocResult;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* m, Point3 p, short a, void* s, int b, Point3* first, Point3* second, float f) {
    calls += "attach(" + num(off(m)) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(a) + "," + num(off(s) - 0x1000) + "," + num(b) + "," + num(f) + ");";
    *first = Point3{7, 8, 9}; *second = Point3{0, 0, 0};
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
    for (int allocated = 0; allocated < 2; ++allocated) {
        for (unsigned k : {1u, 3u}) {
            std::memset(arena, 0, sizeof arena); w10_actor_j_host_base = arena; calls.clear();
            allocResult = allocated ? arena + 0x6000 : nullptr;
            put32(36, 0x300); putA(0x300 + 0, 0x1100); putA(0x300 + 12, 0x400);
            putA(0x300 + 8 * k, 0x1200 + k); putA(0x300 + 8 * k + 8, 0x1300 + k);
            float pt[3] = {4, 5, 6}; std::memcpy(obj + 16, pt, 12);
            short h = 100; std::memcpy(obj + 56, &h, 2);
            put32(964, k);
            dropWeapon__5Mummy(obj);
            std::string want = "attach(1024,4:5:6,-16284,288,0,1);alloc584;";
            if (allocated) want += "prop(" + num(0x6000) + ",7:8:9,4352," + num(0x1200 + k) + "," + num(0x1300 + k) + ",1,16641,null,1);";
            unsigned w964; std::memcpy(&w964, obj + 964, 4);
            if (calls != want || w964 != 0) { std::printf("fail alloc=%d k=%u\n  want %s\n  got  %s\n", allocated, k, want.c_str(), calls.c_str()); bad = 1; }
        }
    }
    return bad;
}
