// Host contract for the AntWave constructor; not an EE input.
#include "ant_wave_ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char vt_AntWave[16];
static std::vector<std::string> events; static unsigned char* theObject; static unsigned flagsAtLink;
static short seenYaw; static float seenScale; static void* seenMap; static char** seenNames; static float seenPos[3];
void Creature_ctor(void* self, const float* p, short yaw, void* map, float scale, char** names) {
    events.push_back("creature"); std::memcpy(seenPos, p, 12); seenYaw = yaw; seenMap = map; seenScale = scale; seenNames = names;
    unsigned t = 1000, f = 0x10; float h = -7.0f;
    std::memcpy(static_cast<unsigned char*>(self) + 808, &t, 4); std::memcpy(static_cast<unsigned char*>(self) + 12, &f, 4);
    std::memcpy(static_cast<unsigned char*>(self) + 28, &h, 4);
}
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* l, void* d) {
    events.push_back("link");
    std::memcpy(&flagsAtLink, theObject + 12, 4);
    if (head != theObject + 968 || l != d) events.push_back("link-args-wrong");
}
void objectAddToRunList__FP10GameObject(void* o) { events.push_back(o == theObject ? "run" : "run-wrong"); }
int main() {
    int bad = 0;
    alignas(8) unsigned char object[1024]; alignas(8) unsigned char owner[64];
    std::memset(object, 0xaa, sizeof object); std::memset(owner, 0, sizeof owner);
    float ownerHeight = 40.0f; std::memcpy(owner + 28, &ownerHeight, 4);
    theObject = object;
    float pos[3] = {1, 2, 3};
    AntWave_ctor(object, owner, pos, 0x12345, 600);
    if (events.size() != 3 || events[0] != "creature" || events[1] != "link" || events[2] != "run") { std::puts("fail: events"); bad = 1; }
    if (seenYaw != 0x2345 || seenMap != nullptr || seenNames != nullptr || seenScale != 10000.0f || seenPos[2] != 3.0f) { std::puts("fail: creature args"); bad = 1; }
    unsigned w; float f;
    std::memcpy(&w, object + 960, 4); if (w != 1000) { std::puts("fail: +960"); bad = 1; }
    std::memcpy(&w, object + 964, 4); if (w != 1600) { std::puts("fail: +964"); bad = 1; }
    std::memcpy(&w, object + 12, 4); if (w != 0x11 || flagsAtLink != 0x11) { std::puts("fail: flags"); bad = 1; }
    std::memcpy(&w, object + 968, 4); if (w != 0) { std::puts("fail: list head"); bad = 1; }
    std::memcpy(&f, object + 28, 4); if (f != 50.0f) { std::puts("fail: +28"); bad = 1; }
    std::memcpy(&f, object + 176, 4); if (f != 100.0f) { std::puts("fail: +176"); bad = 1; }
    std::memcpy(&f, object + 168, 4); if (!(f > 0.5999f && f < 0.6001f)) { std::puts("fail: +168"); bad = 1; }
    std::memcpy(&w, object + 72, 4); if (w != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_AntWave))) { std::puts("fail: table"); bad = 1; }
    if (!bad) std::puts("AntWave ctor ok");
    return bad;
}
