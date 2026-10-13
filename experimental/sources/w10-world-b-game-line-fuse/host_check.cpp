// Host contract for gameLineFuse__Fv. Not an EE plan input.
// Build: c++ -std=c++17 -DNORRATH_HOST_ARENA -ffp-contract=off host_check.cpp
// Record words are arena addresses; the gameLines array holds host pointers. The nops are
// counted through the asm macro so the final spin length can be checked.
#ifndef NORRATH_HOST_ARENA
#define NORRATH_HOST_ARENA
#endif
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include "../w10-world-b-common/host_arena.hpp"
#include "../w10-world-b-common/host_check.hpp"
unsigned char* gameLines[100];
int gameLineCount;
static std::string events;
static long nops;
static bool alloc_fails;
static unsigned alloc_next;
static unsigned char* off(unsigned char* p) { return p; }
extern "C" unsigned char* blockAlloc__Fi(int size) {
    events += "A" + std::to_string(size) + ";";
    if (alloc_fails) return nullptr;
    unsigned char* p = arena_at(0x7000 + 0x40 * alloc_next++);
    std::memset(p, 0, 24);
    return p;
}
extern "C" void __4LineP4LineT1i(unsigned char* self, unsigned char* a, unsigned char* b, int flag) {
    events += "C" + std::to_string(self - arena_at(0)) + "," + std::to_string(a - arena_at(0)) + "," + std::to_string(b - arena_at(0)) + "," + std::to_string(flag) + ";";
    // The new line takes the first line's name, kind and a vertex array for the next scan.
    std::memcpy(self, a, 24);
}
extern "C" void _$_4Line(unsigned char* self, int flags) { events += "D" + std::to_string(self - arena_at(0)) + "," + std::to_string(flags) + ";"; }
#define asm ++nops, (void)
#define volatile
#include "game_line_fuse.cpp"
#undef asm
#undef volatile
static const int MAXL = 10;
static unsigned char* rec(int i) { return arena_at(0x1000 + 0x100 * i); }
static unsigned char* verts(int i) { return arena_at(0x3000 + 0x100 * i); }
static unsigned char* nameat(int i) { return arena_at(0x6000 + 0x20 * i); }
struct Spec { int kind; int name; float first[3]; float last[3]; };
static void build(const std::vector<Spec>& s) {
    std::memset(arena_at(0), 0, 0x8000);
    for (size_t i = 0; i != s.size(); ++i) {
        const char* names[] = {"east", "west", "north"};
        std::strcpy(reinterpret_cast<char*>(nameat(s[i].name)), names[s[i].name]);
        sw_(rec(i), norrath_host_address(nameat(s[i].name)));
        sw_(rec(i) + 4, 3); sw_(rec(i) + 8, s[i].kind); sw_(rec(i) + 20, norrath_host_address(verts(i)));
        for (int k = 0; k != 3; ++k) { sf_(verts(i) + 4 * k, s[i].first[k]); sf_(verts(i) + 128 + 4 * k, s[i].last[k]); }
        gameLines[i] = rec(i);
    }
    gameLineCount = int(s.size()); events.clear(); nops = 0; alloc_fails = false; alloc_next = 0;
}
static std::string id(unsigned char* p) { return std::to_string(p - arena_at(0)); }
int main() {
    // Nothing to do: empty, a single line, or lines that do not match.
    build({}); gameLineFuse__Fv(); if (!events.empty() || nops != 0 || gameLineCount != 0) return 1;
    build({{0, 0, {0, 0, 0}, {10, 0, 0}}}); gameLineFuse__Fv(); if (!events.empty() || nops != 6 || gameLineCount != 1) return 2;
    // Two lines of the same kind and name; A's end (10,0,0) is 5 away from B's start.
    build({{0, 0, {0, 0, 0}, {10, 0, 0}}, {0, 0, {15, 0, 0}, {40, 0, 0}}});
    unsigned char* a = rec(0); unsigned char* b = rec(1);
    gameLineFuse__Fv();
    // The new line is built from (a, b); A's start is 40 from B's end, below 96, so the flag is 1.
    unsigned char* fresh = arena_at(0x7000);
    std::string want = "A24;C" + id(fresh) + "," + id(a) + "," + id(b) + ",1;D" + id(a) + ",3;D" + id(b) + ",3;";
    if (events != want) { std::printf("%s\n%s\n", events.c_str(), want.c_str()); return 3; }
    if (gameLineCount != 1 || gameLines[0] != fresh) return 4;
    if (nops != 6) return 5;                                    // spin after the last scan: one line, six nops
    // Different kind or different name: no fuse.
    build({{0, 0, {0, 0, 0}, {10, 0, 0}}, {1, 0, {15, 0, 0}, {40, 0, 0}}}); gameLineFuse__Fv();
    if (!events.empty() || gameLineCount != 2 || nops != 12) return 6;
    build({{0, 0, {0, 0, 0}, {10, 0, 0}}, {0, 1, {15, 0, 0}, {40, 0, 0}}}); gameLineFuse__Fv();
    if (!events.empty() || gameLineCount != 2) return 7;
    // Gap of exactly 96 or more is not close enough; just under is.
    build({{0, 0, {0, 0, 0}, {10, 0, 0}}, {0, 0, {106, 0, 0}, {140, 0, 0}}}); gameLineFuse__Fv();
    if (!events.empty()) return 8;
    build({{0, 0, {0, 0, 0}, {10, 0, 0}}, {0, 0, {105.9f, 0, 0}, {140, 0, 0}}}); gameLineFuse__Fv();
    if (events.empty()) return 9;
    // Flag 0 when A's start is 96 or more from B's end.
    build({{0, 0, {0, 0, 0}, {10, 0, 0}}, {0, 0, {15, 0, 0}, {200, 0, 0}}}); gameLineFuse__Fv();
    if (events.find(",0;D") == std::string::npos) return 10;
    // The nearest of two candidates wins; the first of equals.
    build({{0, 0, {0, 0, 0}, {10, 0, 0}}, {0, 0, {50, 0, 0}, {60, 0, 0}}, {0, 0, {20, 0, 0}, {70, 0, 0}}});
    unsigned char* r0 = rec(0); unsigned char* r2 = rec(2); unsigned char* r1 = rec(1);
    gameLineFuse__Fv();
    if (events.rfind("A24;C" + id(arena_at(0x7000)) + ",", 0) != 0) return 11;
    if (events.find("C" + id(arena_at(0x7000)) + "," + id(r0) + "," + id(r2) + ",") == std::string::npos) return 12;
    (void)r1;
    // Allocation failure is not exercised here: the image appends the null result to the table and
    // its next scan reads through it, so the path cannot run to completion on a host.
    std::puts("gameLineFuse check passed");
    return 0;
}
