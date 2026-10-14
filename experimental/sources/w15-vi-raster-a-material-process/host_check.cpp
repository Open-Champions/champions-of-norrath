// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
static unsigned char scratch[1 << 12];
extern "C" unsigned char* norrath_host_pointer(unsigned address) {
    if (address >= 0x70000000u) return scratch + (address - 0x70000000u);
    return arena + (address - arena_base);
}
#include "process.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
static float& F(int layer, int off) { return *reinterpret_cast<float*>(arena + layer * 144 + off); }
static int& I(int layer, int off) { return *reinterpret_cast<int*>(arena + layer * 144 + off); }
int main() {
    bool ok = true;
    std::memset(arena, 0, sizeof arena);
    *reinterpret_cast<int*>(arena + 8) = 3;
    *reinterpret_cast<long long*>(arena + 24) = 1000;
    I(0, 48) = 0; F(0, 104) = 0.5f; F(0, 108) = 10.0f; F(0, 112) = 0.25f; F(0, 116) = 0.0f;
    I(1, 48) = -1; F(1, 104) = 5.0f; F(1, 112) = 0.5f;
    I(2, 48) = 2; F(2, 104) = -1000.0f; F(2, 108) = 0.0f; F(2, 112) = 0.0f; F(2, 116) = -0.5f;
    ok = ok && Process__10VIMateriall(arena, 1000) == 0 && F(0, 112) == 0.25f;   // same time: no change
    ok = ok && Process__10VIMateriall(arena, 3000) == 0;                          // dt = 2000 * ~0.001 = ~2.0
    const float dt = 2000.0f * __builtin_bit_cast(float, 0x3a83126eu);
    auto wrap = [](float x) { return (x > 1.0f || x < -1.0f) ? x - static_cast<float>(static_cast<int>(x)) : x; };
    float u0 = wrap(0.25f + 0.5f * dt), v0 = wrap(0.0f + 10.0f * dt);
    float u2 = wrap(0.0f + (-1000.0f) * dt);
    ok = ok && F(0, 112) == u0 && F(0, 116) == v0 && F(1, 112) == 0.5f && F(2, 112) == u2 && F(2, 116) == -0.5f
        && *reinterpret_cast<long long*>(arena + 24) == 3000;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
