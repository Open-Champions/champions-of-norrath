// Host contract for dropItem__3Orci; not an EE input. 32-bit words are offsets from a host base; callees log.
// The authored function was also compared with the original in a private R5900 run. Returns non-zero when wrong.
#define W11_ACTOR_A_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w11_actor_a_host_base;
#include "drop_item.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static void* allocResult;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* m, Point3 p, short a, void* s, int b, Point3* first, Point3*, float f) {
    calls += "attach(" + num(off(m)) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(a) + "," + num(off(s) - 0x1000) + "," + num(b) + "," + num(f) + ",first=" + num(first->x) + ");";
    *first = Point3{7, 8, 9};
}
void* blockAlloc__Fi(int n) { calls += "alloc" + num(n) + ";"; return allocResult; }
void __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif(void* self, Point3 p, short a, void* m, void* s, char* x, char* y, char* z,
                                                                    unsigned long long mask, int t, int two, float f) {
    char b[300];
    std::snprintf(b, sizeof b, "chunk(%u,%g:%g:%g,%d,%u,%u,%u,%u,%u,%llx,%d,%d,%g);", off(self), p.x, p.y, p.z, a, off(m), off(s) - 0x1000, off(x), off(y), off(z), mask, t, two, f);
    calls += b;
}
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, Point3 p, char* a, char* b, char* c, long n, int flags, void* d, float f) {
    calls += "prop(" + num(off(self)) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(off(a)) + "," + num(off(b)) + "," + num(off(c)) + "," + num(n) + "," +
             num(flags) + "," + (d ? "desc" : "null") + "," + num(f) + ");";
}
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* m, int bits) { calls += "chunks(" + num(off(m)) + "," + std::to_string(static_cast<unsigned>(bits)) + ");"; return 0x100000000ull * static_cast<unsigned>(bits) + 5; }
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
int main() {
    int bad = 0;
    // Expected GutChunk masks for kind 1, for chosen words at +244.
    struct Mask { unsigned old; unsigned long long mask; };
    const Mask masks[] = {{0, 0x20406}, {1, 0x60406}, {2, 0x20404 | 0x80000}, {4, 0x20400 | 0x300000}, {0x3, 0x60404 | 0x80000},
                          {0x100, 0x20406ull | 0x80000000ull}, {0x200, 0x20406ull | 0x100000000ull}, {0x400, 0x20406ull | 0x200000000ull}, {0x800, 0x20406ull | 0x400000000ull}};
    // kind 1
    for (const Mask& mk : masks)
        for (int allocated = 0; allocated < 2; ++allocated) {
            std::memset(arena, 0, sizeof arena); w11_actor_a_host_base = arena; calls.clear();
            allocResult = allocated ? arena + 0x6000 : nullptr;
            put32(36, 0x300); putA(0x300 + 12, 0x400); putA(0x300, 0x1100); putA(0x300 + 8, 0x1200); putA(0x300 + 16, 0x1300);
            float pt[3] = {4, 5, 6}; std::memcpy(obj + 16, pt, 12);
            short h = 100; std::memcpy(obj + 56, &h, 2);
            put32(244, mk.old);
            float f452 = 1.5f; std::memcpy(arena + 0x6000 + 452, &f452, 4);
            dropItem__3Orci(obj, 1);
            unsigned nb = mk.old | 0x10000000u;
            std::string want = "attach(1024,4:5:6,-16285,288,8,1,first=4);alloc616;";
            if (allocated) want += "chunk(" + num(0x6000) + ",4:5:6,100,1024,288,4352,4608,4864," + [&] { char b[40]; std::snprintf(b, sizeof b, "%llx", mk.mask); return std::string(b); }() + ",7,0,1);";
            want += "chunks(1024," + std::to_string(nb) + ");";
            float got452; std::memcpy(&got452, arena + 0x6000 + 452, 4);
            unsigned w244; std::memcpy(&w244, obj + 244, 4);
            unsigned long long q; std::memcpy(&q, obj + 552, 8);
            if (calls != want || w244 != nb || q != 0x100000000ull * nb + 5 || (allocated ? got452 != 3.5f : got452 != 1.5f)) {
                std::printf("fail kind 1 old=%x alloc=%d\n  want %s\n  got  %s\n", mk.old, allocated, want.c_str(), calls.c_str()); bad = 1;
            }
        }
    // kinds 2..5 and unknown kinds
    struct K { int kind; unsigned orBits; unsigned long long mask; int selector; };
    const K ks[] = {{2, 0x08000000u, 0x800, 6}, {3, 0x04000000u, 0x10, 7}, {4, 0x02000000u, 0x1000, 4}, {5, 0x01000000u, 0x200, 5}};
    for (const K& k : ks) {
        std::memset(arena, 0, sizeof arena); w11_actor_a_host_base = arena; calls.clear(); allocResult = arena + 0x6000;
        put32(36, 0x300); putA(0x300 + 12, 0x400); float pt[3] = {4, 5, 6}; std::memcpy(obj + 16, pt, 12); put32(244, 0x40);
        dropItem__3Orci(obj, k.kind);
        char b[200]; std::snprintf(b, sizeof b, "attach(1024,4:5:6,-16385,288,%d,1,first=4);alloc616;chunk(24576,4:5:6,0,1024,288,0,0,0,%llx,7,0,1);chunks(1024,%u);", k.selector, k.mask, 0x40u | k.orBits);
        if (calls != b) { std::printf("fail kind %d\n  want %s\n  got  %s\n", k.kind, b, calls.c_str()); bad = 1; }
    }
    // kinds 6 and 7: prop drops through the table at +1136
    for (int kind : {6, 7, 0, 9, -1})
        for (unsigned index : {0u, 1u, 2u, 5u})
            for (int allocated = 0; allocated < 2; ++allocated) {
                std::memset(arena, 0, sizeof arena); w11_actor_a_host_base = arena; calls.clear(); allocResult = allocated ? arena + 0x6000 : nullptr;
                put32(36, 0x300); putA(0x300 + 12, 0x400); float pt[3] = {4, 5, 6}; std::memcpy(obj + 16, pt, 12);
                put32(244, 0x40); put32(1136, 0x2000); put32(972, index); put32(1112, index);
                for (unsigned i = 0; i < 8; ++i) { putA(0x2000 + 8 * i, 0x3000 + i * 16); putA(0x2000 + 8 * i + 136, 0x4000 + i * 16); }
                dropItem__3Orci(obj, kind);
                std::string want;
                int selector = kind == 6 ? (index == 2 ? 1 : 0) : kind == 7 ? 3 : -1;
                int flags = kind == 6 ? ((index == 2 ? 8192 : 16384) | 0x100) : kind == 7 ? 16384 : 0;
                unsigned idx = (kind == 6 || kind == 7) ? index : 0;
                if (selector != -1) want += "attach(1024,4:5:6,-16385,288," + num(selector) + ",1,first=4);";
                if (idx) {
                    want += "alloc584;";
                    if (allocated) want += "prop(" + num(0x6000) + ",7:8:9," + num(0x3000) + "," + num(0x3000 + idx * 16) + "," + num(0x4000 + idx * 16) + ",1," + num(flags | 1) + ",null,1);";
                }
                want += "chunks(1024,64);";
                unsigned w972, w1112; std::memcpy(&w972, obj + 972, 4); std::memcpy(&w1112, obj + 1112, 4);
                if (selector == -1) { /* no attach: first stays the object position */ }
                if (selector == -1 && idx) bad = 1;
                if (calls != want || (kind == 6 && w972 != 0) || (kind == 7 && w1112 != 0)) { std::printf("fail kind %d index %u alloc %d\n  want %s\n  got  %s\n", kind, index, allocated, want.c_str(), calls.c_str()); bad = 1; }
            }
    return bad;
}
