// Host contract for cutRunFrame__Fb. Not an EE plan input.
#include <cstdarg>
#include <cstdio>
#include <cstring>

#define sprintf host_sprintf
#define strcmp host_strcmp
#include "cut_run_frame.cpp"
#undef sprintf
#undef strcmp

int gameControlPaused, animationLoadPos, displayDialogPos, nmDialogNodes, coreLoading, displayFrame;
int frontEndRunning, nmPlayers, displayActionPos, nmActions, cutTime, flags, engineDisplayDisable, eRandState;
int frameNm;
unsigned char* dialogPacket;
char* dlgDir;
unsigned char animationLoading, cutInit, cutSkipMode, hudNoDraw;
signed char netNetworkMode, coreLoadingScreenRunning;
CutAnimSlot anim[2];
unsigned char* cutCurrentScene;
float myFOV;
short myRoll, myPitch, myYaw;
unsigned char cutCameraPos[12];
CutPadRecord pad[4];
void* gameCam = (void*)0x1234;

static char trace[1024];
static void note(const char* text) { std::strcat(trace, text); }
static char fmtBuf[96];
static void notef(const char* format, ...) {
    va_list args;
    va_start(args, format);
    std::vsnprintf(fmtBuf, sizeof fmtBuf, format, args);
    va_end(args);
    note(fmtBuf);
}
static unsigned bitsOf(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }

static bool busy = false;
static bool completeLoad = false;
static unsigned char* lastFlag = 0;
extern "C" int cdIsBusy__Fv() { note("b"); return busy; }
extern "C" int host_sprintf(char* to, const char* format, ...) {
    va_list args;
    va_start(args, format);
    const char* text = va_arg(args, const char*);
    va_end(args);
    if (std::strcmp(format, "%s.anm") != 0) note("!");
    std::strcpy(to, text);
    std::strcat(to, ".anm");
    note("p");
    return 0;
}
extern "C" int host_strcmp(const char* a, const char* b) { return std::strcmp(a, b); }
extern "C" char* soundGetDialogFilename__Fv() { note("f"); return (char*)"FILE"; }
static char loadName[16];
static void* loadDest;
static int loadSize, loadOffset;
extern "C" void cdLoadAsync__FPcPvT0ii(char* name, void* destination, void* flag, int size, int offset) {
    std::strcpy(loadName, name);
    loadDest = destination;
    lastFlag = (unsigned char*)flag;
    loadSize = size;
    loadOffset = offset;
    note("L");
    if (completeLoad) *(unsigned char*)flag = 1;
}
static float lightScale;
extern "C" void lightGlobalCharLightScale__Ff(float s) { lightScale = s; note("l"); }
extern "C" void cutInitScene__FP9_cutscene(void* scene) { note(scene == cutCurrentScene ? "I" : "!"); }
static float advanceIn[8];
static int advances = 0;
static bool actionStep = false;
extern "C" float advanceFrame__Ff(float step) {
    if (advances < 8) advanceIn[advances] = step;
    ++advances;
    note("a");
    if (actionStep) ++displayActionPos;
    return step + 1.0f;
}
static float cameraIn;
extern "C" void setCamera__Ff(float s) { cameraIn = s; note("c"); }
static float frustum[4];
extern "C" void worldSetFrustum__Fffff(float a, float b, float c, float d) {
    frustum[0] = a; frustum[1] = b; frustum[2] = c; frustum[3] = d; note("S");
}
extern "C" void matIdent__FP8Matrix34(void* m) { std::memset(m, 0, 48); note("i"); }
static int rotAxes[4], rotAngles[4], rotCount = 0;
extern "C" void matRot__FP8Matrix34is(void* m, int axis, short angle) {
    if (rotCount < 4) { rotAxes[rotCount] = axis; rotAngles[rotCount] = angle; }
    ++rotCount;
    note("r");
}
extern "C" void worldSetViewMatricies__FP6Point3P8Matrix34(void* p, void* m) {
    note(p == cutCameraPos && m ? "V" : "!");
}
extern "C" void worldSetViewCenter__Fv() { note("W"); }
extern "C" void engineSyncVBlank__Fv() { note("y"); }
extern "C" void engineSetDisplayDisable__Fi(int v) { notef("D%d", v); }
static int resets[4], resetCount = 0;
extern "C" void soundResetStream__Fi(int s) { resets[resetCount++] = s; note("s"); }
static int streamStatus = 0;
extern "C" int soundGetStreamPlayStatus__Fi(int s) { note("g"); return s == 0 ? streamStatus : 0; }
extern "C" void cutStop__Fv() { note("X"); }
extern "C" void coreStartLoading__Fv() { note("Q"); }
extern "C" void SetMode__6Camerai(void* c, int m) { notef("M%d%s", m, c == gameCam ? "" : "!"); }
extern "C" void SetCutWasActive__6Camera(void* c) { note(c == gameCam ? "A" : "!"); }
extern "C" void scriptActivatePostAction__Fv() { note("P"); }

