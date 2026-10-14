// Host check only; not a build input.
#include "process.cpp"
#include <cstring>
static char order[16]; static int n, timeCalls;
static long long t1, t2; static int size, plRet, phRet, bpRet, levelSeen;
extern "C" long long GetTime__7VIScene(void*) { order[n++] = 'T'; return timeCalls++ ? t2 : t1; }
extern "C" int Size__Ct6VIPool1Z13VIHSpritePlay(const void*) { order[n++] = 'S'; return size; }
extern "C" int ProcessPlayList__9VIHSpriteiR7VIScene(void*, int level, void*) { order[n++] = 'P'; levelSeen = level; return plRet; }
extern "C" int ProcessHierarchy__9VIHSprite(void*) { order[n++] = 'H'; return phRet; }
extern "C" int SetBindPose__9VIHSprite(void*) { order[n++] = 'B'; return bpRet; }
static void reset() { n = 0; timeCalls = 0; std::memset(order, 0, sizeof order); }
int main() {
    alignas(8) unsigned char sprite[256] = {};
    char scene[4];
    long long stored = 5;
    std::memcpy(sprite + 200, &stored, 8);
    t1 = 5; t2 = 5; reset();
    if (Process__9VIHSpriteiR7VIScene(sprite, 3, scene) != 0 || std::strcmp(order, "TT")) return 1;
    t1 = 6; t2 = 7; size = 2; reset();
    if (Process__9VIHSpriteiR7VIScene(sprite, 3, scene) != 0 || std::strcmp(order, "TSPHT") || levelSeen != 3) return 2;
    std::memcpy(&stored, sprite + 200, 8);
    if (stored != 7) return 3;
    size = 0; t1 = 8; reset();
    if (Process__9VIHSpriteiR7VIScene(sprite, 3, scene) != 0 || std::strcmp(order, "TSBT")) return 4;
    bpRet = -1; reset(); std::memcpy(sprite + 200, &(stored = 1), 8);
    if (Process__9VIHSpriteiR7VIScene(sprite, 3, scene) != -1 || std::strcmp(order, "TSB")) return 5;
    size = 1; plRet = -1; reset();
    if (Process__9VIHSpriteiR7VIScene(sprite, 3, scene) != -1 || std::strcmp(order, "TSP")) return 6;
    plRet = 0; phRet = -3; reset();
    if (Process__9VIHSpriteiR7VIScene(sprite, 3, scene) != -1 || std::strcmp(order, "TSPH")) return 7;
    std::memcpy(&stored, sprite + 200, 8);
    if (stored != 1) return 8;
    t1 = (1ll << 32) + 1; stored = 1; std::memcpy(sprite + 200, &stored, 8); phRet = 0; reset();
    if (Process__9VIHSpriteiR7VIScene(sprite, 3, scene) != 0 || std::strcmp(order, "TSPHT")) return 9;
    return 0;
}
