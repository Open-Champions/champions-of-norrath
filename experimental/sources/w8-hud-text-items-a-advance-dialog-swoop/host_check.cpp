// Host contract for AdvanceDialogSwoop__6CameraiPf. Not an EE plan input.
#include <cstring>
#include "advance_swoop.cpp"
static int herm, lerps, builds;
static float hermArgs[5], lerpAmount[2], lerpA[2], lerpB[2]; static short lerpFrom[2], lerpTo[2];
static float bx, by, bz; static void* bcam; static void* bptr;
extern "C" {
float evalHermite__Ffffff(float t, float a, float b, float c, float d) {
    ++herm; hermArgs[0] = t; hermArgs[1] = a; hermArgs[2] = b; hermArgs[3] = c; hermArgs[4] = d; return t * 0.5f;
}
short lerpShortRot__Fssfff(short f, short t, float amt, float a, float b) {
    int n = lerps++; lerpFrom[n & 1] = f; lerpTo[n & 1] = t; lerpAmount[n & 1] = amt; lerpA[n & 1] = a; lerpB[n & 1] = b;
    return static_cast<short>(1000 * (n + 1));
}
void BuildMatrix__6CamerafffP6Point3(void* c, float x, float y, float z, void* p) { ++builds; bcam = c; bx = x; by = y; bz = z; bptr = p; }
}
alignas(8) static unsigned char cam[256];
static void setF(int off, float v) { std::memcpy(cam + off, &v, 4); }
static float getF(int off) { float v; std::memcpy(&v, cam + off, 4); return v; }
static void setH(int off, short v) { std::memcpy(cam + off, &v, 2); }
static short getH(int off) { short v; std::memcpy(&v, cam + off, 2); return v; }
static void init(float progress, float rate) {
    std::memset(cam, 0, sizeof cam); herm = lerps = builds = 0;
    setF(0, 2.0f); setF(4, 4.0f); setF(8, 8.0f); setF(12, 10.0f); setF(16, 20.0f); setF(20, 40.0f);
    setF(168, progress); setF(172, rate);
    setH(118, 11); setH(120, 12); setH(122, 13); setH(124, 14);
    setH(104, -5); setH(106, -6);
}
int main() {
    float out = -9.0f;
    // Finished forwards and backwards: nothing is touched.
    init(1.0f, 0.0f);
    if (AdvanceDialogSwoop__6CameraiPf(cam, 45, &out) != 1 || herm || lerps || builds || out != -9.0f || getF(168) != 1.0f) return 1;
    init(0.0f, 0.0f);
    if (AdvanceDialogSwoop__6CameraiPf(cam, -45, &out) != 1 || herm || lerps || builds || out != -9.0f) return 2;
    // Forward step of 1/4 with no rate: progress 0.25, s = 0.125.
    init(0.0f, 0.0f);
    if (AdvanceDialogSwoop__6CameraiPf(cam, 4, &out) != 0) return 3;
    if (getF(168) != 0.25f || herm != 1 || hermArgs[0] != 0.25f || hermArgs[1] != 0.0f || hermArgs[2] != 1.0f || hermArgs[3] != 0.0f || hermArgs[4] != 0.0f) return 4;
    if (out != 0.125f) return 5;
    // First lerp: (h124, h122, s^3), second: (h118, h120, s); both use -1, -1.
    if (lerpFrom[0] != 14 || lerpTo[0] != 13 || lerpAmount[0] != 0.125f * 0.125f * 0.125f || lerpA[0] != -1.0f || lerpB[0] != -1.0f) return 6;
    if (lerpFrom[1] != 11 || lerpTo[1] != 12 || lerpAmount[1] != 0.125f || lerpA[1] != -1.0f || lerpB[1] != -1.0f) return 7;
    if (getH(106) != 1000 || getH(104) != 2000) return 8;
    if (builds != 1 || bcam != cam || bptr != nullptr || bx != 2.0f + 8.0f * 0.125f || by != 4.0f + 16.0f * 0.125f || bz != 8.0f + 32.0f * 0.125f) return 9;
    // The rate damps the step: 0.25 - 0.25*4*0.75 = -0.5 -> progress raised to 0.
    init(0.0f, 4.0f);
    AdvanceDialogSwoop__6CameraiPf(cam, 4, nullptr);
    if (getF(168) != 0.0f || hermArgs[0] != 0.0f) return 10;
    // Cap at 1.0 going forward (progress .75 + .5).
    init(0.75f, 0.0f);
    AdvanceDialogSwoop__6CameraiPf(cam, 2, &out);
    if (getF(168) != 1.0f || hermArgs[0] != 1.0f || out != 0.5f) return 11;
    // Backward: negative count subtracts; negative result clamps to zero.
    init(0.75f, 0.0f);
    AdvanceDialogSwoop__6CameraiPf(cam, -2, &out);
    if (getF(168) != 0.25f || hermArgs[0] != 0.25f) return 12;
    init(0.25f, 0.0f);
    AdvanceDialogSwoop__6CameraiPf(cam, -2, &out);
    if (getF(168) != 0.0f || hermArgs[0] != 0.0f || out != 0.0f) return 13;
    // A count of one finishes in one step.
    init(0.5f, 0.0f);
    AdvanceDialogSwoop__6CameraiPf(cam, 1, &out);
    if (getF(168) != 1.0f) return 14;
    return 0;
}
