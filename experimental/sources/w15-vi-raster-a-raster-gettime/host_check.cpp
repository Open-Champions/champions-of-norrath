// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "gettime.cpp"
#include <cstdio>
int main() {
    *reinterpret_cast<unsigned long long*>(arena + 17872) = 0x123456789abcdef0ull;
    bool ok = GetTime__8VIRaster(arena) == 0x123456789abcdef0ull;
    *reinterpret_cast<unsigned long long*>(arena + 17872) = 5;
    ok = ok && GetTime__8VIRaster(arena) == 5;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
