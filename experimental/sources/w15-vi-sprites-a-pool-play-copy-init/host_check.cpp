// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "init.cpp"
#include <cstring>
#include <initializer_list>
static int dpCalls, delCalls, alCalls, alRet; static void* delSeen;
extern "C" int DestroyPool__t6VIPool1Z13VIHSpritePlay(void*) { ++dpCalls; return -5; }
extern "C" int Allocate__t6VIPool1Z13VIHSpritePlay(void* p) { ++alCalls; static_cast<unsigned*>(p)[2] = arena_base + 0x8000; return alRet; }
extern "C" void __builtin_vec_delete(void* p) { ++delCalls; delSeen = p; }
int main() {
    const int stride = 104, nxt = 96, mark = 100;
    unsigned pool[6] = {7, 9, 0, 1, 2, 3}, source[6] = {4, 8, arena_base + 0x100, 1, 2, 77};
    // live chain 2 -> 0 -> end; free chain 1 -> 3 -> end
    for (int i = 0; i < stride * 4; ++i) arena[0x100 + i] = static_cast<unsigned char>(i * 5 + 1);
    auto src = [&](int i, int off) { return reinterpret_cast<int*>(arena + 0x100 + i * stride + off); };
    *src(2, nxt) = 0; *src(0, nxt) = -1; *src(1, nxt) = 3; *src(3, nxt) = -1;
    source[3] = 1; source[4] = 2;
    std::memset(arena + 0x8000, 0xee, stride * 4);
    if (Init__t6VIPool1Z13VIHSpritePlayRCt6VIPool1Z13VIHSpritePlay(pool, source) != 0) return 1;
    if (dpCalls || delCalls || alCalls != 1) return 2;
    if (pool[0] != 4 || pool[1] != 8 || pool[2] != arena_base + 0x8000 || pool[3] != 1 || pool[4] != 2 || pool[5] != 77) return 3;
    for (int i : {2, 0}) if (std::memcmp(arena + 0x100 + i * stride, arena + 0x8000 + i * stride, stride) != 0) return 4;
    for (int i : {1, 3}) {
        const int* d = reinterpret_cast<const int*>(arena + 0x8000 + i * stride);
        if (d[nxt / 4] != *src(i, nxt) || d[mark / 4] != -2) return 5;
        for (int b = 0; b < stride; ++b) if ((b < nxt || b >= mark + 4) && arena[0x8000 + i * stride + b] != 0xee) return 6;
    }
    // old storage released
    pool[2] = arena_base + 0x4000;
    if (Init__t6VIPool1Z13VIHSpritePlayRCt6VIPool1Z13VIHSpritePlay(pool, source) != 0 || dpCalls != 1 || delCalls != 1 || delSeen != arena + 0x4000) return 7;
    // empty live chain, empty free chain
    source[3] = static_cast<unsigned>(-1); source[4] = static_cast<unsigned>(-1); source[0] = 0; pool[2] = 0;
    std::memset(arena + 0x8000, 0xee, stride * 4);
    if (Init__t6VIPool1Z13VIHSpritePlayRCt6VIPool1Z13VIHSpritePlay(pool, source) != 0 || pool[0] != 0 || pool[3] != static_cast<unsigned>(-1) || arena[0x8000] != 0xee) return 8;
    alRet = -1;
    if (Init__t6VIPool1Z13VIHSpritePlayRCt6VIPool1Z13VIHSpritePlay(pool, source) != -1) return 9;
    return 0;
}
