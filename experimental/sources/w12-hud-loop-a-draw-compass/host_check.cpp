// Host check for drawCompass__Fv. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -ffp-contract=off -fno-builtin host_check.cpp
#define NORRATH_HOST_CONTRACT
#include "draw_compass.cpp"

#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
int localNmPlayersAlive;
int validLocalPlayer;
alignas(8) unsigned char player[23984 * 3];
signed char drawCompassOn;
float worldViewMatrix[16];
float worldCameraPos[3];
float drawCompassPoint[2];
unsigned short drawCompassTitle[2] = {84, 0};
int fontObject;
unsigned char* norrath_draw_compass_host_base;
void* smallFont = &fontObject;
}

static std::string trace;
static int dlg;
static int atanResult;
static void add(const char* format, double a = 0, double b = 0, double c = 0, double d = 0) {
    char text[200];
    std::snprintf(text, sizeof text, format, a, b, c, d);
    trace += text;
}

extern "C" {
int DLG_Active__Fv() { trace += "D;"; return dlg; }
void matTrans__FP8Matrix44fff(float* m, float x, float y, float z) {
    add("V%g,", m[0]);
    add("%g,", m[15]);
    add("%g,%g,%g;", x, y, z);
    m[12] = 100.0f;
}
void matUntransform__FP6Point3P8Matrix44T0(float* out, float* m, float* in) {
    add("U%g,", m[12]);
    add("%g,%g,%g;", in[0], in[1], in[2]);
    out[0] = 1.0f; out[1] = 2.0f; out[2] = 3.0f;
}
void matIdent__FP8Matrix34(float* m) { trace += "I;"; for (int i = 0; i < 12; ++i) m[i] = static_cast<float>(i); }
void matTrans__FP8Matrix34fff(float* m, float x, float y, float z) { add("T%g,", m[11]); add("%g,%g,%g;", x, y, z); }
int iatan2__Fff(float y, float x) { add("A%g,%g;", y, x); return atanResult; }
void matRot__FP8Matrix34is(float*, int axis, short angle) { add("Q%g,%g;", axis, angle); }
void modelSetNoZRead__Fv() { trace += "N;"; }
void* lumpFindResource__FPcT0(const char* group, const char* name) {
    trace += "L"; trace += group; trace += ","; trace += name; trace += ";";
    return std::strcmp(name, "compass.vif") == 0 ? static_cast<void*>(&fontObject) : nullptr;
}
void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const float* offset, float* m, void* animation, unsigned long flags,
    void* record, int extra) {
    add("M%g,", model == &fontObject);
    add("%g,%g,", texture == nullptr, mode);
    add("%g,%g,", offset[0], offset[1]);
    add("%g,%g,", offset[2], m[1]);
    add("%g,%g,", animation == nullptr, record == nullptr);
    add("%g,%g;", static_cast<double>(flags), extra);
}
void modelResetOptionalParams__Fv() { trace += "R;"; }
void lightDisableVolumetricLighting__Fv() { trace += "X;"; }
void lightDisableLocalLights__Fv() { trace += "Y;"; }
void lightResetOptionalParams__Fv() { trace += "Z;"; }
void fontStart__FP4FontiUii(void* font, int mode, unsigned color, int last) {
    add("S%g,", font == &fontObject);
    add("%g,%g,%g,%g;", mode, static_cast<double>(color >> 16), static_cast<double>(color & 0xffff), last);
}
void fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, unsigned short* text, int w, int a, int b, int c) {
    add("W%g,%g,", x, y);
    add("%g,%g,", text == drawCompassTitle, w);
    add("%g,%g,%g;", a, b, c);
}
void fontEnd__Fv() { trace += "E;"; }
}

static void put_u32(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static void put_f32(unsigned char* p, float v) { std::memcpy(p, &v, 4); }

static int expect(int code, const char* want) {
    if (trace == want) return 0;
    std::printf("case %d\n got  %s\n want %s\n", code, trace.c_str(), want);
    return code;
}

static unsigned char entities[256];

static void reset() {
    trace.clear();
    std::memset(player, 0, sizeof player);
    std::memset(entities, 0, sizeof entities);
    norrath_draw_compass_host_base = entities;
    localNmPlayersAlive = 2;
    validLocalPlayer = 1;
    drawCompassOn = 1;
    dlg = 0;
    atanResult = 40000;
    for (int i = 0; i < 16; ++i) worldViewMatrix[i] = static_cast<float>(i + 1);
    worldCameraPos[0] = 10.0f; worldCameraPos[1] = 20.0f; worldCameraPos[2] = 30.0f;
    drawCompassPoint[0] = 1.5f; drawCompassPoint[1] = 2.5f;
    put_u32(player + 23984 + 21872, 64);     // entity at entities + 64
    put_f32(entities + 64 + 16, 5.5f);
    put_f32(entities + 64 + 20, 9.5f);
}

int main() {
    int rc;
    reset(); localNmPlayersAlive = 0; drawCompass__Fv();
    if ((rc = expect(1, ""))) return rc;
    reset(); validLocalPlayer = -1; drawCompass__Fv();
    if ((rc = expect(2, ""))) return rc;
    reset(); put_u32(player + 23984 + 21872, 0); drawCompass__Fv();
    if ((rc = expect(3, ""))) return rc;
    reset(); dlg = 1; drawCompass__Fv();
    if ((rc = expect(4, "D;"))) return rc;
    reset(); drawCompassOn = 0; drawCompass__Fv();
    if ((rc = expect(5, "D;"))) return rc;
    reset(); drawCompass__Fv();
    const char* full =
        "D;V1,16,0,0,0;U100,-0.113998,0.156996,-5.75479;I;T11,11,22,33;A7,4;Q2,7232;N;"
        "Lcompass.lmp,compass.vif;Lcompass.lmp,compass.tex;M1,1,5,0,0,1,1,1,1,4.29497e+09,0;R;X;Y;"
        "Lcompass.lmp,compass.vif;Lcompass.lmp,compass.tex;M1,1,5,0,0,1,1,1,1,4.29497e+09,0;R;Z;"
        "S1,5,25391,24921,-1;W65,394,1,131,-100,500,18;E;";
    if ((rc = expect(6, full))) return rc;
    // the rotation argument is the angle minus 32768 truncated to 16 bits
    reset(); atanResult = 0x18000 + 5; drawCompass__Fv();
    if (trace.find("Q2,5;") == std::string::npos) return 7;
    reset(); atanResult = 0x4000; drawCompass__Fv();
    if (trace.find("Q2,-16384;") == std::string::npos) return 8;
    std::puts("draw compass passed");
    return 0;
}
