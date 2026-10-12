// Host contract for cutClear__Fv. Not an EE plan input.
#include "cut_clear.cpp"

#include <cstring>

int engineScissory1 = 5;
int engineScissory2 = 5;
unsigned char cutMode = 3;
unsigned char cutSkipMode = 4;
unsigned char animationLoading = 0;
unsigned char thereIsACutPlayer = 1;
CutAnimSlot anim[2];

static char trace[256];
static int traceLength = 0;
static void note(const char* text) {
    while (*text) trace[traceLength++] = *text++;
    trace[traceLength] = 0;
}
static float frustum[4];
static float farClip = 0, lightScale = 0;
static void* freed[2];
static int freedCount = 0;

extern "C" void cutRunFrameTask__Fv() {}
extern "C" void cutDrawFrame__Fv() {}
extern "C" void engineRemoveTask__FPFv_v(void (*task)()) {
    note(task == cutRunFrameTask__Fv ? "R" : task == cutDrawFrame__Fv ? "D" : "?");
}
extern "C" void worldSetFarClip__Ff(float d) { farClip = d; note("F"); }
extern "C" void worldSetFrustum__Fffff(float a, float b, float c, float d) {
    frustum[0] = a; frustum[1] = b; frustum[2] = c; frustum[3] = d; note("S");
}
extern "C" void lightGlobalCharLightScale__Ff(float s) { lightScale = s; note("L"); }
extern "C" void cdWait__Fv() { note("W"); }
extern "C" void memFree__FPv(void* p) { freed[freedCount++] = p; note("M"); }

int main() {
    char a, b;
    anim[0] = {&a, 7};
    anim[1] = {&b, 9};
    animationLoading = 1;
    cutClear__Fv();
    if (std::strcmp(trace, "RDFSLWMM") != 0) return 1;
    if (engineScissory2 != 447 || engineScissory1 != 0) return 2;
    if (cutMode != 0 || cutSkipMode != 0 || thereIsACutPlayer != 0) return 3;
    if (farClip != 1800.0f || lightScale != 1.0f) return 4;
    if (frustum[0] != 50.0f || frustum[1] != 1.33f || frustum[2] != 80.0f || frustum[3] != 1.0f) return 5;
    if (animationLoading != 0) return 6;
    if (freedCount != 2 || freed[0] != &a || freed[1] != &b) return 7;
    for (int i = 0; i < 2; ++i)
        if (anim[i].block != nullptr || anim[i].second != 0) return 8;
    // Empty slots are not freed and a second clear does not wait.
    traceLength = 0; trace[0] = 0; freedCount = 0;
    animationLoading = 1;
    cutMode = 1;
    cutClear__Fv();
    if (std::strcmp(trace, "RDFSL") != 0 || freedCount != 0 || animationLoading != 1) return 9;
    // Only the second slot loaded.
    anim[1] = {&b, 1};
    traceLength = 0; trace[0] = 0;
    animationLoading = 0;
    cutClear__Fv();
    if (std::strcmp(trace, "RDFSLM") != 0 || freed[0] != &b) return 10;
    return 0;
}
