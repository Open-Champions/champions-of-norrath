// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
static int lockResult, unlockResult, storeOut, pitchOut, calls, unlocks;
extern "C" unsigned char* Surface__8VIRasteri(unsigned char*, int) { return arena + 0x100; }
extern "C" int LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc(unsigned char*, int, int* pitch, int* store, unsigned char** buffer) {
    ++calls; *pitch = pitchOut; *store = storeOut; *buffer = nullptr; return lockResult;
}
extern "C" int UnlockMipLevel__9VISurfacei(unsigned char*, int) { ++unlocks; return unlockResult; }
#include "init_slots.cpp"
#include <cstdio>
static int& I(unsigned off) { return *reinterpret_cast<int*>(arena + off); }
static int S(int slot, int off) { return I(0x1000 + 56 * slot + off); }
int main() {
    I(12) = 3; I(20) = 4; I(24) = 2; I(68) = arena_base + 0x1000;
    I(0x100 + 8) = 8; I(0x100 + 12) = 8;
    lockResult = 0; unlockResult = 0; storeOut = 1; pitchOut = 10;
    bool ok = InitSlots__7VIRFontR8VIRaster(arena, arena) == 0 && calls == 1 && unlocks == 1 && I(44) == 4 && I(56) == 0 && I(60) == 3;
    const int xs[4] = {0, 4, 0, 4}, ys[4] = {0, 0, 4, 4}, offs[4] = {0, 4, 40, 44};
    for (int k = 0; k < 4; ++k)
        ok = ok && S(k, 16) == xs[k] && S(k, 20) == ys[k] && S(k, 0) == offs[k] && S(k, 8) == (k ? k - 1 : -1) && S(k, 4) == (k < 3 ? k + 1 : -1)
                && *reinterpret_cast<unsigned short*>(arena + 0x1000 + 56 * k + 12) == 0xffff;
    storeOut = 4;  // 3 * cell per step
    ok = ok && InitSlots__7VIRFontR8VIRaster(arena, arena) == 0 && S(1, 0) == 12 && S(3, 0) == 40 + 12;
    storeOut = 7;  // unsupported: -1, surface left locked
    unlocks = 0;
    ok = ok && InitSlots__7VIRFontR8VIRaster(arena, arena) == -1 && unlocks == 0;
    storeOut = 0; I(20) = 3; I(24) = 1;  // cell 3 rounds up to 4, half step 2
    ok = ok && InitSlots__7VIRFontR8VIRaster(arena, arena) == 0 && I(44) == 4 && S(1, 0) == 2;
    storeOut = 1; I(20) = 4; lockResult = -1; calls = 0;
    I(44) = 99;
    ok = ok && InitSlots__7VIRFontR8VIRaster(arena, arena) == -1 && I(44) == 99;
    lockResult = 0; unlockResult = -1;
    ok = ok && InitSlots__7VIRFontR8VIRaster(arena, arena) == -1;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
