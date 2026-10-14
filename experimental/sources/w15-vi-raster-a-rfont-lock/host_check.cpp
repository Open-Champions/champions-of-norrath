// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "lock.cpp"
#include <cstdio>
int main() {
    int& flag = *reinterpret_cast<int*>(arena + 8);
    flag = 0;
    bool ok = Lock__7VIRFontR8VIRaster(arena, nullptr) == 0 && flag == 1;
    ok = ok && Lock__7VIRFontR8VIRaster(arena, nullptr) == -1 && flag == 1;
    flag = 5;
    ok = ok && Lock__7VIRFontR8VIRaster(arena, nullptr) == -1 && flag == 5;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
