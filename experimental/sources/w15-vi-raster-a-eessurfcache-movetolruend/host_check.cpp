// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
static unsigned char scratch[1 << 12];
extern "C" unsigned char* norrath_host_pointer(unsigned address) {
    if (address >= 0x70000000u) return scratch + (address - 0x70000000u);
    return arena + (address - arena_base);
}
#include "move_to_lru_end.cpp"
#include <cstdio>
#include <cstring>
static int& F(int i, int f) { return *reinterpret_cast<int*>(arena + 0x6000 + i * 104 + f); }
static int& H() { return *reinterpret_cast<int*>(arena + 8); }
static int& T() { return *reinterpret_cast<int*>(arena + 12); }
static void build() {
    std::memset(arena, 0, sizeof arena);
    *reinterpret_cast<unsigned*>(arena + 64) = arena_base + 0x6000;
    H() = 2; T() = 3;   // 2 -> 0 -> 3
    F(2, 92) = 0; F(2, 96) = -1; F(0, 92) = 3; F(0, 96) = 2; F(3, 92) = -1; F(3, 96) = 0;
}
int main() {
    bool ok = true;
    build(); ok = ok && MoveToLRUEnd__13VIEESurfCachei(arena, 3) == 0 && H() == 2 && T() == 3 && F(3, 96) == 0;   // already last
    build(); ok = ok && MoveToLRUEnd__13VIEESurfCachei(arena, 0) == 0 && H() == 2 && T() == 0 && F(2, 92) == 3 && F(3, 96) == 2 && F(3, 92) == 0 && F(0, 92) == -1 && F(0, 96) == 3;
    build(); ok = ok && MoveToLRUEnd__13VIEESurfCachei(arena, 2) == 0 && H() == 0 && T() == 2 && F(0, 96) == -1 && F(3, 92) == 2 && F(2, 92) == -1 && F(2, 96) == 3;
    build(); H() = -1; T() = -1; ok = ok && MoveToLRUEnd__13VIEESurfCachei(arena, 1) == 0 && H() == 1 && T() == 1 && F(1, 92) == -1 && F(1, 96) == -1;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
