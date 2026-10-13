// Host contract for playerRaisePlayer; not an EE input. Pointers kept in game memory are host-width
// here; the routine reads them through a copy helper so the offsets are the image's.
#include "player_raise_player.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
alignas(16) unsigned char player[4 * 23984];
int syncRandState;
static std::vector<std::string> events; static int localMask; static void* nextBlock; static int dummyBlock;
static unsigned char fakeObject[128], fakeVtable[64], newObject[128];
int netPlayerIsLocal__Fi(int p) { events.push_back("local:" + std::to_string(p)); return (localMask >> p) & 1; }
void gameNetKick__Fi(int v) { events.push_back("kick:" + std::to_string(v)); }
void* blockAlloc__Fi(int n) { events.push_back("alloc:" + std::to_string(n)); return nextBlock; }
void objectFindEmptySpot__FG6Point3iiPc(float* out, const float* in, int w, int h, char* f) {
    events.push_back("find:" + std::to_string(w) + ":" + std::to_string(h) + ":" + (f ? "flag" : "null") + ":" + std::to_string(in[0]));
    out[0] = in[0] + 1; out[1] = in[1] + 2; out[2] = in[2] + 3;
}
static float ctorSpot[3]; static short ctorYaw; static int ctorPlayer, ctorExtra, ctorState;
void __6PlayerG6Point3sii(void* self, float* pos, short yaw, int pn, int extra) {
    events.push_back("ctor"); std::memcpy(ctorSpot, pos, 12); ctorYaw = yaw; ctorPlayer = pn; ctorExtra = extra; ctorState = syncRandState;
    unsigned char* o = static_cast<unsigned char*>(self);
    std::memcpy(player + pn * 23984 + 21872, &o, sizeof o);
}
void resurectEffect__6Player(void* o) { events.push_back(o == newObject ? "resurect:new" : o ? "resurect:old" : "resurect:null"); }
static int virtCalls; static void* virtThis; static int virtArg;
static void virtTarget(void* self, int a) { events.push_back("virt"); ++virtCalls; virtThis = self; virtArg = a; }
int main() {
    int bad = 0;
    float pos[3] = {10.0f, 20.0f, 30.0f};
    // existing object with this-adjust 16, a successful allocation, local player 2
    unsigned char* vt = fakeVtable; unsigned char* ob = fakeObject;
    std::memcpy(fakeObject + 72, &vt, sizeof vt);
    short adjust = 16; std::memcpy(fakeVtable + 8, &adjust, 2);
    void (*fn)(void*, int) = virtTarget; std::memcpy(fakeVtable + 12, &fn, sizeof fn);
    std::memcpy(player + 2 * 23984 + 21872, &ob, sizeof ob);
    float f176 = 7.0f; std::memcpy(player + 2 * 23984 + 176, &f176, 4);
    localMask = 4; nextBlock = newObject; syncRandState = 1;
    playerRaisePlayer__FiG6Point3(2, pos);
    const char* want[] = {"local:2", "kick:1", "virt", "alloc:1408", "find:96:24:null:10.000000", "ctor", "resurect:new"};
    if (events.size() != 7) { std::printf("fail: event count %zu\n", events.size()); bad = 1; }
    for (size_t i = 0; i < events.size() && i < 7; ++i) if (events[i] != want[i]) { std::printf("fail: event %zu %s\n", i, events[i].c_str()); bad = 1; }
    if (virtThis != ob + 16 || virtArg != 3) { std::puts("fail: virtual call"); bad = 1; }
    unsigned state = 1u * 1664525u + 1013904223u;
    if (syncRandState != static_cast<int>(state) || ctorState != syncRandState || ctorYaw != static_cast<short>(static_cast<int>(state) >> 16) || ctorPlayer != 2 || ctorExtra != 0) { std::puts("fail: ctor args"); bad = 1; }
    if (ctorSpot[0] != 11 || ctorSpot[1] != 22 || ctorSpot[2] != 33) { std::puts("fail: spot"); bad = 1; }
    float f172; std::memcpy(&f172, player + 2 * 23984 + 172, 4);
    if (f172 != 3.5f) { std::puts("fail: half"); bad = 1; }
    // no object, failed allocation, not local: only the allocation and the final steps
    std::memset(player + 2 * 23984 + 21872, 0, sizeof(void*)); events.clear(); nextBlock = nullptr; localMask = 0; syncRandState = 5;
    playerRaisePlayer__FiG6Point3(2, pos);
    const char* want2[] = {"local:2", "alloc:1408", "resurect:null"};
    if (events.size() != 3) { std::puts("fail: second count"); bad = 1; }
    for (size_t i = 0; i < events.size() && i < 3; ++i) if (events[i] != want2[i]) { std::printf("fail: second %zu %s\n", i, events[i].c_str()); bad = 1; }
    if (syncRandState != 5) { std::puts("fail: state touched"); bad = 1; }
    if (!bad) std::puts("playerRaisePlayer ok");
    return bad;
}