static unsigned char scene[128];
static unsigned char packet[3 * 1096];
static char dir[5 * 80];
static char blockA, blockB;

static void reset() {
    trace[0] = 0;
    advances = 0;
    rotCount = 0;
    resetCount = 0;
    gameControlPaused = 0;
    animationLoadPos = 0;
    displayDialogPos = 0;
    nmDialogNodes = 3;
    dialogPacket = 0;
    dlgDir = 0;
    animationLoading = 7;
    anim[0].block = &blockA; anim[0].second = 0;
    anim[1].block = &blockB; anim[1].second = 0;
    cutInit = 1;
    cutCurrentScene = scene;
    *(int*)(scene + 112) = 50;
    displayFrame = 120;
    coreLoading = 0;
    netNetworkMode = 0;
    cutSkipMode = 0;
    frontEndRunning = 0;
    myFOV = 40.0f;
    myRoll = 11; myPitch = 22; myYaw = 33;
    coreLoadingScreenRunning = 0;
    nmPlayers = 0;
    for (auto& p : pad) p.buttons = 0;
    displayActionPos = 1;
    nmActions = 5;
    cutTime = 99;
    flags = 0;
    engineDisplayDisable = 0;
    hudNoDraw = 1;
    eRandState = 0;
    frameNm = 4242;
    busy = false;
    completeLoad = false;
    actionStep = false;
    streamStatus = 0;
}

static void setupDialog() {
    dialogPacket = packet;
    dlgDir = dir;
    std::memset(packet, 0, sizeof packet);
    std::memset(dir, 0, sizeof dir);
    const char* names[] = {"a.anm", "b.anm", "c.anm"};
    int offsets[] = {0, 100, 250, 400};
    for (int i = 0; i < 3; ++i) {
        std::strcpy(dir + 80 * i, names[i]);
        *(int*)(dir + 80 * i + 64) = offsets[i];
    }
    // The size of the last entry comes from the word at +64 of the entry after it (the terminator row).
    *(int*)(dir + 80 * 3 + 64) = offsets[3];
    std::strcpy((char*)packet + 49, "a");
    std::strcpy((char*)packet + 1096 + 49, "b");
    std::strcpy((char*)packet + 2 * 1096 + 49, "zz");
}

#define EXPECT_TRACE(code, text) if (std::strcmp(trace, text) != 0) { std::printf("trace %s\n", trace); return code; }

