// Observed moveCamera__Fv at 0x0027e040, 1932 bytes, 352-byte frame. The per-frame camera move of the character
// selection screens. State is in file-local words of the original unit: `src` and `dst` (the two positions blended
// between), `hermiteTime` (blend position), `delta` (direction and speed; zero when idle) and `hermDeltaScale`. Returns 1
// when `delta` was nonzero on entry and 0 when it was zero.
//  1. delta above zero with src equal to dst sets hermiteTime to 1; delta below zero with src equal to dst sets it to 0.
//  2. getTweeks(src, &yawA, &pitchA, &tweakA) and getTweeks(dst, &yawB, &pitchB, &tweakB) give a yaw and pitch offset
//     (shorts) and a three-float offset for each end.
//  3. The two ends of a Hermite curve are the points p1 + tweakA and p2 + tweakB; evalHermite3D(hermiteTime, copy of the
//     first, copy of the second, v1, v2, &position) fills position. t = evalHermite(hermiteTime, 0, 1, 0.5, 0.5).
//  4. For each end i (src, dst), a view direction is the vector (0, -1, 0) transformed by the matrix lightMats[i] (nine
//     floats, 36-byte stride) rotated about axis 2 by (short)(int)(lightInfo[i].f4 / 360 * 65536) and then about axis 0 by
//     (short)(int)(lightInfo[i].f0 / 360 * 65536). lightInfo has a 32-byte stride, floats at +0 to +28.
//  5. lightSetSceneLights is called with the direction lerp(dirSrc, dirDst, t) and six integers made by rounding half
//     away from zero lerp(lightInfo[src].f, lightInfo[dst].f, t) for the fields +8, +12, +16, (0), +20, +24, +28, then 0 and
//     128 on the stack (the same shape of call as the stats screen's).
//  6. Camera::SetYaw(gameCam, lerpShortRot(yaws[src] + yawA, yaws[dst] + yawB, t, -1, -1)) and Camera::SetPitch(gameCam,
//     lerpShortRot(pitch1 + pitchA, pitch2 + pitchB, t, -1, -1)), then Camera::BuildMatrix(gameCam, position, null).
//  7. hermiteTime += delta * hermDeltaScale * 0.99999994 (the word 0x3f7fffff). Above 1: hermiteTime = 0, initPosition(dst,
//     src) is called, and delta = 0. Then below 0: hermiteTime = 0 and delta = 0. (initPosition may itself change them; the
//     comparison reads hermiteTime again after the call.)
// Inferred: this slides the camera between the stored look-at positions of two characters with an eased blend and
// stops when the blend leaves 0..1. The names p1, p2, v1, v2, src, dst, lightInfo, lightMats, yaws, pitch1 and pitch2 come
// from the symbol table; their meaning is a guess from use only.
// Unknown: the layout of lightInfo and lightMats beyond the fields read, the meaning of 1/360 * 65536 beyond an angle
// conversion, what the constant (0, -1, 0) denotes, and getTweeks' behavior (an import here).
extern "C" {
extern int src;
extern int dst;
extern float hermiteTime;
extern float delta;
extern float hermDeltaScale;
extern float p1[];
extern float p2[];
extern float v1[];
extern float v2[];
extern float lightInfo[];   // 32-byte records
extern float lightMats[];   // 36-byte records
extern short yaws[];
extern unsigned short pitch1;
extern unsigned short pitch2;
extern void* gameCam;

void getTweeks__FiPsT1P6Point3(int index, short* yaw, short* pitch, float* vector);
void evalHermite3D__FfP6Point3N41(float t, const float* a, const float* b, const float* c, const float* d, float* out);
float evalHermite__Ffffff(float t, float a, float b, float c, float d);
void matRot__FP8Matrix33is(float* matrix, int axis, short angle);
void matTransform__FP6Point3P8Matrix33T0(float* out, float* matrix, const float* in);
int lerpShortRot__Fssfff(short a, short b, float t, float c, float d);
void SetYaw__6Camerai(void* camera, int yaw);
void SetPitch__6Camerai(void* camera, int pitch);
void BuildMatrix__6CamerafffP6Point3(void* camera, float x, float y, float z, float* point);
void lightSetSceneLights__FG6Point3iiiiiiiii(const void* direction, int a, int b, int c, int d, int e, int f, int g, int h, int i);
void initPosition__Fii(int current, int target);
}

