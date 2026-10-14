// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 14];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "init.cpp"
#include <cstring>
static int calcRet, allocRet, calcCalls, allocCalls;
static unsigned snapshot[16];
extern "C" int CalcDataSize__13VIHSpriteAnim(void* o) { ++calcCalls; return calcRet; }
extern "C" int Allocate__13VIHSpriteAnim(void* o) {
    ++allocCalls;
    unsigned* w = static_cast<unsigned*>(o);
    std::memcpy(snapshot, w, sizeof snapshot);
    w[13] = arena_base + 0x1000;
    return allocRet;
}
int main() {
    alignas(8) unsigned w[16];
    const char* S = "Init__13VIHSpriteAnim19VIHSpriteAnimFormatiiiff14VIPlaybackType";
    (void)S;
    std::memset(w, 0, sizeof w);
    w[9] = 0x40; w[10] = 0x100; w[11] = 0x180;
    int r = Init__13VIHSpriteAnim19VIHSpriteAnimFormatiiiff14VIPlaybackType(w, 1, 4, 3, 9, 1.5f, 2.5f, 7);
    if (r != 0 || calcCalls != 1 || allocCalls != 1) return 1;
    if (snapshot[2] != 1 || snapshot[3] != 4 || snapshot[4] != 3 || snapshot[5] != 9 || snapshot[8] != 7) return 2;
    float a, b; std::memcpy(&a, &snapshot[6], 4); std::memcpy(&b, &snapshot[7], 4);
    if (a != 1.5f || b != 2.5f) return 3;
    if (w[9] != arena_base + 0x1040 || w[10] != arena_base + 0x1100 || w[11] != arena_base + 0x1180) return 4;
    for (int i = 0; i < 4; ++i) if (*reinterpret_cast<int*>(arena + 0x1040 + i * 8 + 4) != i * 3) return 5 + i;
    if (*reinterpret_cast<int*>(arena + 0x1040 + 4 * 8 + 4) != 0) return 9;
    calcRet = -1; allocCalls = 0;
    if (Init__13VIHSpriteAnim19VIHSpriteAnimFormatiiiff14VIPlaybackType(w, 0, 1, 1, 1, 0, 0, 0) != -1 || allocCalls) return 10;
    calcRet = 0; allocRet = -1;
    if (Init__13VIHSpriteAnim19VIHSpriteAnimFormatiiiff14VIPlaybackType(w, 0, 1, 1, 1, 0, 0, 0) != -1 || !allocCalls) return 11;
    return 0;
}
