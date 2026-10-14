// Host contract for setCamera__Ff. Not an EE plan input. Build with -DNORRATH_HOST_CAMERA (actor
// slots hold 32-bit arena offsets). evalHermite is a linear stub that records its arguments, iatan2
// is a recorded integer stub, and the engine calls are recorders.
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
struct Action { float time; short category; short kind; int arg[3]; };
Action* action = nullptr;
int nmActions = 0;
int displayActionPos = 0;
float displayCutPos = 0.0f;
short myYaw = 0, myPitch = 0, myRoll = 0;
float myFOV = 0.0f;
unsigned char* actor = nullptr;
unsigned world = 0x1234;
alignas(16) unsigned char arena[0x1000];
unsigned char* norrath_camera_arena = arena;
extern "C" unsigned char worldCameraPos[12];
unsigned char worldCameraPos[12];
extern "C" float cutCameraPos[3];
float cutCameraPos[3];

struct Hermite { float u, p0, p1, m0, m1; };
static std::vector<Hermite> hermite_calls;
static std::string events;
static int color_args[4], blur_arg = -1, switch_arg = -1;
static float trans_args[3];
static short rot_angles[2];
static unsigned precache_world = 0;

extern "C" {
float evalHermite__Ffffff(float u, float p0, float p1, float m0, float m1) {
    hermite_calls.push_back({u, p0, p1, m0, m1});
    return p0 + (p1 - p0) * u;
}
int iatan2__Fff(float y, float x) { return static_cast<int>(y * 100.0f + x); }
void matIdent__FP8Matrix34(float*) { events += "I"; }
void matRot__FP8Matrix34is(float*, int axis, short angle) { events += axis == 0 ? "R0" : "R2"; rot_angles[axis == 0 ? 0 : 1] = angle; }
void matTrans__FP8Matrix34fff(float*, float x, float y, float z) { events += "T"; trans_args[0] = x; trans_args[1] = y; trans_args[2] = z; }
void worldPrecacheWorld__FP12_worldHeaderP8Matrix34(void* w, float*) { events += "P"; precache_world = static_cast<unsigned>(reinterpret_cast<unsigned long>(w)); }
void coreStartSwitchScene__Fi(int s) { events += "S"; switch_arg = s; }
void engineSetScreenColor__Fiiii(int r, int g, int b, int a) { events += "C"; color_args[0] = r; color_args[1] = g; color_args[2] = b; color_args[3] = a; }
void engineSetBlur__Fi(int a) { events += "B"; blur_arg = a; }
void setCamera__Ff(float);
}

static Action list[32];
static void clear() {
    std::memset(list, 0, sizeof list);
    nmActions = 0; displayActionPos = 0; displayCutPos = -1.0f; hermite_calls.clear(); events.clear();
    myYaw = 11; myPitch = 22; myRoll = 33; myFOV = 1.0f;
    action = list;
    float cam[3] = {5.0f, 6.0f, 7.0f};
    std::memcpy(worldCameraPos, cam, 12);
}
static void key(int i, float t, int kind, int a = 0, int b = 0, int c = 0, int cat = -100) {
    list[i].time = t; list[i].category = short(cat); list[i].kind = short(kind);
    list[i].arg[0] = a; list[i].arg[1] = b; list[i].arg[2] = c;
}