int main() {
    const float sixtieth = bitsOf(0x1.111110p-6f) == 0x3c888888u ? 0x1.111110p-6f : 0.0f;
    if (sixtieth == 0.0f) return 100;

    // A: plain frame with no dialog and no skip. The scaled frame is advanced and fed to the camera.
    reset();
    cutRunFrame__Fb(false);
    EXPECT_TRACE(1, "lacSirrrrVW");
    if (gameControlPaused != 60) return 2;
    if (bitsOf(lightScale) != bitsOf(50.0f * 0x1.999998p-4f)) return 3;
    if (bitsOf(lightScale) == bitsOf(50.0f * 0.1f)) return 4;
    if (advances != 1 || bitsOf(advanceIn[0]) != bitsOf(120.0f * sixtieth)) return 5;
    if (bitsOf(cameraIn) != bitsOf(advanceIn[0] + 1.0f)) return 6;
    if (frustum[0] != 40.0f || bitsOf(frustum[1]) != 0x3faa3d70u || bitsOf(frustum[2]) != bitsOf(40.0f * 0x1.999998p+0f) ||
        frustum[3] != 0.75f) return 7;
    if (rotCount != 4 || rotAxes[0] != 0 || rotAngles[0] != 16339 || rotAxes[1] != 1 || rotAngles[1] != 11 ||
        rotAxes[2] != 0 || rotAngles[2] != 22 || rotAxes[3] != 2 || rotAngles[3] != 33) return 8;
    if (eRandState != 4242 || engineDisplayDisable != 0 || hudNoDraw != 1) return 9;

    // B: the front end uses 1.0 as the last frustum value; the argument or the loading state suppress advancing.
    reset();
    frontEndRunning = 1;
    cutRunFrame__Fb(true);
    EXPECT_TRACE(10, "lcSirrrrVW");
    if (frustum[3] != 1.0f || bitsOf(cameraIn) != bitsOf(120.0f * sixtieth)) return 11;
    reset();
    coreLoading = 1;
    cutRunFrame__Fb(false);
    EXPECT_TRACE(12, "lcSirrrrVW");
    reset();
    coreLoading = 1;
    netNetworkMode = 1;
    cutRunFrame__Fb(false);
    EXPECT_TRACE(13, "lacSirrrrVW");
    reset();
    cutInit = 0;
    cutSkipMode = 0;
    cutRunFrame__Fb(false);
    EXPECT_TRACE(14, "lIacSirrrrVW");
    if (cutInit != 1) return 15;

    // C: loading screen running ends the frame before the skip and finish logic.
    reset();
    coreLoadingScreenRunning = 1;
    nmPlayers = 1;
    pad[0].buttons = 0x800;
    displayActionPos = nmActions;
    cutRunFrame__Fb(false);
    EXPECT_TRACE(16, "lacSirrrrVW");
    if (cutSkipMode != 0 || eRandState != 0) return 17;

    // D: dialog load. Entry "b.anm" is index 1; the size comes from the next entry's +64 word.
    reset();
    setupDialog();
    animationLoadPos = 1;
    displayDialogPos = 1;
    cutRunFrame__Fb(true);
    if (std::strncmp(trace, "bpfL", 4) != 0) { std::printf("trace %s\n", trace); return 20; }
    if (std::strcmp(loadName, "FILE") != 0 || loadDest != &blockB || loadOffset != 100 || loadSize != 150) return 21;
    if (animationLoading != 1 || anim[1].second != 1 || anim[0].second != 0) return 22;
    if (lastFlag == 0) return 23;
    // The flag the loader receives is the one the next call clears; once set, the position advances.
    *lastFlag = 1;
    reset();
    setupDialog();
    animationLoadPos = 1;
    displayDialogPos = 1;
    anim[1].second = 1;
    cutRunFrame__Fb(true);
    if (animationLoadPos != 2 || animationLoading != 0 || *lastFlag != 0) return 24;
    if (std::strncmp(trace, "l", 1) != 0) return 25;

    // E: a name that is in no entry marks the animation loaded and leaves the slot's second word zero.
    reset();
    setupDialog();
    animationLoadPos = 2;
    displayDialogPos = 2;
    cutRunFrame__Fb(true);
    if (std::strncmp(trace, "bp", 2) != 0 || trace[2] == 'L') return 26;
    if (animationLoading != 7 || anim[0].second != 0) return 27;
    // The flag is now set: the next call clears it and advances without loading.
    cutRunFrame__Fb(true);
    if (animationLoadPos != 3 || animationLoading != 0) return 28;

    // F: first entry empty, busy drive, full slot, past the node count, and displayDialogPos < animationLoadPos
    // all leave the dialog step alone.
    reset();
    setupDialog();
    dir[0] = 0;
    animationLoadPos = 1;
    displayDialogPos = 1;
    cutRunFrame__Fb(true);
    if (std::strncmp(trace, "bp", 2) != 0 || trace[2] == 'L') return 29;
    cutRunFrame__Fb(true);
    if (animationLoadPos != 2) return 30;
    reset();
    setupDialog();
    busy = true;
    animationLoadPos = 1; displayDialogPos = 1;
    cutRunFrame__Fb(true);
    if (std::strncmp(trace, "bl", 2) != 0) return 31;
    reset();
    setupDialog();
    anim[0].block = 0;
    animationLoadPos = 1; displayDialogPos = 1;
    cutRunFrame__Fb(true);
    if (trace[0] != 'l') return 32;
    reset();
    setupDialog();
    nmDialogNodes = 1;
    animationLoadPos = 1; displayDialogPos = 1;
    cutRunFrame__Fb(true);
    if (std::strncmp(trace, "bl", 2) != 0) return 33;
    reset();
    setupDialog();
    anim[1].second = 5;
    animationLoadPos = 1; displayDialogPos = 1;
    cutRunFrame__Fb(true);
    if (std::strncmp(trace, "bl", 2) != 0) return 34;
    reset();
    setupDialog();
    animationLoadPos = 2; displayDialogPos = 1;
    cutRunFrame__Fb(true);
    if (trace[0] != 'l') return 35;
    reset();
    animationLoadPos = 1; displayDialogPos = 1;
    dialogPacket = packet;
    cutRunFrame__Fb(true);
    if (trace[0] != 'l') return 36;

    // G: pad bit 0x800 of any of nmPlayers records forces skip; the action loop runs to nmActions, then the finish
    // sequence runs (flags bit 4 clear, so loading starts).
    reset();
    nmPlayers = 2;
    pad[1].buttons = 0x800;
    actionStep = true;
    cutRunFrame__Fb(false);
    EXPECT_TRACE(40, "lacSirrrrVWyD1sgaaaXQM1AP");
    if (cutSkipMode != 1 || resetCount != 1 || resets[0] != 2) return 41;
    if (engineDisplayDisable != 3 || hudNoDraw != 0 || eRandState != 4242 || displayActionPos != 5) return 42;
    if (bitsOf(advanceIn[1]) != bitsOf(120.0f * sixtieth)) return 43;

    // Stream 0 playing and not yet finished is reset too; finished streams are left.
    reset();
    nmPlayers = 1; pad[0].buttons = 0x800 | 1; actionStep = true;
    streamStatus = 0x80;
    cutRunFrame__Fb(true);
    EXPECT_TRACE(44, "lcSirrrrVWyD1sggsaaaaXQM1AP");
    if (resetCount != 2 || resets[0] != 2 || resets[1] != 0) return 45;
    reset();
    nmPlayers = 1; pad[0].buttons = 0x800; actionStep = true;
    streamStatus = 0x180;
    cutRunFrame__Fb(true);
    EXPECT_TRACE(46, "lcSirrrrVWyD1sggaaaaXQM1AP");

    // A pad bit beyond nmPlayers is ignored; a pad bit without the mask is ignored.
    reset();
    nmPlayers = 1; pad[1].buttons = 0x800; pad[0].buttons = 0x7ff;
    cutRunFrame__Fb(false);
    if (cutSkipMode != 0) return 47;
    reset();
    nmPlayers = 1; pad[0].buttons = 0x800; frontEndRunning = 1;
    cutRunFrame__Fb(false);
    if (cutSkipMode != 0) return 48;

    // Finish without a skip: the sequence runs once the positions meet. Flag bit 4 suppresses loading.
    reset();
    displayActionPos = nmActions;
    flags = 4;
    cutRunFrame__Fb(false);
    EXPECT_TRACE(50, "lacSirrrrVWXM1AP");
    if (engineDisplayDisable != 0 || hudNoDraw != 0 || eRandState != 4242) return 51;
    reset();
    displayActionPos = nmActions;
    flags = 3;
    cutRunFrame__Fb(false);
    EXPECT_TRACE(52, "lacSirrrrVWXQM1AP");
    if (engineDisplayDisable != 3) return 53;
    // Flag bit 4 with skip still loads.
    reset();
    displayActionPos = nmActions;
    flags = 4;
    cutSkipMode = 1;
    cutRunFrame__Fb(false);
    EXPECT_TRACE(54, "lcSirrrrVWyD1sgXQM1AP");

    // Front end finish: counters are zeroed and nothing else runs, including eRandState.
    reset();
    frontEndRunning = 1;
    displayActionPos = nmActions;
    displayFrame = 77;
    cutRunFrame__Fb(false);
    EXPECT_TRACE(55, "lacSirrrrVW");
    if (displayFrame != 0 || displayActionPos != 0 || cutTime != 0 || eRandState != 0) return 56;
    return 0;
}
