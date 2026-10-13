// Host check for inventoryDrawPlayer__FiiP14AnimationStatei. Not an EE plan input.
// Pointer words in the image are 32-bit, so the source maps them through a hook here.
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
static void* handleTable[8];
void* hostPointer(unsigned w) { return handleTable[w]; }
#define INVENTORY_DRAW_PLAYER_HOST_POINTER hostPointer
#include "inventory_draw_player.cpp"
unsigned char player[5 * 23984];
unsigned char inventory[232] __attribute__((aligned(4)));
unsigned char netLocalPad[352];
unsigned char animState[16];
int eRandState, charAnimTest, worldWaterBumpSpeed;
static std::string ev;
static float lastScale;
#define N(x) std::to_string(x)
extern "C" {
void animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void*, void* p, void*, void*) { ev += "A" + N(static_cast<float*>(p)[2]) + ";"; }
void matMakeYPRScale34_ReallySlow__FP8Matrix34G6Point3sssf(void*, const void* v, short yaw, short a, short b, float s) {
    const float* f = static_cast<const float*>(v);
    ev += "K" + N(f[0]) + "," + N(f[1]) + "," + N(f[2]) + ":" + N(yaw) + "," + N(a) + "," + N(b) + ":" + N(s) + ";";
}
void matRot__FP8Matrix34is(void*, int axis, short angle) { ev += "R" + N(axis) + "," + N(angle) + ";"; }
void* modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(void* m, void* t, int a, const void*, void*, void* anim, unsigned long long flags, void*, int) {
    ev += "M" + N(m != nullptr) + N(t != nullptr) + N(a) + N(anim != nullptr) + ":" + N(flags) + ";";
    return nullptr;
}
void drawAttachments__6PlayerP11_drawRecordf(void*, void*, float s) { ev += "D" + N(s) + ";"; }
void playerDrawWeaponEffects__6Playerf(void*, float s) { ev += "W" + N(s) + ";"; }
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void*, void* h, float a, float b, int c) { ev += "O" + std::string(static_cast<char*>(h)) + ";"; }
void* lumpFindResource__FPcT0(char* l, char* n) { static std::string names[16]; static int k; names[k % 16] = std::string(l) + "/" + n; return const_cast<char*>(names[k++ % 16].c_str()); }
}
static unsigned char object[2048];
static unsigned char lumpHolder[8];
static char lumpName[] = "objlump";
static int rngFor(int r) {  // eRandState that makes r the result of the first generator step
    unsigned inv = 1;
    for (int i = 0; i < 32; i++) inv *= 2 - 0x19660du * inv;
    return static_cast<int>((((static_cast<unsigned>(r) << 16) - 0x3c6ef35fu) * inv));
}
int main() {
    handleTable[1] = object; handleTable[2] = lumpHolder; handleTable[3] = lumpName;
    *reinterpret_cast<unsigned*>(player + 21872) = 1;
    *reinterpret_cast<unsigned*>(object + 36) = 2;
    *reinterpret_cast<unsigned*>(lumpHolder) = 3;
    *reinterpret_cast<unsigned long long*>(object + 552) = 0x1122334455667788ull;
    *reinterpret_cast<unsigned*>(object + 1332) = 1; *reinterpret_cast<unsigned*>(object + 1344) = 1;
    animState[4] = 1;
    // owner out of range or without an object: nothing
    inventoryDrawPlayer__FiiP14AnimationStatei(5, 0, animState, 0);
    inventoryDrawPlayer__FiiP14AnimationStatei(1, 0, animState, 0);
    if (!ev.empty()) return 1;
    // mode 1, stand drawing after: yaw comes from R+23536 plus the pad turn (0.5 * 608 = 304)
    *reinterpret_cast<int*>(player + 23536) = 20000;
    *reinterpret_cast<float*>(netLocalPad + 28) = 0.5f;
    eRandState = rngFor(60000);
    inventoryDrawPlayer__FiiP14AnimationStatei(0, 1, animState, 0);
    if (*reinterpret_cast<int*>(player + 23536) != 20304) return 2;
    std::string expect = "A403.000000;K47.000000,30.000000,-44.000000:3920,-87,-55:1.189998;M1141:1234605616436508552;D1.500000;K492.000000,1465.000000,-491.000000:0,0,-820:14.616776;R2,20304;M1140:18446744073709551615;W1.000000;";
    if (ev != expect) return 3;
    // mode 2 uses other constants and the mirrored stand
    ev.clear(); eRandState = rngFor(60000);
    inventoryDrawPlayer__FiiP14AnimationStatei(0, 2, animState, 0);
    if (ev.find("K-43.445999,30.000000,-44.000000:") == std::string::npos || ev.find("K-494.000000,1465.000000,-491.000000:0,0,1304:") == std::string::npos) return 4;
    // state 1 with class 0, animState+4 == 1 and a small random value starts the one-shot in the object's lump
    ev.clear(); player[23582] = 1; *reinterpret_cast<unsigned*>(player + 80) = 2; eRandState = rngFor(224);
    inventoryDrawPlayer__FiiP14AnimationStatei(0, 3, animState, 0);
    if (ev.find("Oobjlump/dhs_idlec.anm;") == std::string::npos) return 5;
    ev.clear(); eRandState = rngFor(225);
    inventoryDrawPlayer__FiiP14AnimationStatei(0, 3, animState, 0);
    if (ev.find("dhs_idlec") != std::string::npos) return 6;
    // state 5, class 0: charAnimTest counts up, 279 clears the 16 words, nw_idled plays
    ev.clear(); player[23582] = 5; *reinterpret_cast<unsigned*>(player + 80) = 0; charAnimTest = 278;
    for (int o = 21896; o <= 21956; o += 4) *reinterpret_cast<unsigned*>(player + o) = 9;
    *reinterpret_cast<unsigned*>(player + 21944) = 0;
    eRandState = rngFor(100);
    inventoryDrawPlayer__FiiP14AnimationStatei(0, 3, animState, 0);
    if (charAnimTest != 279 || *reinterpret_cast<unsigned*>(player + 21896) != 0 || *reinterpret_cast<unsigned*>(player + 21956) != 0) return 7;
    if (ev.find("Ofx.lmp/nw_idled.anm;") == std::string::npos) return 9;
    // nothing is idling and the water flag with both pad bits starts nw_idlep and sets charAnimTest
    ev.clear(); charAnimTest = 0; worldWaterBumpSpeed = 1; *reinterpret_cast<unsigned*>(netLocalPad + 8) = 0x600;
    *reinterpret_cast<unsigned*>(player + 21912) = 1;   // blocks nw_idlec
    inventoryDrawPlayer__FiiP14AnimationStatei(0, 3, animState, 0);
    if (charAnimTest != 1 || ev.find("Ofx.lmp/nw_idlep.anm;") == std::string::npos) return 10;
    ev.clear(); charAnimTest = 0; *reinterpret_cast<unsigned*>(netLocalPad + 8) = 0x400;
    inventoryDrawPlayer__FiiP14AnimationStatei(0, 3, animState, 0);
    if (charAnimTest != 0 || ev.find("nw_idlep") != std::string::npos) return 11;
    // nw_idlec when nothing blocks it
    ev.clear(); *reinterpret_cast<unsigned*>(player + 21912) = 0; *reinterpret_cast<unsigned*>(player + 21908) = 0; worldWaterBumpSpeed = 0;
    eRandState = rngFor(194);
    inventoryDrawPlayer__FiiP14AnimationStatei(0, 3, animState, 0);
    if (ev.find("Oobjlump/nw_idlec.anm;") == std::string::npos) return 12;
    return 0;
}
