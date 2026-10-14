#include "world_ctor.cpp"

#include <cstring>

static int order[8];
static char* seen[8];
static int n;
extern "C" void __11VIWorldTree(void* p) { order[n] = 1; seen[n++] = static_cast<char*>(p); }
extern "C" void __t7VIArray1Z6VIZone(void* p) { order[n] = 2; seen[n++] = static_cast<char*>(p); }
extern "C" void __t7VIArray1Z13VIWorldRegion(void* p) { order[n] = 3; seen[n++] = static_cast<char*>(p); }

int main() {
    alignas(8) static unsigned char object[1200];
    std::memset(object, 0x5a, sizeof object);
    __7VIWorld(object);
    if (n != 3 || order[0] != 1 || order[1] != 2 || order[2] != 3) return 1;
    if (seen[0] != reinterpret_cast<char*>(object) + 52 || seen[1] != reinterpret_cast<char*>(object) + 72 ||
        seen[2] != reinterpret_cast<char*>(object) + 80) return 2;
    unsigned char expect[1200];
    std::memset(expect, 0x5a, sizeof expect);
    auto w = [&](int off, unsigned v) { std::memcpy(expect + off, &v, 4); };
    const unsigned neg = 0xc47a0000u, pos = 0x447a0000u;
    for (int i = 0; i < 3; ++i) { w(4 * i, neg); w(12 + 4 * i, pos); w(36 + 4 * i, 0); }
    w(48, 4); w(28, 1); w(32, 0x44fa0000u); w(24, 1);
    for (int i = 0; i < 22; ++i) for (int k = 0; k < 4; ++k) w(96 + 40 * i + 4 * k, 0);
    return std::memcmp(object, expect, sizeof object) == 0 ? 0 : 3;
}
