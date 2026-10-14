// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "process_weapon_trails.cpp"
#include <cstring>
#include <initializer_list>
enum { ATTACH_ID = 77, GROUP = 0x4000, MEMBERS = 0x5000, TABLE = 0x6000 };
static long long timeNow[2], timeCalls; static int memberTypes[8], memberKinds[8], attachType, beginRet;
static int dtors; static float* dtorSeen[4];
static int trailEnds, trailEndArg, begins, segments; static long long beginTime; static int beginCount; static float beginValue;
static float segA[3], segB[3], segColor[4], segColor2[4]; static int segHandle;
static float* lastMemberOut; static int muls, m4Muls; static const void* recordSeen[8]; static int membersDone[8], membersN;
static int gat;
extern "C" long long GetTime__7VIScene(void*) { return timeNow[timeCalls++ > 0 ? 1 : 0]; }
extern "C" void VITrailFxEnd__FP9VITrailFx(int t) { ++trailEnds; trailEndArg = t; }
extern "C" int VITrailFxBegin__Flif(long long t, int n, float v) { ++begins; beginTime = t; beginCount = n; beginValue = v; return beginRet; }
extern "C" void AddSegment__9VITrailFxRC7VIVect3T1RC10VIColor32FT3(int h, const float* a, const float* b, const void* c, const float* c2) {
    ++segments; segHandle = h; std::memcpy(segA, a, 12); std::memcpy(segB, b, 12); std::memcpy(segColor, c, 16); std::memcpy(segColor2, c2, 16);
}
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(void* self, const void*, const void* b) {
    ++muls;
    float* d = static_cast<float*>(self);
    if (d == lastMemberOut) { ++m4Muls; d[12] = 1000.0f * m4Muls; d[13] = 1000.0f * m4Muls + 1; d[14] = 1000.0f * m4Muls + 2; }
    else if (lastMemberOut) recordSeen[m4Muls] = b;
}
extern "C" void CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(void*, const void*, void*, void*) {}
extern "C" int GetAttachSprite__C9VIHSpritei(const void*, int n) { gat = n; return ATTACH_ID; }
extern "C" int GetSpriteType__7VIScenei(void*, int id) { return id == ATTACH_ID ? attachType : memberTypes[id - 100]; }
extern "C" unsigned char* Sprite__7VIScenei(void*, int id) { return id == ATTACH_ID ? arena + GROUP : arena + MEMBERS + 0x100 * (id - 100); }
extern "C" void CalcMemberTransform__13VIGroupSpriteiR10VIMatrix44(void*, int i, void* out) { lastMemberOut = static_cast<float*>(out); membersDone[membersN++] = i; }
extern "C" void _$_10VIMatrix44(void* self, int flags) { if (flags != 2) dtors += 100; dtorSeen[dtors < 4 ? dtors : 3] = static_cast<float*>(self); ++dtors; }
static void put(unsigned char* b, int off, int v) { std::memcpy(b + off, &v, 4); }
static void reset() {
    std::memset(arena, 0, sizeof arena);
    dtors = trailEnds = begins = segments = muls = m4Muls = membersN = 0; lastMemberOut = 0; timeCalls = 0; gat = -9;
    timeNow[0] = 5000; timeNow[1] = 6000; attachType = 11; beginRet = 0x1234;
    for (int i = 0; i < 8; ++i) { memberTypes[i] = 2; memberKinds[i] = i % 2; }
}
int main() {
    alignas(8) static unsigned char o[8192], camera[512], scene[64], matrix[64];
    auto setup = [&](int enable, long long last, int trail, int cur, int target, int sel, int attach, int count) {
        std::memset(o, 0, sizeof o);
        put(o, 4544, enable); std::memcpy(o + 4568, &last, 8); put(o, 4576, trail); put(o, 4580, cur); put(o, 4584, target); put(o, 4492, sel);
        for (int k = 0; k < 6; ++k) put(o, 4400 + 4 * k, attach);
        put(o, 72, arena_base + TABLE);
        for (int s = 0; s < 3; ++s) put(o, 4364 + 12 * s, s + 1);
        put(arena + GROUP, 64, count);
        for (int i = 0; i < 8; ++i) { put(arena + GROUP, 68 + 32 * i, 100 + i); put(arena + MEMBERS + 0x100 * i, 64, memberKinds[i]); }
    };
    // disabled
    reset(); setup(0, 0, 0, -1, -1, 22, 5, 4);
    if (ProcessWeaponTrails__9VICSpriteR8VICameraR7VISceneR10VIMatrix44(o, camera, scene, matrix) != 0 || dtors != 4 || timeCalls != 0) return 1;
    // too soon
    reset(); setup(1, 4960, 0, -1, -1, 22, 5, 4);
    if (ProcessWeaponTrails__9VICSpriteR8VICameraR7VISceneR10VIMatrix44(o, camera, scene, matrix) != 0 || dtors != 4 || begins) return 2;
    // 50 ticks exactly is enough; unknown selector returns 0 after the old trail ended
    reset(); setup(1, 4950, 0x55, 3, 4, 31, 5, 4);
    if (ProcessWeaponTrails__9VICSpriteR8VICameraR7VISceneR10VIMatrix44(o, camera, scene, matrix) != 0 || dtors != 4 || trailEnds != 1 || trailEndArg != 0x55) return 3;
    { int t; std::memcpy(&t, o + 4576, 4); if (t != 0) return 4; }
    // begin fails
    reset(); setup(1, 0, 0, -1, 4, 23, 5, 4); beginRet = 0;
    if (ProcessWeaponTrails__9VICSpriteR8VICameraR7VISceneR10VIMatrix44(o, camera, scene, matrix) != -1 || dtors != 4 || begins != 1) return 5;
    if (beginTime != 6000 || beginCount != 25 || beginValue != 0.39999998f) return 6;
    // full path: slot from selector 30 (2) and members 0,1 give a segment
    reset(); setup(1, 0, 0, -1, 4, 30, 5, 4);
    if (ProcessWeaponTrails__9VICSpriteR8VICameraR7VISceneR10VIMatrix44(o, camera, scene, matrix) != 0 || dtors != 4) return 7;
    if (gat != 5 || begins != 1 || segments != 1 || segHandle != 0x1234 || membersN != 2) return 8;
    if (segA[0] != 1000.0f || segA[2] != 1002.0f || segB[0] != 2000.0f || segB[1] != 2001.0f) return 9;
    if (segColor2[3] != 1.0f || segColor2[0] != 0.0f) return 10;
    { int cur; std::memcpy(&cur, o + 4580, 4); if (cur != 4) return 11; }
    if (dtorSeen[0] != lastMemberOut) return 12;
    // wrong sprite type / attachment -1 / member types other than 2 / count zero: no segment
    reset(); setup(1, 0, 0, -1, 4, 24, 5, 4); attachType = 3;
    if (ProcessWeaponTrails__9VICSpriteR8VICameraR7VISceneR10VIMatrix44(o, camera, scene, matrix) != 0 || segments || dtors != 4) return 13;
    reset(); setup(1, 0, 0, -1, 4, 24, -1, 4);
    if (ProcessWeaponTrails__9VICSpriteR8VICameraR7VISceneR10VIMatrix44(o, camera, scene, matrix) != 0 || segments || gat != -9 || dtors != 4) return 14;
    reset(); setup(1, 0, 0, -1, 4, 24, 5, 4); for (int i = 0; i < 8; ++i) memberTypes[i] = 5;
    if (ProcessWeaponTrails__9VICSpriteR8VICameraR7VISceneR10VIMatrix44(o, camera, scene, matrix) != 0 || segments || membersN) return 15;
    // two members of kind 0: counter reaches 2, so a segment is added with b still zero
    reset(); setup(1, 0, 0, 4, 4, 40, 5, 4); for (int i = 0; i < 8; ++i) memberKinds[i] = 0;
    setup(1, 0, 0, 4, 4, 40, 5, 4);
    if (ProcessWeaponTrails__9VICSpriteR8VICameraR7VISceneR10VIMatrix44(o, camera, scene, matrix) != 0 || segments != 1 || begins != 0 || segB[0] != 0.0f || segA[0] != 2000.0f) return 16;
    // only one usable member: no segment
    reset(); setup(1, 0, 0, 4, 4, 41, 5, 1);
    if (ProcessWeaponTrails__9VICSpriteR8VICameraR7VISceneR10VIMatrix44(o, camera, scene, matrix) != 0 || segments != 0 || membersN != 1) return 17;
    return 0;
}
