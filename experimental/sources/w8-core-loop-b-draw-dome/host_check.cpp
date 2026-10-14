// Host contract for drawDome__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>

#define strcmp host_strcmp
#include "draw_dome.cpp"
#undef strcmp

void* domeLump = 0;
char levelName[322];
unsigned char worldCameraPos[12];
int frameNm = 0;
unsigned char modelClippingOn = 0;

static char trace[512];
static void note(const char* text) { std::strcat(trace, text); }
static char fmt[64];

extern "C" int host_strcmp(const char* a, const char* b) { return std::strcmp(a, b); }
static const void* madePoint = 0;
static int rotAxis = -1, rotAngle = 0;
extern "C" void matMakeTrans__FP8Matrix34G6Point3(void* matrix, const void* point) {
    note("T");
    madePoint = point;
    ((unsigned char*)matrix)[0] = 1;
}
extern "C" void matRot__FP8Matrix34is(void* matrix, int axis, short angle) {
    note("R");
    rotAxis = axis;
    rotAngle = angle;
    if (((unsigned char*)matrix)[0] != 1) note("!");
}
extern "C" void modelSetNoZWrite__Fv() { note(modelClippingOn == 1 ? "Z" : "z"); }
extern "C" void lightDisableLocalLights__Fv() { note("D"); }
static short ambientSeen[4];
extern "C" void lightSetAmbientColor__FPs(short* c) {
    note("C");
    std::memcpy(ambientSeen, c, sizeof ambientSeen);
}
extern "C" void lightSetAmbientOnly__Fv() { note("O"); }

static DomeLumpEntry vif[3] = {{0, (void*)0x10}, {0, (void*)0x11}, {0, (void*)0x12}};
static DomeLumpEntry tex[3] = {{0, (void*)0x20}, {0, (void*)0x21}, {0, (void*)0x22}};
static int vifCount = 3, texCount = 3;
extern "C" DomeLumpEntry* lumpFindByType__FPvPci(void* lump, const char* type, int index) {
    if (lump != domeLump) note("!");
    std::snprintf(fmt, sizeof fmt, "[%s%d]", type, index);
    note(fmt);
    if (!std::strcmp(type, "vif")) return index < vifCount ? &vif[index] : 0;
    if (!std::strcmp(type, "tex")) return index < texCount ? &tex[index] : 0;
    return 0;
}
static int draws = 0;
static void* drawModel[4];
static void* drawTexture[4];
static float drawPoint[4][3];
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned flags, void* record, int last) {
    note("M");
    if (mode != 0 || animation || flags != 0xffffffffu || record || last || ((unsigned char*)matrix)[0] != 1)
        note("!");
    drawModel[draws] = model;
    drawTexture[draws] = texture;
    std::memcpy(drawPoint[draws], point, 12);
    ++draws;
    return 0;
}
extern "C" void modelResetOptionalParams__Fv() { note("X"); }
extern "C" void lightResetOptionalParams__Fv() { note("Y"); }

static void reset() {
    trace[0] = 0;
    draws = 0;
    rotAxis = -1;
    madePoint = 0;
    modelClippingOn = 0;
}

int main() {
    // Null lump: nothing happens.
    reset();
    drawDome__Fv();
    if (trace[0] != 0 || modelClippingOn != 0) return 1;

    int token;
    domeLump = &token;
    std::strcpy(levelName + 81, "other");
    frameNm = 5;
    reset();
    drawDome__Fv();
    if (std::strcmp(trace, "TZDCO[vif0][tex0]M[vif1][tex1]M[vif2][tex2]M[vif3]XY") != 0) return 2;
    if (madePoint == 0 || rotAxis != -1 || modelClippingOn != 1) return 3;
    if (ambientSeen[0] != 128 || ambientSeen[1] != 128 || ambientSeen[2] != 128 || ambientSeen[3] != 0) return 4;
    if (draws != 3 || drawModel[0] != (void*)0x10 || drawTexture[2] != (void*)0x22) return 5;
    if (drawPoint[1][0] != 1000000.0f || drawPoint[1][1] != 0.0f || drawPoint[1][2] != 0.0f) return 6;

    // Each of the four names rotates by (frameNm % 65536) * 3 as a short.
    const char* names[] = {"hate1", "hate3", "hatecut", "intro"};
    for (const char* name : names) {
        std::strcpy(levelName + 81, name);
        frameNm = 70000;
        reset();
        drawDome__Fv();
        if (rotAxis != 2 || rotAngle != (short)((70000 % 65536) * 3)) return 7;
        if (std::strncmp(trace, "TRZ", 3) != 0) return 8;
    }
    // Names that only share a prefix do not rotate.
    std::strcpy(levelName + 81, "hate2");
    reset();
    drawDome__Fv();
    if (rotAxis != -1) return 9;
    // Negative frame numbers keep the C remainder sign.
    std::strcpy(levelName + 81, "intro");
    frameNm = -70000;
    reset();
    drawDome__Fv();
    if (rotAngle != (short)((-70000 % 65536) * 3)) return 10;
    // Missing texture stops the loop without a draw, and the cleanup still runs.
    std::strcpy(levelName + 81, "x");
    texCount = 1;
    reset();
    drawDome__Fv();
    if (std::strcmp(trace, "TZDCO[vif0][tex0]M[vif1][tex1]XY") != 0 || draws != 1) return 11;
    // No vif at all.
    vifCount = 0;
    reset();
    drawDome__Fv();
    if (std::strcmp(trace, "TZDCO[vif0]XY") != 0 || draws != 0) return 12;
    return 0;
}
