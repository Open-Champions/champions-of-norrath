// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
static int calls, result;
static unsigned char* seen_self;
extern "C" int ResetCache__7VIRFontR8VIRaster(unsigned char* self, unsigned char*) { ++calls; seen_self = self; return result; }
#include "unlock.cpp"
#include <cstdio>
int main() {
    int& flag = *reinterpret_cast<int*>(arena + 8);
    bool ok = true;
    flag = 0; calls = 0; result = 0;
    ok = ok && Unlock__7VIRFontR8VIRaster(arena, nullptr) == -1 && calls == 0;
    flag = 1; result = 0;
    ok = ok && Unlock__7VIRFontR8VIRaster(arena, nullptr) == 0 && calls == 1 && flag == 0 && seen_self == arena;
    flag = 1; result = 7;
    ok = ok && Unlock__7VIRFontR8VIRaster(arena, nullptr) == 0 && calls == 2;
    flag = 1; result = -3;
    ok = ok && Unlock__7VIRFontR8VIRaster(arena, nullptr) == -1 && calls == 3 && flag == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