int main() {
    actor = arena;
    // 1. Nothing listed: defaults and the colour and blur calls.
    clear();
    setCamera__Ff(1.0f);
    if (myYaw != -8192 || myPitch != 10000 || myRoll != 0 || myFOV != 50.0f) return 1;
    if (events != "CB" || color_args[0] != 0 || color_args[3] != 0 || blur_arg != 0 || !hermite_calls.empty()) return 2;

    // 2. Two position keys: no prev or after, so both tangents are zero.
    clear();
    key(0, 1.0f, 0, 0, 0, 0); key(1, 3.0f, 0, 100, 200, 300);
    nmActions = 2;
    setCamera__Ff(2.0f);
    if (hermite_calls.size() != 3 || hermite_calls[0].u != 0.5f || hermite_calls[0].m0 != 0.0f || hermite_calls[0].m1 != 0.0f) return 3;
    if (hermite_calls[1].p1 != 200.0f || hermite_calls[2].p1 != 300.0f) return 4;
    if (cutCameraPos[0] != 50.0f || cutCameraPos[1] != 100.0f || cutCameraPos[2] != 150.0f) return 5;
    // Past the last key only the cur key is used and its position is copied.
    clear();
    key(0, 1.0f, 0, 7, 8, 9); nmActions = 1;
    setCamera__Ff(5.0f);
    if (cutCameraPos[0] != 7.0f || cutCameraPos[1] != 8.0f || cutCameraPos[2] != 9.0f || !hermite_calls.empty()) return 6;

    // 3. Four keys: non-zero tangents 15 and 25 (see the header formulas).
    clear();
    key(0, 0.0f, 0, 0, 0, 0); key(1, 1.0f, 0, 10, 0, 0); key(2, 2.0f, 0, 30, 0, 0); key(3, 3.0f, 0, 60, 0, 0);
    nmActions = 4;
    setCamera__Ff(1.5f);
    if (hermite_calls.size() != 3 || hermite_calls[0].m0 != 15.0f || hermite_calls[0].m1 != 25.0f || hermite_calls[0].p0 != 10.0f) return 7;
    // A kind 4 mark at the cur time zeroes the incoming tangent.
    clear();
    key(0, 0.0f, 0, 0, 0, 0); key(1, 1.0f, 0, 10, 0, 0); key(2, 1.0f, 4); key(3, 2.0f, 0, 30, 0, 0); key(4, 3.0f, 0, 60, 0, 0);
    nmActions = 5;
    setCamera__Ff(1.5f);
    if (hermite_calls[0].m0 != 0.0f || hermite_calls[0].m1 != 25.0f) return 8;

    // 4. Yaw keys that straddle the turn: the end angle is shifted by 65536.
    clear();
    key(0, 0.0f, 1, 60000); key(1, 1.0f, 1, 0); nmActions = 2;
    setCamera__Ff(0.5f);
    if (hermite_calls.size() != 1 || hermite_calls[0].p0 != 60000.0f || hermite_calls[0].p1 != 65536.0f) return 9;
    if (myYaw != static_cast<short>(62768)) return 10;
    // Only one yaw key: its low 16 bits are used. Pitch, roll and field of view defaults are untouched by absent keys.
    clear();
    key(0, 0.0f, 1, 70000); nmActions = 1;
    setCamera__Ff(1.0f);
    if (myYaw != static_cast<short>(70000 & 0xffff) || myPitch != 10000) return 11;
    clear();
    key(0, 0.0f, 8, 3); key(1, 0.0f, 9, 70); nmActions = 2;
    setCamera__Ff(1.0f);
    if (myRoll != 3 * 182 || myFOV != 70.0f) return 12;

    // 5. A cut marker at the next key: the position holds at the cur key, the world is precached from the
    // old camera position, and a marker near the cur key's time starts scene switch 4 once.
    clear();
    key(0, 1.0f, 0, 10, 20, 30); key(1, 1.0f, 5); key(2, 2.0f, 0, 100, 200, 300); key(3, 2.0f, 5);
    nmActions = 4;
    setCamera__Ff(1.02f);
    if (hermite_calls.empty() || hermite_calls[0].p1 != 10.0f) return 13;
    if (events != "CBSIR0R2TP") { std::fprintf(stderr, "events %s\n", events.c_str()); return 14; }
    if (switch_arg != 4 || displayCutPos != 1.0f) return 15;
    if (trans_args[0] != -100.0f || trans_args[1] != -200.0f || trans_args[2] != -300.0f) return 16;
    if (rot_angles[0] != static_cast<short>(22 + 16339) || rot_angles[1] != 11) return 21;   // the entry pitch and yaw
    if (precache_world != 0x1234u) return 17;
    events.clear(); switch_arg = -1;
    setCamera__Ff(1.02f);                      // the marker already fired
    if (switch_arg != -1) return 18;

    // 6. Look-at key toward actor 0 at (10, 20, 30) with height term 8: yaw = 16384 - iatan2(20, 10).
    clear();
    const float position[3] = {10.0f, 20.0f, 30.0f}, height = 8.0f;
    std::memcpy(arena + 0x100 + 16, position, 12);
    std::memcpy(arena + 0x100 + 32, &height, 4);
    unsigned off = 0x100;
    std::memcpy(arena, &off, 4);               // slot 0 points at the object
    key(0, 0.0f, 3, 0); nmActions = 1;
    cutCameraPos[0] = cutCameraPos[1] = cutCameraPos[2] = 0.0f;
    setCamera__Ff(0.0f);
    if (myYaw != 16384 - 2010) { std::fprintf(stderr, "yaw %d\n", myYaw); return 19; }
    if (myPitch != -3422) { std::fprintf(stderr, "pitch %d\n", myPitch); return 20; }
    std::printf("set camera contract passed\n");
    return 0;
}
