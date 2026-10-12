// Host contract for drawAutoMap__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>

#include "draw_auto_map.cpp"

signed char netNetworkMode = 0;
int validLocalPlayer = 0;
alignas(16) unsigned char player[4 * 23984];
AutoMapWorld* world;
signed char cutMode = 0;
int nmPlayers = 1;
int mapButtonWasDown[4];
unsigned automapButton[4] = {0x10, 0x20, 0x40, 0x80};
AutoMapPadRecord netLocalPad[4];
float scaleScale = 1.0f;
int gameAutoMapMode = 0;
float mapX, mapY, mapScale, mapScissorX, mapScissorY;
void* gameCam;
void** mapObjList;

static char trace[512];
static void note(const char* text) { std::strcat(trace, text); }
static unsigned bitsOf(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
static double asDouble(unsigned long long b) { double d; std::memcpy(&d, &b, 8); return d; }
static unsigned long long asBits(double d) { unsigned long long b; std::memcpy(&b, &d, 8); return b; }

static bool paused = false;
static int asyncModes[2] = {0, 0};
static int asyncCalls = 0;
static bool deathTask = false;
static int targetCalls = 0, targetNum = 0, yaw = 77;
static int revealCount = 0;
static Point3 revealed[4];
static bool willDrawResult = true, revealedMap = false;
static float willDrawArg = -1.0f;
static Point3 boxLow, boxHigh;
static int findCalls = 0, findCount = 0;
static void* findResult[8];
static void* findList = 0;
static void* localObject[4];
static int drawCalls = 0;
static float drawF[7];
static Point2 drawPoint;
static int drawYaw = -1;
static void* drawWorld;
static void* drawCallback;
static void* listAtDraw[16];

extern "C" unsigned long long norrath_entity_fptodp(float value) { return asBits(value); }
extern "C" int norrath_softdouble_compare_2fba48(unsigned long long a, unsigned long long b) {
    double x = asDouble(a), y = asDouble(b);
    return x < y ? -1 : x > y ? 1 : 0;
}
extern "C" unsigned long long norrath_softdouble_multiply_2fb680(unsigned long long a, unsigned long long b) {
    return asBits(asDouble(a) * asDouble(b));
}
extern "C" float norrath_softdouble_to_float_2fbc20(unsigned long long a) { return (float)asDouble(a); }
extern "C" int gameIsPaused__Fv() { note("g"); return paused; }
extern "C" int MC_GetAsyncMode__Fv() { note("m"); return asyncModes[asyncCalls++ & 1]; }
extern "C" int engineTaskActive__FPc(const char* name) {
    note(std::strcmp(name, "deathTask") == 0 ? "e" : "!");
    return deathTask;
}
extern "C" int GetTargetNum__6Camera(void* c) {
    ++targetCalls;
    note(c == gameCam ? "t" : "!");
    return targetNum;
}
extern "C" int GetYaw__6Camera(void*) { note("y"); return yaw; }
extern "C" void revealMap__3mapG6Point3(const void* p) {
    note("r");
    std::memcpy(&revealed[revealCount++], p, 12);
}
extern "C" int willDraw__3mapP12_worldHeaderf(AutoMapWorld* h, float v) {
    note(h == world ? "w" : "!");
    willDrawArg = v;
    return willDrawResult;
}
extern "C" int isRevealed__3mapv() { note("i"); return revealedMap; }
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(const void* low, const void* high, void** list, int count) {
    note("f");
    std::memcpy(&boxLow, low, 12);
    std::memcpy(&boxHigh, high, 12);
    if (count != 200 || list != mapObjList) note("!");
    findList = list;
    for (int i = 0; i < findCount; ++i) list[i] = findResult[i];
    list[findCount] = 0;
}
extern "C" void* netGetLocalObject__FP8Creature(void* c) {
    note("n");
    for (int i = 0; i < 4; ++i)
        if (c == *(void**)(player + i * 23984 + 21872)) return localObject[i];
    return 0;
}
extern "C" int drawCharacterCallBack__FiP6Point3PsPiN23(int, void*, short*, int*, int*, int*) { return 0; }
extern "C" void draw__3mapP12_worldHeaderfffffffG6Point2PFiP6Point3PsPiPiPi_is(
    AutoMapWorld* h, float a, float b, float c, float d, float e, float f, float g, const void* point,
    int (*callback)(int, void*, short*, int*, int*, int*), int y) {
    note("D");
    ++drawCalls;
    drawWorld = h;
    drawF[0] = a; drawF[1] = b; drawF[2] = c; drawF[3] = d; drawF[4] = e; drawF[5] = f; drawF[6] = g;
    std::memcpy(&drawPoint, point, 8);
    drawCallback = (void*)callback;
    drawYaw = y;
    for (int i = 0; mapObjList && i < 15; ++i) {
        listAtDraw[i] = mapObjList[i];
        if (!mapObjList[i]) break;
    }
}

static AutoMapWorld worldStorage;
static unsigned char camera[64];
static unsigned char objects[4][64];
static unsigned char extra[3][64];

static unsigned char*& objectWord(int p) { return *(unsigned char**)(player + p * 23984 + 21872); }

static void reset() {
    trace[0] = 0;
    std::memset(player, 0, sizeof player);
    std::memset(&worldStorage, 0, sizeof worldStorage);
    worldStorage.flagA = 1;
    worldStorage.record = (void*)1;
    world = &worldStorage;
    std::memset(camera, 0, sizeof camera);
    gameCam = camera;
    *(int*)(camera + 52) = 1;
    *(float*)(camera + 24) = 111.0f;
    *(float*)(camera + 28) = 222.0f;
    std::memset(objects, 0, sizeof objects);
    for (int i = 0; i < 4; ++i) {
        *(Point3*)(objects[i] + 16) = Point3{(float)(i + 1), (float)(i + 10), (float)(i + 100)};
        objectWord(i) = objects[i];
        *(float*)(player + i * 23984 + 172) = 50.0f;
    }
    netNetworkMode = 0;
    validLocalPlayer = 0;
    cutMode = 0;
    nmPlayers = 1;
    for (int i = 0; i < 4; ++i) {
        mapButtonWasDown[i] = 0;
        std::memset(&netLocalPad[i], 0, sizeof netLocalPad[i]);
    }
    scaleScale = 1.0f;
    gameAutoMapMode = 0;
    mapX = mapY = mapScale = mapScissorX = mapScissorY = 0.0f;
    mapObjList = 0;
    paused = false;
    asyncModes[0] = asyncModes[1] = 0;
    asyncCalls = 0;
    deathTask = false;
    targetCalls = 0;
    targetNum = 0;
    revealCount = 0;
    willDrawResult = true;
    revealedMap = false;
    willDrawArg = -1.0f;
    findCalls = 0;
    findCount = 0;
    drawCalls = 0;
    drawYaw = -1;
    std::memset(listAtDraw, 0, sizeof listAtDraw);
}

#define EXPECT_TRACE(code, text) if (std::strcmp(trace, text) != 0) { std::printf("trace %s\n", trace); return code; }

int main() {
    // Early outs.
    reset();
    validLocalPlayer = -1;
    drawAutoMap__Fv();
    EXPECT_TRACE(1, "");
    reset();
    objectWord(0) = 0;
    drawAutoMap__Fv();
    EXPECT_TRACE(2, "");
    reset();
    worldStorage.flagA = 0;
    drawAutoMap__Fv();
    EXPECT_TRACE(3, "");
    reset();
    worldStorage.record = 0;
    drawAutoMap__Fv();
    EXPECT_TRACE(4, "");
    // With a network game the local player checks are skipped; the world checks stay.
    reset();
    netNetworkMode = 1;
    validLocalPlayer = -1;
    worldStorage.flagA = 0;
    drawAutoMap__Fv();
    EXPECT_TRACE(5, "");

    // Plain frame: no buttons; mode 0, one player.
    reset();
    drawAutoMap__Fv();
    EXPECT_TRACE(6, "gmemrtttwittfyD");
    if (mapX != 0.0f || mapY != 0.0f) return 8;
    // One 0.25 step towards the table value (mode 0, single player): scale 0.9, scissor (320, 224).
    if (bitsOf(mapScale) != bitsOf(0.9f * 0.25f)) return 9;
    if (mapScissorX != 80.0f || mapScissorY != 56.0f) return 10;
    if (drawCalls != 1 || drawWorld != world || drawYaw != 77) return 11;
    if (bitsOf(drawF[2]) != bitsOf(mapScale * 1.0f) || drawF[0] != mapX || drawF[1] != mapY ||
        drawF[3] != mapScissorX || drawF[4] != mapScissorY || drawF[5] != 6.0f || drawF[6] != 16.0f) return 12;
    if (drawPoint.x != 111.0f || drawPoint.y != 222.0f) return 13;
    if (drawCallback != (void*)drawCharacterCallBack__FiP6Point3PsPiN23) return 14;
    if (revealCount != 1 || revealed[0].x != 1.0f || revealed[0].y != 10.0f || revealed[0].z != 100.0f) return 15;
    if (willDrawArg != mapScissorX) return 16;
    if (bitsOf(boxLow.x) != bitsOf(111.0f - 960.0f) || boxLow.y != 222.0f - 960.0f || boxLow.z != 100.0f - 960.0f ||
        boxHigh.x != 111.0f + 960.0f || boxHigh.y != 222.0f + 960.0f || boxHigh.z != 100.0f + 960.0f) return 17;
    if (mapObjList == 0) return 18;

    // A revealed map doubles the half-size.
    reset();
    revealedMap = true;
    drawAutoMap__Fv();
    if (boxLow.x != 111.0f - 1920.0f || boxHigh.z != 100.0f + 1920.0f) return 19;
    // willDraw false skips the object search but still draws; mapObjList stays untouched.
    reset();
    willDrawResult = false;
    drawAutoMap__Fv();
    if (findCalls != 0 || mapObjList != 0 || drawCalls != 1 || std::strstr(trace, "f") != 0) return 20;
    if (std::strstr(trace, "i") != 0) return 21;

    // Easing: close values snap, far values move a quarter, equal values stay.
    reset();
    mapX = 0.004f; mapY = 3.0f; mapScale = 0.9f - 0.001f; mapScissorX = 320.0f - 0.0019f; mapScissorY = 224.0f - 0.01f;
    drawAutoMap__Fv();
    if (mapX != 0.0f) return 22;
    if (mapY != 3.0f + (0.0f - 3.0f) * 0.25f) return 23;
    if (bitsOf(mapScale) != bitsOf(0.9f)) return 24;
    if (mapScissorX != 320.0f) return 25;
    if (bitsOf(mapScissorY) != bitsOf((224.0f - 0.01f) + (0.01f) * 0.25f) &&
        bitsOf(mapScissorY) != bitsOf((224.0f - 0.01f) + (224.0f - (224.0f - 0.01f)) * 0.25f)) return 26;
    reset();
    mapX = 0.006f;
    drawAutoMap__Fv();
    if (bitsOf(mapX) != bitsOf(0.006f + (0.0f - 0.006f) * 0.25f)) return 27;
    // The limits differ: 0.0035 is snapped for mapX but moved for mapScale-like values at 0.0035 distance.
    reset();
    mapScale = 0.9f - 0.0035f;
    drawAutoMap__Fv();
    if (bitsOf(mapScale) == bitsOf(0.9f)) return 28;

    // Two players use the second half of the tables; mode 1 and 2 use their own rows.
    reset();
    nmPlayers = 2;
    gameAutoMapMode = 1;
    drawAutoMap__Fv();
    if (mapX != 0.0f || mapY != -30.0f || mapScissorX != 30.0f || mapScissorY != 25.0f) return 29;
    if (drawF[5] != 5.0f || drawF[6] != 10.0f) return 30;
    reset();
    nmPlayers = 1;
    gameAutoMapMode = 2;
    drawAutoMap__Fv();
    if (mapX != 45.0f || mapY != -30.0f) return 31;
    if (mapScissorX != 0.0f || mapScissorY != 0.0f) return 32;

    // Gates after the button step.
    reset();
    asyncModes[0] = 3;
    drawAutoMap__Fv();
    EXPECT_TRACE(33, "gm");
    reset();
    deathTask = true;
    drawAutoMap__Fv();
    EXPECT_TRACE(34, "gme");
    reset();
    asyncModes[1] = 7;
    drawAutoMap__Fv();
    EXPECT_TRACE(35, "gmem");
    reset();
    *(int*)(camera + 52) = 2;
    drawAutoMap__Fv();
    EXPECT_TRACE(36, "gmem");
    if (drawCalls != 0) return 37;

    // Camera target checks.
    reset();
    targetNum = -1;
    drawAutoMap__Fv();
    if (targetCalls != 1 || drawCalls != 0 || revealCount != 1) return 38;
    reset();
    *(float*)(player + 0 * 23984 + 172) = -1.0f;
    drawAutoMap__Fv();
    if (targetCalls != 2 || drawCalls != 0) return 39;
    reset();
    *(float*)(player + 0 * 23984 + 172) = 0.0f;
    drawAutoMap__Fv();
    if (drawCalls != 1) return 139;
    reset();
    targetNum = 1;
    nmPlayers = 2;
    objectWord(1) = 0;
    drawAutoMap__Fv();
    if (targetCalls != 3 || drawCalls != 0 || revealCount != 1) return 40;
    reset();
    targetNum = 1;
    nmPlayers = 2;
    drawAutoMap__Fv();
    // 3 checks + 2 for the box + the yaw call is separate.
    if (targetCalls != 5 || drawCalls != 1) return 41;
    if (boxLow.z != 101.0f - 960.0f) return 42;

    // Buttons: pressed sets the latch, held with the latch cycles mode on release.
    reset();
    netLocalPad[0].pressed = 0x10;
    netLocalPad[0].held = 0x10;
    drawAutoMap__Fv();
    if (mapButtonWasDown[0] != 1 || gameAutoMapMode != 0) return 43;
    // Pressed without held in the same frame latches and releases at once.
    reset();
    netLocalPad[0].pressed = 0x10;
    drawAutoMap__Fv();
    if (mapButtonWasDown[0] != 0 || gameAutoMapMode != 1) return 143;
    reset();
    mapButtonWasDown[0] = 1;
    drawAutoMap__Fv();
    if (gameAutoMapMode != 1 || mapButtonWasDown[0] != 0) return 44;
    // Held keeps the latch and does not cycle.
    reset();
    mapButtonWasDown[0] = 1;
    netLocalPad[0].held = 0x10;
    drawAutoMap__Fv();
    if (gameAutoMapMode != 0 || mapButtonWasDown[0] != 1) return 45;
    // Cycling wraps 2 -> 0, and the other player's button bits are ignored.
    reset();
    gameAutoMapMode = 2;
    mapButtonWasDown[0] = 1;
    netLocalPad[0].held = 0x20;
    drawAutoMap__Fv();
    if (gameAutoMapMode != 0) return 46;
    // Four players skip mode 1.
    reset();
    nmPlayers = 4;
    mapButtonWasDown[2] = 1;
    drawAutoMap__Fv();
    if (gameAutoMapMode != 2 || mapButtonWasDown[2] != 0) return 47;
    reset();
    nmPlayers = 4;
    gameAutoMapMode = 2;
    mapButtonWasDown[3] = 1;
    drawAutoMap__Fv();
    if (gameAutoMapMode != 0) return 48;
    // Paused, cut mode, and a null / flagged object each prevent the step.
    reset();
    paused = true;
    mapButtonWasDown[0] = 1;
    drawAutoMap__Fv();
    if (gameAutoMapMode != 0 || trace[0] != 'g' || trace[1] != 'm') return 49;
    reset();
    cutMode = 1;
    mapButtonWasDown[0] = 1;
    drawAutoMap__Fv();
    if (gameAutoMapMode != 0) return 50;
    reset();
    nmPlayers = 2;
    objectWord(1) = 0;
    mapButtonWasDown[1] = 1;
    drawAutoMap__Fv();
    if (gameAutoMapMode != 0 || mapButtonWasDown[1] != 1) return 51;
    reset();
    nmPlayers = 2;
    *(unsigned*)(objects[1] + 12) = 8;
    mapButtonWasDown[1] = 1;
    drawAutoMap__Fv();
    if (gameAutoMapMode != 0) return 52;
    // The same two cases in a network game still process the player.
    reset();
    netNetworkMode = 1;
    nmPlayers = 2;
    *(unsigned*)(objects[1] + 12) = 8;
    mapButtonWasDown[1] = 1;
    drawAutoMap__Fv();
    if (gameAutoMapMode != 1) return 53;

    // Zoom by the stick while the button is held.
    reset();
    netLocalPad[0].held = 0x10;
    netLocalPad[0].stick = 0.95f;
    mapButtonWasDown[0] = 1;
    drawAutoMap__Fv();
    if (bitsOf(scaleScale) != bitsOf((float)(1.0 * asDouble(0x3fefaee41e6a7499)))) return 54;
    if (mapButtonWasDown[0] != 0) return 55;
    reset();
    netLocalPad[0].held = 0x10;
    netLocalPad[0].stick = -0.95f;
    mapButtonWasDown[0] = 1;
    drawAutoMap__Fv();
    if (bitsOf(scaleScale) != bitsOf((float)(1.0 * asDouble(0x3ff028f5c28f5c28)))) return 56;
    if (mapButtonWasDown[0] != 0) return 57;
    reset();
    netLocalPad[0].held = 0x10;
    netLocalPad[0].stick = 0.9f;
    mapButtonWasDown[0] = 1;
    drawAutoMap__Fv();
    if (scaleScale != 1.0f || mapButtonWasDown[0] != 1) return 58;
    // The stick is not looked at without the held bit.
    reset();
    netLocalPad[0].stick = 0.95f;
    drawAutoMap__Fv();
    if (scaleScale != 1.0f) return 59;
    // Clamps.
    reset();
    netLocalPad[0].held = 0x10;
    netLocalPad[0].stick = 0.95f;
    scaleScale = 2.5f;
    drawAutoMap__Fv();
    if (scaleScale != 2.0f) return 60;
    reset();
    netLocalPad[0].held = 0x10;
    netLocalPad[0].stick = -0.95f;
    scaleScale = 0.4f;
    drawAutoMap__Fv();
    if (scaleScale != 0.5f) return 61;
    reset();
    netLocalPad[0].held = 0x10;
    scaleScale = 3.0f;
    drawAutoMap__Fv();
    if (scaleScale != 2.0f) return 62;

    // Reveal happens for each player with an object; draw scale multiplies by scaleScale.
    reset();
    nmPlayers = 3;
    objectWord(1) = 0;
    scaleScale = 2.0f;
    drawAutoMap__Fv();
    if (revealCount != 2 || revealed[1].x != 3.0f) return 63;
    if (bitsOf(drawF[2]) != bitsOf(mapScale * 2.0f)) return 64;

    // Network list filtering.
    reset();
    netNetworkMode = 1;
    nmPlayers = 2;
    unsigned char o1[32] = {}, o2[32] = {}, o3[32] = {};
    *(unsigned*)(o2 + 12) = 0x40;
    *(unsigned*)(o3 + 12) = 0x3f;
    findCount = 3;
    findResult[0] = o1; findResult[1] = o2; findResult[2] = o3;
    localObject[0] = &extra[0]; localObject[1] = &extra[1];
    drawAutoMap__Fv();
    if (listAtDraw[0] != o1 || listAtDraw[1] != o3 || listAtDraw[2] != &extra[0] || listAtDraw[3] != &extra[1] ||
        listAtDraw[4] != 0) return 65;
    // An empty find result yields only the local objects.
    reset();
    netNetworkMode = 1;
    nmPlayers = 2;
    objectWord(1) = 0;
    localObject[0] = &extra[2];
    findCount = 0;
    drawAutoMap__Fv();
    if (listAtDraw[0] != &extra[2] || listAtDraw[1] != 0) return 66;
    // Without a network the list is left alone.
    reset();
    findCount = 2;
    findResult[0] = o2; findResult[1] = o1;
    drawAutoMap__Fv();
    if (listAtDraw[0] != o2 || listAtDraw[1] != o1 || listAtDraw[2] != 0 || std::strchr(trace, 'n') != 0) return 67;
    return 0;
}
