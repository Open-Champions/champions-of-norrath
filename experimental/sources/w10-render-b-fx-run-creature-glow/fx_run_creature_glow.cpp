// Observed fxRunCreatureGlow__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffPsT9ifi
// at 0x001db2d8, extent 948. Arguments in order: object, model, animation, a Point3 (pointer to a
// caller copy), a halfword angle, the FxCreatureState, floats a, b, c, two arrays of four halfwords
// (colorsA, colorsB), an int mode, a float scale and an int value. The state pointer and float a
// are not read. mode and value arrive as the first two stack words; scale is the fourth float
// argument. Offsets are from the object address.
//  1. position = *Point3. Returns at once when gameCloseToCamera(&position) is zero, when model is
//     null, or when modelGetNmShadowPoints(model) (n) is zero. Then position.x += icos(angle) * c
//     and position.y += isin(angle) * c (icos is called first). Returns when the object is null or
//     bit 0x20 of the word at +12 is clear.
//  2. X[k] and Y[k] are colorsA[k] and colorsB[k] converted to float, k = 0..3. With s = scale * 60:
//     mode 0: for each k, o108[k] = min(X[k], o108[k] + X[k] / s) (the compare is X < sum) and
//             o124[k] = min(Y[k], o124[k] + Y[k] / s), where o108 and o124 are the float arrays at
//             +108 and +124.
//     mode 2: o108[k] = max(0, o108[k] - X[k] / s) and o124[k] = max(0, o124[k] - Y[k] / s).
//     Other modes leave the arrays alone. The divisions read X, Y before either store.
//  3. For each shadow point index j = 0..n-1 (only when n > 0, so n < 0 does nothing):
//     modelGetShadowPos(model, animation, &position, angle, j, &point, &extra, 0, b), then
//     P_FaceSprite(&point, K0, ((isin(frameNm << 10) * 250.0f + 10000.0f) - 2013.0f) + (float)value,
//     r, g, b, a, haloTexture2, (short)(frameNm << 6), 3), where r, g, b, a are the truncated
//     floats at +124, +128, +132 and +136 in that order, and K0 = -12.856 (bits 0xc14db22d).
// Inferred: a halo glow around each shadow point whose colour is the object's current colour
// (+124..+136) and whose size pulses with the frame counter. Unknown: the roles of the
// two colour arrays, a, and the fields at +108..+136 beyond their use here.
struct Point3 {
    float x, y, z;
};

extern "C" {
extern int frameNm;
extern void* haloTexture2;
int gameCloseToCamera__FP6Point3(Point3* position);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
float icos__Fi(int angle);
float isin__Fi(int angle);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* animation, Point3* position, short yaw, int index, Point3* out_point, float* out_extra,
    void* matrix33, float scale);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3* position, float a, float b, int c, int d, int e, int f,
                                               void* texture, short g, int h);
}

namespace {
constexpr float K0 = __builtin_bit_cast(float, 0xc14db22du);
}

extern "C" void fxRunCreatureGlow__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffPsT9ifi(
    unsigned char* object, void* model, void* animation, Point3* positionArg, short angle, void* state, float a,
    float b, float c, short* colorsA, short* colorsB, int mode, float scale, int value) {
    Point3 position = *positionArg;
    if (!gameCloseToCamera__FP6Point3(&position)) return;
    if (!model) return;
    int n = modelGetNmShadowPoints__FP12_modelHeader(model);
    if (!n) return;
    float cosine = icos__Fi(angle);
    float sine = isin__Fi(angle);
    float vx = cosine * c;
    float vy = sine * c;
    position.x = position.x + vx;
    position.y = position.y + vy;
    if (!object) return;
    if (!(*reinterpret_cast<int*>(object + 12) & 0x20)) return;
    float X[4], Y[4];
    for (int k = 0; k < 4; ++k) {
        X[k] = static_cast<float>(colorsA[k]);
        Y[k] = static_cast<float>(colorsB[k]);
    }
    float* o108 = reinterpret_cast<float*>(object + 108);
    float* o124 = reinterpret_cast<float*>(object + 124);
    if (mode == 0) {
        float s = scale * 60.0f;
        for (int k = 0; k < 4; ++k) {
            float dx = X[k] / s;
            float dy = Y[k] / s;
            float u = o108[k] + dx;
            o108[k] = u;
            float v = o124[k] + dy;
            o124[k] = v;
            if (X[k] < u) o108[k] = X[k];
            if (Y[k] < o124[k]) o124[k] = Y[k];
        }
    } else if (mode == 2) {
        float s = scale * 60.0f;
        for (int k = 0; k < 4; ++k) {
            float dx = X[k] / s;
            float dy = Y[k] / s;
            float u = o108[k] - dx;
            o108[k] = u;
            float v = o124[k] - dy;
            o124[k] = v;
            if (u < 0.0f) o108[k] = 0.0f;
            if (o124[k] < 0.0f) o124[k] = 0.0f;
        }
    }
    if (n <= 0) return;
    float valueF = static_cast<float>(value);
    for (int j = 0; j < n; ++j) {
        Point3 point;
        float extra[4];
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
            model, animation, &position, angle, j, &point, extra, 0, b);
        float pulse = isin__Fi(frameNm << 10) * 250.0f;
        int r = static_cast<int>(o124[0]);
        int g = static_cast<int>(o124[1]);
        int bl = static_cast<int>(o124[2]);
        int al = static_cast<int>(o124[3]);
        P_FaceSprite__FG6Point3ffiiiiP8_texturesi(&point, K0, ((pulse + 10000.0f) - 2013.0f) + valueF, r, g, bl, al,
                                                  haloTexture2, static_cast<short>(frameNm << 6), 3);
    }
}
