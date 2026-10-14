// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "init.cpp"
#include <cstring>
static int dpRet, alRet, dpCalls, alCalls, delCalls, mcalls; static void* delSeen; static long mdst[8], msrc[8];
extern "C" int DestroyPool__t8VIVector1Z10VIMatrix44(void*) { ++dpCalls; return dpRet; }
extern "C" int Allocate__t8VIVector1Z10VIMatrix44(void* a) { ++alCalls; static_cast<unsigned*>(a)[2] = arena_base + 0x8000; return alRet; }
extern "C" void __builtin_vec_delete(void* p) { ++delCalls; delSeen = p; }
extern "C" void* __10VIMatrix44RC10VIMatrix44(void* d, const void* s) {
    mdst[mcalls] = static_cast<unsigned char*>(d) - arena; msrc[mcalls] = static_cast<const unsigned char*>(s) - arena; ++mcalls;
    std::memcpy(d, s, 64);
    return d;
}
int main() {
    unsigned vector[3] = {5, 6, 0}, source[3] = {3, 4, arena_base + 0x100};
    for (int i = 0; i < 512; ++i) arena[0x100 + i] = static_cast<unsigned char>(i * 7 + 3);
    std::memset(arena + 0x8000, 0xee, 0x400);
    if (Init__t8VIVector1Z10VIMatrix44RCt8VIVector1Z10VIMatrix44(vector, source) != 0) return 1;
    if (vector[0] != 3 || vector[1] != 4 || vector[2] != arena_base + 0x8000 || alCalls != 1 || dpCalls || delCalls) return 2;
    if (mcalls != 3 || mdst[0] != 0x8000 || msrc[0] != 0x100 || mdst[1] != 0x8000 + 64 || msrc[1] != 0x100 + 64 || mdst[2] != 0x8000 + 128) return 3;
    if (std::memcmp(arena + 0x100, arena + 0x8000, 192) != 0) return 4;
    if (arena[0x8000 + 192] != 0xee) return 5;
    // old storage released
    vector[2] = arena_base + 0x4000; mcalls = 0;
    if (Init__t8VIVector1Z10VIMatrix44RCt8VIVector1Z10VIMatrix44(vector, source) != 0 || dpCalls != 1 || delCalls != 1 || delSeen != arena + 0x4000) return 7;
    dpRet = -1; vector[2] = arena_base + 0x4000; alCalls = 0;
    if (Init__t8VIVector1Z10VIMatrix44RCt8VIVector1Z10VIMatrix44(vector, source) != -1 || alCalls) return 8;
    dpRet = 0; alRet = -2; vector[2] = 0;
    if (Init__t8VIVector1Z10VIMatrix44RCt8VIVector1Z10VIMatrix44(vector, source) != -1) return 9;
    alRet = 0; source[0] = static_cast<unsigned>(-3);
    if (Init__t8VIVector1Z10VIMatrix44RCt8VIVector1Z10VIMatrix44(vector, source) != 0 || vector[0] != static_cast<unsigned>(-3)) return 10;
    return 0;
}
