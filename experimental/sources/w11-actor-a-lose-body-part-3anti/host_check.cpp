// Host contract for loseBodyPart__3Anti; not an EE input. Words that hold addresses are offsets from a host base;
// the callees are stubs that log. The authored function was also compared with the original in a private R5900
// run (56 scenarios over parts -2..13 and large values). Returns non-zero when wrong.
#define W11_ACTOR_A_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
#include <initializer_list>
#include "../w11-actor-a-common/access.hpp"
unsigned char* w11_actor_a_host_base;
static std::string calls;
static unsigned char* allocResult;
static int nChunks;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
extern "C" {
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void*, int mask) {
    calls += "m" + num(mask) + ";";
    return nChunks++ == 0 ? 0xff00ff00ff00ff00ull : 0x0f000f000f000f00ull | static_cast<unsigned>(mask);
}
void* blockAlloc__Fi(int n) { calls += "a" + num(n) + ";"; return allocResult; }
void __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif(void* self, Point3 p, short ang, void*, void*, char* a, char* b, char* c,
                                                                    unsigned long long chunks, int type, int two, float scale) {
    char buf[200];
    std::snprintf(buf, sizeof buf, "ctor(%p,%g:%g:%g,%d,%llx,%d,%d,%g);", self == allocResult ? (void*)1 : (void*)0, p.x, p.y, p.z, ang, chunks, type, two, scale);
    calls += buf;
    (void)a; (void)b; (void)c;
}
}
#include "lose_body_part.cpp"
int main() {
    alignas(16) static unsigned char arena[0x8000];
    w11_actor_a_host_base = arena;
    int bad = 0;
    unsigned char* o = arena + 0x1000;
    for (int part : {0, 1, 7, 8, -1, 100})
        for (int alloc = 0; alloc < 2; ++alloc) {
            std::memset(arena, 0, sizeof arena); calls.clear(); nChunks = 0;
            unsigned rw = 0x200, mdl = 0x300; std::memcpy(o + 36, &rw, 4); std::memcpy(arena + 0x200 + 12, &mdl, 4);
            float pos[3] = {1, 2, 3}; std::memcpy(o + 16, pos, 12);
            short ang = 77; std::memcpy(o + 56, &ang, 2);
            unsigned flags = 0x1; std::memcpy(o + 244, &flags, 4);
            allocResult = alloc ? arena + 0x5000 : nullptr;
            float f452 = 1.5f; std::memcpy(arena + 0x5000 + 452, &f452, 4);
            loseBodyPart__3Anti(o, part);
            unsigned nf; std::memcpy(&nf, o + 244, 4);
            unsigned want = (part >= 0 && part < 8) ? (flags | (2u << part)) : flags;
            if (nf != want) { std::printf("flags part=%d got %x want %x\n", part, nf, want); bad = 1; }
            unsigned long long stored; std::memcpy(&stored, o + 552, 8);
            if (stored != (0x0f000f000f000f00ull | want)) { std::printf("stored %llx\n", stored); bad = 1; }
            unsigned long long lost = 0xff00ff00ff00ff00ull & ~stored;
            std::string want_calls = "m1;m" + num((int)want) + ";a616;";
            if (alloc) {
                char b[200];
                std::snprintf(b, sizeof b, "ctor(%p,%g:%g:%g,%d,%llx,%d,%d,%g);", (void*)1, 1.0, 2.0, 3.0, 77, lost, 14, 2, 1.0);
                want_calls += b;
                float g; std::memcpy(&g, arena + 0x5000 + 452, 4);
                if (g != 3.5f) bad = 1;
            }
            if (calls != want_calls) { std::printf("calls part=%d alloc=%d\n  want %s\n  got  %s\n", part, alloc, want_calls.c_str(), calls.c_str()); bad = 1; }
        }
    return bad;
}
