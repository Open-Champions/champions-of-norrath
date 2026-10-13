// Host contract for loseBodyPart__10BlackWidowi; not an EE input. Words that hold addresses are offsets from a
// host base; the callees are stubs that log. The authored function was also compared with the original in a
// private R5900 run (84 scenarios). Returns non-zero when wrong.
#define W11_ACTOR_A_HOST 1
#include "../w11-actor-a-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <initializer_list>
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
void __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif(void* self, Point3 p, short ang, void*, void*, char*, char*, char*,
                                                                    unsigned long long chunks, int type, int two, float scale) {
    char buf[200];
    std::snprintf(buf, sizeof buf, "ctor(%d,%g:%g:%g,%d,%llx,%d,%d,%g);", self == allocResult, p.x, p.y, p.z, ang, chunks, type, two, scale);
    calls += buf;
}
}
#include "lose_body_part.cpp"
int main() {
    alignas(16) static unsigned char arena[0x8000];
    w11_actor_a_host_base = arena;
    int bad = 0;
    unsigned char* o = arena + 0x1000;
    for (int part : {0, 3, 7, 8, 11, 12, -1})
        for (int alloc = 0; alloc < 2; ++alloc)
            for (int big = 0; big < 2; ++big) {
                std::memset(arena, 0, sizeof arena); calls.clear(); nChunks = 0;
                unsigned rw = 0x200, mdl = 0x300; std::memcpy(o + 36, &rw, 4); std::memcpy(arena + 0x200 + 12, &mdl, 4);
                float pos[3] = {1, 2, 3}; std::memcpy(o + 16, pos, 12);
                short ang = 77; std::memcpy(o + 56, &ang, 2);
                unsigned flags = 0x1; std::memcpy(o + 244, &flags, 4);
                unsigned long long w160 = big ? 0x1000000ull : 0; std::memcpy(o + 160, &w160, 8);
                float scale = 4.5f; std::memcpy(o + 224, &scale, 4);
                allocResult = alloc ? arena + 0x5000 : nullptr;
                loseBodyPart__10BlackWidowi(o, part);
                unsigned nf; std::memcpy(&nf, o + 244, 4);
                unsigned want = (part >= 0 && part < 12) ? (flags | (1u << part)) : flags;
                if (nf != want) { std::printf("flags part=%d got %x want %x\n", part, nf, want); bad = 1; }
                unsigned long long stored; std::memcpy(&stored, o + 552, 8);
                if (stored != (0x0f000f000f000f00ull | want)) { std::printf("stored %llx\n", stored); bad = 1; }
                const bool spawn = !(part >= 8 && part < 12);
                std::string want_calls = "m1;m" + num((int)want) + ";";
                if (spawn) {
                    want_calls += "a616;";
                    if (alloc) {
                        char b[200];
                        std::snprintf(b, sizeof b, "ctor(1,1:2:3,77,%llx,%d,2,4.5);", 0xff00ff00ff00ff00ull & ~stored, big ? 270 : 14);
                        want_calls += b;
                    }
                }
                if (calls != want_calls) { std::printf("calls part=%d alloc=%d\n  want %s\n  got  %s\n", part, alloc, want_calls.c_str(), calls.c_str()); bad = 1; }
            }
    return bad;
}
