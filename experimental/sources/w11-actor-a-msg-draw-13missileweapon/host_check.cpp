// Host contract for msg_draw__13MissileWeapon; not an EE input. 32-bit words are offsets from a host base; every
// callee logs its arguments (matrix calls also log which stack matrix they touched). The authored function was
// also compared with the original in a private R5900 run (160 scenarios). Build with -ffp-contract=off. Returns
// non-zero when wrong.
#define W11_ACTOR_A_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w11_actor_a_host_base;
#include "msg_draw.cpp"

signed char modelClippingOn;
int levelFlags, frameNm;
void* haloTexture1;
static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static unsigned modelResult;
static float* lastMatrix;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static std::string pt(const Point3& p) { return num(p.x) + ":" + num(p.y) + ":" + num(p.z); }
static unsigned off(const void* p) { return p ? static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena) : 0u; }
static std::string mtag(void* m) { if (!lastMatrix) lastMatrix = static_cast<float*>(m); return m == lastMatrix ? "M" : "?"; }
unsigned w11_actor_a_host_call(unsigned fn, void* adjusted, int argc, unsigned long, unsigned long) {
    calls += "vcall(" + num(fn) + "," + num(off(adjusted)) + "," + num(argc) + ");"; return modelResult;
}
extern "C" {
void matMakeTransYawScale__FP8Matrix34G6Point3sf(void* m, Point3 p, short y, float s) { lastMatrix = nullptr; calls += "mk(" + mtag(m) + "," + pt(p) + "," + num(y) + "," + num(s) + ");"; }
void matRot__FP8Matrix34is(void* m, int axis, short a) { calls += "rot(" + mtag(m) + "," + num(axis) + "," + num(a) + ");"; }
void modelSetHalo__FP7HaloDefi(void* h, int a) { calls += "halo(" + num(off(h)) + "," + num(a) + ");"; }
void lightSetAmbientOffset__FPs(short* v) { calls += "amb(" + num(v[0]) + "," + num(v[1]) + "," + num(v[2]) + "," + num(v[3]) + ");"; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void* m, void* a, Point3 p, short h, int i, Point3* out, float* v, void* mat, float s) {
    calls += "shadow(" + num(reinterpret_cast<__UINTPTR_TYPE__>(m)) + "," + num(off(a)) + "," + pt(p) + "," + num(h) + "," + num(i) + "," + (v ? "val" : "null") + "," + num(s) + ");";
    *out = Point3{11, 12, 13}; float* r = static_cast<float*>(mat); for (int k = 0; k < 9; ++k) r[k] = static_cast<float>(k);
}
void matMul__FP8Matrix33N20(void* d, void* a, void* b) { calls += std::string("mul33(") + (d == a ? "same" : "diff") + "," + num(off(b)) + ");"; }
void matMakeTransYaw__FP8Matrix34G6Point3s(void* m, Point3 p, short y) { calls += "tyaw(" + mtag(m) + "," + pt(p) + "," + num(y) + ");"; }
void matMul__FP8Matrix34P8Matrix33T0(void* d, void*, void* m) { calls += "mul34(" + mtag(d) + mtag(m) + ");"; }
void matTrans__FP8Matrix34fff(void* m, float x, float y, float z) { calls += "trans(" + mtag(m) + "," + num(x) + "," + num(y) + "," + num(z) + ");"; }
void matScale__FP8Matrix34fff(void* m, float x, float y, float z) { calls += "scale(" + mtag(m) + "," + num(x) + "," + num(y) + "," + num(z) + ");"; }
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* d, void* m, void* s, Point3 p, short h, void* second, void* anim, float a, int b, int c, float e, void* mat) {
    calls += "fx(" + num(off(d)) + "," + num(off(m)) + "," + num(off(s) - 0x1000) + "," + pt(p) + "," + num(h) + "," + num(off(second)) + num(off(anim)) + "," + num(a) + "," + num(b) +
             "," + num(c) + "," + num(e) + "," + mtag(mat) + ");";
}
void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* m, void* t, int mode, Point3 p, void* mat, void* anim, unsigned long long flags, void* rec, int extra) {
    calls += "draw(" + num(off(m)) + "," + num(off(t)) + "," + num(mode) + "," + pt(p) + "," + mtag(mat) + "," + (anim ? "anim" : "null") + "," + std::to_string(flags) + "," + (rec ? "rec" : "null") + "," + num(extra) + ");";
}
void lightResetOptionalParams__Fv() { calls += "lres;"; }
void modelResetOptionalParams__Fv() { calls += "mres;"; }
float isin__Fi(int a) { calls += "isin(" + num(a) + ");"; return 0.5f; }
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 p, float a, float b, int c0, int c1, int c2, int alpha, void* tex, short ang, int mode) {
    calls += "sprite(" + pt(p) + "," + num(a) + "," + num(b) + "," + num(c0) + "," + num(c1) + "," + num(c2) + "," + num(alpha) + "," + num(off(tex)) + "," + num(ang) + "," + num(mode) + ");";
}
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putf(unsigned o, float v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
int main() {
    int bad = 0;
    for (int variant = 0; variant < 12; ++variant) {
        const int kind = (int[]){2, 5, 6, 0, 3, 4, 2, 2, 2, 2, 5, 2}[variant];
        const bool hasX = variant == 6 || variant == 7, modelOk = variant != 7, hasY = variant >= 8 && variant != 11, byte248 = variant == 9, rotOn = variant == 10;
        const bool flag100 = variant == 1 || variant == 5;
        std::memset(arena, 0, sizeof arena); w11_actor_a_host_base = arena; calls.clear(); lastMatrix = nullptr;
        modelResult = modelOk ? 0x500 : 0;
        putf(16, 1); putf(20, 2); putf(24, 3);
        *reinterpret_cast<short*>(obj + 56) = 77; *reinterpret_cast<short*>(obj + 250) = rotOn; *reinterpret_cast<short*>(obj + 252) = -9; obj[254] = static_cast<unsigned char>(-3);
        obj[248] = byte248; put32(208, kind); put32(244, 5); put32(444, 0x600); put32(448, 0x700);
        putf(428, 10); putf(432, 20); putf(436, 30);
        levelFlags = flag100 ? 0x100 : 0; frameNm = 0x3001 + variant; haloTexture1 = arena + 0x7000;
        if (hasX) { put32(292, 0x2000); putA(0x2000 + 12, 0x2800); putA(0x2800 + 72, 0x3000); *reinterpret_cast<short*>(arena + 0x3000 + 128) = 4; putA(0x3000 + 132, 0x4444); putf(0, 0); }
        if (hasY) { put32(296, 0x3800); putA(0x3800, 1); }
        // The 'T' object is at 0x2800: position floats, heading and scale at +16, +56, +224.
        if (hasX) { float tp[3] = {7, 8, 9}; std::memcpy(arena + 0x2800 + 16, tp, 12); *reinterpret_cast<short*>(arena + 0x2800 + 56) = 33; float sc = 0.25f; std::memcpy(arena + 0x2800 + 224, &sc, 4); }
        msg_draw__13MissileWeapon(obj);
        std::string want = "mk(M,1:2:3,77,1);";
        if (rotOn) want += "rot(M,-3,-9);";
        want += "halo(0,0);";
        want += std::string("amb(") + (flag100 ? "40,40,40,60" : "32,32,32,0") + ");";
        if (hasX) {
            want += "vcall(17476,10244,0);";
            if (modelOk) want += "shadow(1280,10528,7:8:9,33,5,null,0.25);mul33(same,4352);tyaw(M,11:12:13,0);mul34(MM);trans(M,0,-24,0);scale(M,3,1,3);";
        } else if (hasY) {
            if (byte248) want += "fx(14336,1536,300,1:2:3,77,00,0,0,0,1,M);";
            else want += "fx(14336,1536,300,1:2:3,77,00,1,1,0,1,M);";
        }
        want += "draw(1536,1792,1,1:2:3,M,null,18446744073709551615,null,0);lres;mres;";
        if (!hasX && (kind == 2 || kind == 5 || kind == 6)) {
            for (int i = 0; i < 10; ++i) {
                const float scale = 0.08f - static_cast<float>(i) * 0.01f;
                const int c0 = kind == 2 ? 255 : 50, c2 = kind == 2 ? 50 : 255;
                const int ang = static_cast<short>(static_cast<int>(static_cast<unsigned>(static_cast<unsigned short>(frameNm)) << 25) >> 16);
                want += "isin(" + num(frameNm << 10) + ");sprite(" + num(1 + 10 * scale) + ":" + num(2 + 20 * scale) + ":" + num(3 + 30 * scale) + ",5," + num(static_cast<float>(5000 - 200 * i) + 0.5f * 250.0f) + "," + num(c0) +
                        ",100," + num(c2) + ",128,28672," + num(ang) + ",3);";
            }
        }
        if (calls != want || modelClippingOn != 1) { std::printf("fail variant %d\n  want %s\n  got  %s\n", variant, want.c_str(), calls.c_str()); bad = 1; }
        if (hasY && !hasX && byte248 != (obj[248] != 0 ? 1 : 0) && obj[248] != 0) { std::puts("fail: byte 248 not cleared"); bad = 1; }
    }
    return bad;
}
