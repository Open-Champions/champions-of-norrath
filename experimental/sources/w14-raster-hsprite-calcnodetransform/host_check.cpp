// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
// Every callee is stubbed; they log the order of calls and their arguments.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "calc_node_transform.cpp"
#include <cstdio>
#include <cstring>
static char order[32];
static int n;
static int timeValue, poolSize, lodValue, playStatus, upStatus, bindStatus;
static int levelSeen, indexSeen;
static const float *parentSeen, *mulA, *mulB;
static float *mulOut;
static float* dtorSeen[4];
static int dtorFlag[4], dtorCalls;
static float aFirst[16];
extern "C" void CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(unsigned char*, const float* parent, unsigned char*, float* out) {
    order[n++] = 'C'; parentSeen = parent; for (int i = 0; i < 16; ++i) { aFirst[i] = out[i]; out[i] = 1.0f + i; }
}
extern "C" int GetTime__7VIScene(unsigned char*) { order[n++] = 'T'; return timeValue; }
extern "C" int Size__Ct6VIPool1Z13VIHSpritePlay(const unsigned char*) { order[n++] = 'S'; return poolSize; }
extern "C" int CalcLODLevelIndex__9VIHSpriteRC10VIMatrix44(unsigned char*, const float*) { order[n++] = 'L'; return lodValue; }
extern "C" int ProcessPlayList__9VIHSpriteiR7VIScene(unsigned char*, int level, unsigned char*) { order[n++] = 'P'; levelSeen = level; return playStatus; }
extern "C" int ProcessUpHierarchy__9VIHSpritei(unsigned char*, int index) { order[n++] = 'U'; indexSeen = index; return upStatus; }
extern "C" int SetBindPose__9VIHSprite(unsigned char*) { order[n++] = 'B'; return bindStatus; }
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(float* out, const float* a, const float* b) { order[n++] = 'M'; mulOut = out; mulA = a; mulB = b; }
extern "C" void _$_10VIMatrix44(float* m, int flags) { order[n++] = 'D'; dtorSeen[dtorCalls] = m; dtorFlag[dtorCalls++] = flags; }
static void reset() { n = 0; dtorCalls = 0; std::memset(order, 0, sizeof order); }
int main() {
    alignas(16) static unsigned char sprite[512];
    float parent[16] = {}, out[16] = {};
    unsigned char scene[16];
    *reinterpret_cast<unsigned*>(sprite + 72) = arena_base + 0x100;
    *reinterpret_cast<long long*>(sprite + 200) = 5;
    float* node = reinterpret_cast<float*>(arena + 0x100 + 2 * 256 + 48);
    // same time: no processing at all
    timeValue = 5; reset();
    int r = CalcNodeTransform__9VIHSpriteiR10VIMatrix44R7VISceneT2(sprite, 2, parent, scene, out);
    bool ok = r == 0 && std::strcmp(order, "CTMDD") == 0 && parentSeen == parent && mulOut == out && mulA == node && mulB == dtorSeen[1] && dtorSeen[0] != dtorSeen[1] &&
              dtorFlag[0] == 2 && dtorFlag[1] == 2;
    for (int i = 0; i < 16; ++i) ok = ok && aFirst[i] == 0.0f;
    // different time with an empty pool: bind pose
    timeValue = 6; poolSize = 0; bindStatus = 0; reset();
    r = CalcNodeTransform__9VIHSpriteiR10VIMatrix44R7VISceneT2(sprite, 2, parent, scene, out);
    ok = ok && r == 0 && std::strcmp(order, "CTSBMDD") == 0;
    bindStatus = -1; reset();
    ok = ok && CalcNodeTransform__9VIHSpriteiR10VIMatrix44R7VISceneT2(sprite, 2, parent, scene, out) == -1 && std::strcmp(order, "CTSBDD") == 0;
    // pool with entries: LOD level, play list, hierarchy
    poolSize = 4; lodValue = 7; playStatus = 0; upStatus = 0; reset();
    r = CalcNodeTransform__9VIHSpriteiR10VIMatrix44R7VISceneT2(sprite, 2, parent, scene, out);
    ok = ok && r == 0 && std::strcmp(order, "CTSLPUMDD") == 0 && levelSeen == 7 && indexSeen == 2;
    playStatus = -1; reset();
    ok = ok && CalcNodeTransform__9VIHSpriteiR10VIMatrix44R7VISceneT2(sprite, 2, parent, scene, out) == -1 && std::strcmp(order, "CTSLPDD") == 0;
    playStatus = 0; upStatus = -2; reset();
    ok = ok && CalcNodeTransform__9VIHSpriteiR10VIMatrix44R7VISceneT2(sprite, 2, parent, scene, out) == -1 && std::strcmp(order, "CTSLPUDD") == 0;
    // the stored time is compared as a full 64-bit value
    upStatus = 0; *reinterpret_cast<long long*>(sprite + 200) = 0x100000006ll; reset();
    CalcNodeTransform__9VIHSpriteiR10VIMatrix44R7VISceneT2(sprite, 2, parent, scene, out);
    ok = ok && order[2] == 'S';
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