namespace {
constexpr float bits(unsigned word) { return __builtin_bit_cast(float, word); }

inline int roundAway(float value) { return static_cast<int>(value >= 0.0f ? value + 0.5f : value - 0.5f); }

inline float field(int record, int offset) { return lightInfo[record * 8 + offset / 4]; }

inline int lerpField(int offset, float t) {
    float from = field(src, offset);
    return roundAway(from + (field(dst, offset) - from) * t);
}

// The direction vector of one end: (0, -1, 0) through the record's matrix rotated about axis 2 then axis 0.
inline void direction(int record, float* out, const float* up) {
    float matrix[9];
    for (int i = 0; i < 9; ++i) matrix[i] = lightMats[record * 9 + i];
    matRot__FP8Matrix33is(matrix, 2, static_cast<short>(static_cast<int>(field(record, 4) * bits(0x3b360b60u) * 65536.0f)));
    matRot__FP8Matrix33is(matrix, 0, static_cast<short>(static_cast<int>(field(record, 0) * bits(0x3b360b60u) * 65536.0f)));
    matTransform__FP6Point3P8Matrix33T0(out, matrix, up);
}
}

extern "C" int moveCamera__Fv() {
    const int moving = delta != 0.0f ? 1 : 0;
    const float up[3] = {0.0f, -1.0f, 0.0f};
    if (0.0f < delta && dst == src) hermiteTime = 1.0f;
    if (delta < 0.0f && dst == src) hermiteTime = 0.0f;
    short yawA, yawB, pitchA, pitchB;
    float tweakA[3], tweakB[3];
    getTweeks__FiPsT1P6Point3(src, &yawA, &pitchA, tweakA);
    getTweeks__FiPsT1P6Point3(dst, &yawB, &pitchB, tweakB);
    float from[3] = {p1[0] + tweakA[0], p1[1] + tweakA[1], p1[2] + tweakA[2]};
    float to[3] = {p2[0] + tweakB[0], p2[1] + tweakB[1], p2[2] + tweakB[2]};
    float position[3];
    evalHermite3D__FfP6Point3N41(hermiteTime, from, to, v1, v2, position);
    const float t = evalHermite__Ffffff(hermiteTime, 0.0f, 1.0f, 0.5f, 0.5f);
    float directionSrc[3], directionDst[3];
    direction(src, directionSrc, up);
    direction(dst, directionDst, up);
    float light[3];
    for (int i = 0; i < 3; ++i) light[i] = directionSrc[i] + (directionDst[i] - directionSrc[i]) * t;
    lightSetSceneLights__FG6Point3iiiiiiiii(light, lerpField(8, t), lerpField(12, t), lerpField(16, t), 0, lerpField(20, t),
                                            lerpField(24, t), lerpField(28, t), 0, 128);
    SetYaw__6Camerai(gameCam, lerpShortRot__Fssfff(static_cast<short>(yaws[src] + yawA), static_cast<short>(yaws[dst] + yawB), t,
                                                  -1.0f, -1.0f));
    SetPitch__6Camerai(gameCam, lerpShortRot__Fssfff(static_cast<short>(pitch1 + pitchA), static_cast<short>(pitch2 + pitchB), t,
                                                    -1.0f, -1.0f));
    BuildMatrix__6CamerafffP6Point3(gameCam, position[0], position[1], position[2], nullptr);
    float next = hermiteTime + delta * hermDeltaScale * bits(0x3f7fffffu);
    if (1.0f < next) {
        hermiteTime = 0.0f;
        initPosition__Fii(dst, src);
        delta = 0.0f;
        next = hermiteTime;
    } else {
        hermiteTime = next;
    }
    if (next < 0.0f) {
        hermiteTime = 0.0f;
        delta = 0.0f;
    }
    return moving;
}
