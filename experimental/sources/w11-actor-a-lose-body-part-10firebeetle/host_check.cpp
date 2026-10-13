// Host contract for loseBodyPart__10FireBeetlei; not an EE input. Words that hold addresses are offsets from a
// host base; the callees are stubs that log. The authored function was also compared with the original in a
// private R5900 run (80 scenarios). Returns non-zero when wrong.
#define W11_ACTOR_A_HOST 1
#include "../w11-actor-a-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <initializer_list>
unsigned char* w11_actor_a_host_base;
static std::string calls;
static unsigned char* allocResult;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
extern "C" {
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void*, int mask) {
    calls += "m" + num(mask) + ";";
    return 0x0f000f000f000f00ull | static_cast<unsigned>(mask);
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
    const unsigned long long masks[6] = {4, 6, 64, 3072, 128, 768};
    for (int part : {0, 1, 2, 3, 4, 5, 6, 7, 8, -1})
        for (int alloc = 0; alloc < 2; ++alloc) {
            std::memset(arena, 0, sizeof arena); calls.clear();
            unsigned rw = 0x200, mdl = 0x300; std::memcpy(o + 36, &rw, 4); std::memcpy(arena + 0x200 + 12, &mdl, 4);
            float pos[3] = {1, 2, 3}; std::memcpy(o + 16, pos, 12);
            short ang = 77; std::memcpy(o + 56, &ang, 2);
            unsigned flags = 0x1; std::memcpy(o + 244, &flags, 4);
            allocResult = alloc ? arena + 0x5000 : nullptr;
            float f452 = 1.5f; std::memcpy(arena + 0x5000 + 452, &f452, 4);
            loseBodyPart__10FireBeetlei(o, part);
            const bool hit = part >= 2 && part < 8;
            unsigned nf; std::memcpy(&nf, o + 244, 4);
            unsigned want = hit ? (flags | (2u << (part - 2))) : flags;
            if (nf != want) { std::printf("flags part=%d got %x want %x\n", part, nf, want); bad = 1; }
            unsigned long long stored; std::memcpy(&stored, o + 552, 8);
            if (stored != (0x0f000f000f000f00ull | want)) { std::printf("stored %llx\n", stored); bad = 1; }
            std::string want_calls = "a616;";
            if (alloc) {
                char b[200];
                std::snprintf(b, sizeof b, "ctor(1,1:2:3,77,%llx,7,2,1);", hit ? masks[part - 2] : 0ull);
                want_calls += b;
                float g; std::memcpy(&g, arena + 0x5000 + 452, 4);
                if (g != 3.5f) bad = 1;
            }
            want_calls += "m" + num((int)want) + ";";
            if (calls != want_calls) { std::printf("calls part=%d alloc=%d\n  want %s\n  got  %s\n", part, alloc, want_calls.c_str(), calls.c_str()); bad = 1; }
        }
    return bad;
}
