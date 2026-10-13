// Host contract for freeLump__FPv. Not an EE plan input. Build: c++ -std=c++17 -DNORRATH_HOST_ARENA host_check.cpp
#ifndef NORRATH_HOST_ARENA
#define NORRATH_HOST_ARENA
#endif
#include <string>
#include "../w10-world-b-common/host_arena.hpp"
#include "../w10-world-b-common/host_check.hpp"
static std::string events;
extern "C" void textureFreeTexture__FP8_texture(void* p) { events += "T" + std::to_string(norrath_host_address((unsigned char*)p) - norrath_arena_base) + ";"; }
extern "C" void worldFreeWorld__FP12_worldHeader(void* p) { events += "W" + std::to_string(norrath_host_address((unsigned char*)p) - norrath_arena_base) + ";"; }
extern "C" void textRemoveText__FPv(void* p) { events += "U" + std::to_string(norrath_host_address((unsigned char*)p) - norrath_arena_base) + ";"; }
#include "free_lump.cpp"
int main() {
    unsigned char* lump = arena_at(0x100);
    // Names live in the arena as C strings; data pointers are arena addresses 0x1000 + 16 * i.
    const char* names[] = {"a.tex", "b.world", "c.uni", "d", "e.TEX", "f.tex.world", "g.", "h.texx", ".uni", "i.text", "x.y.uni", "w.worl"};
    const unsigned n = sizeof names / sizeof names[0];
    for (unsigned i = 0; i != n; ++i) {
        std::strcpy(reinterpret_cast<char*>(arena_at(0x800 + 32 * i)), names[i]);
        unsigned char* e = lump + 4 + 12 * i;
        sw_(e, norrath_host_address(arena_at(0x800 + 32 * i)));
        sw_(e + 4, norrath_host_address(arena_at(0x1000 + 16 * i)));
        sw_(e + 8, 0xdeadbeefu);
    }
    sh_(lump, n);
    freeLump__FPv(lump);
    // Expected, by independent reading of the table: a.tex -> T, b.world -> W, c.uni -> U, d skipped,
    // e.TEX skipped (case matters), f.tex.world -> W, g. skipped, h.texx skipped, .uni -> U,
    // i.text skipped, x.y.uni -> U, w.worl skipped.
    std::string expect;
    auto d = [&](int i) { return std::to_string(norrath_host_address(arena_at(0x1000 + 16 * i)) - norrath_arena_base); };
    expect = "T" + d(0) + ";W" + d(1) + ";U" + d(2) + ";W" + d(5) + ";U" + d(8) + ";U" + d(10) + ";";
    if (events != expect) { std::printf("got %s\nwant %s\n", events.c_str(), expect.c_str()); return 1; }
    // Zero count and null lump do nothing; count 1 reads only the first entry.
    events.clear(); sh_(lump, 0); freeLump__FPv(lump); freeLump__FPv(nullptr);
    if (!events.empty()) return 2;
    sh_(lump, 1); freeLump__FPv(lump);
    if (events != "T" + d(0) + ";") return 3;
    // The count is a halfword: upper bytes of the word are ignored.
    events.clear(); sw_(lump, 0x7f7f0002u); freeLump__FPv(lump);
    if (events != "T" + d(0) + ";W" + d(1) + ";") return 4;
    std::puts("freeLump check passed");
    return 0;
}
