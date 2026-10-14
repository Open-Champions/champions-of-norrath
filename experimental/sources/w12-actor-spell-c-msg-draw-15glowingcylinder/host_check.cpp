// Host contract for msg_draw__15GlowingCylinder; not an EE input. The callees log. The authored function was
// also compared with the original in a private R5900 run (39 scenarios over the three countdown ranges and
// their edges, several frames; 209 of 210 original words reached, the missing one padding).
#include <cstdio>
#include <cstring>
#include <string>
static std::string calls;
extern "C" int frameNm = 3;
#include "msg_draw.cpp"

static std::string num(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
extern "C" {
void lightSetAmbientColor__FPs(short* c) { calls += "amb(" + std::to_string(c[0]) + "," + std::to_string(c[1]) + "," + std::to_string(c[2]) + "," + std::to_string(c[3]) + ");"; }
void lightResetOptionalParams__Fv() { calls += "reset;"; }
void matMakeTrans__FP8Matrix34G6Point3(Matrix34*, Point3 p) { calls += "trans(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + ");"; }
void matRot__FP8Matrix34is(Matrix34*, int axis, short a) { calls += "rot(" + std::to_string(axis) + "," + std::to_string(a) + ");"; }
void matScale__FP8Matrix34fff(Matrix34*, float x, float y, float z) { calls += "scale(" + num(x) + "," + num(y) + "," + num(z) + ");"; }
void* lumpFindResource__FPcT0(char* a, char* b) { calls += std::string("lump(") + a + "," + b + ");"; return b[9] == 'v' ? reinterpret_cast<void*>(0x10) : reinterpret_cast<void*>(0x20); }
void modelDrawEffect__FP12_modelHeaderP8_textureiP8Matrix34fP14AnimationStateUl(void* m, void* t, int a, Matrix34*, float f, void* an, unsigned long fl) {
    calls += "draw(" + std::to_string(reinterpret_cast<unsigned long>(m)) + "," + std::to_string(reinterpret_cast<unsigned long>(t)) + "," + std::to_string(a) + "," + num(f) +
             "," + std::to_string(an == nullptr) + "," + std::to_string(static_cast<int>(fl)) + ");";
}
}

static unsigned char obj[256] __attribute__((aligned(16)));
static void putf(int off, float v) { std::memcpy(obj + off, &v, 4); }
static void put16(int off, int v) { short s = static_cast<short>(v); std::memcpy(obj + off, &s, 2); }
static void put32(int off, int v) { std::memcpy(obj + off, &v, 4); }
static void reset(int n, int r, int g, int b) {
    std::memset(obj, 0, sizeof obj); calls.clear();
    put32(76, n); put16(80, r); put16(82, g); put16(84, b);
    putf(16, 1); putf(20, 2); putf(24, 3);
}

int main() {
    int bad = 0;
    // 10 <= n < 61: colour unchanged, scale 1.0 + 1.0 = 2
    reset(30, 1000, -20, 300);
    frameNm = 3;
    msg_draw__15GlowingCylinder(obj);
    const std::string expect =
        "amb(1000,-20,300,0);trans(1:2:3);rot(2,384);scale(0.8,0.8,2);lump(fx.lmp,cylinder.vif);lump(fx.lmp,cylinder.tex);draw(16,32,3,2000,1,-1);"
        "trans(1:2:3);rot(2,-435);scale(1,1,2);draw(16,32,3,2000,1,-1);"
        "trans(1:2:3);rot(2,472);scale(1.2,1.2,2);draw(16,32,3,2000,1,-1);reset;";
    if (calls.find("amb(1000,-20,300,0);") != 0) { std::puts(calls.c_str()); std::puts("fail: middle colour"); bad = 1; }
    if (calls != expect) { std::puts(calls.c_str()); std::puts("fail: sequence for n = 30"); bad = 1; }
    // n >= 61: s = (70 - 65) * 0.1f = 0.49999997 (0.1f is just below 0.1), so each colour truncates one low; scale 1.5
    reset(65, 1001, -21, 300);
    msg_draw__15GlowingCylinder(obj);
    if (calls.find("amb(500,-10,149,0);") != 0 || calls.find("scale(0.8,0.8,1.5);") == std::string::npos) { std::puts(calls.c_str()); std::puts("fail: n = 65"); bad = 1; }
    // n < 10: s = 4 * 0.1f = 0.39999998
    reset(4, 1000, -20, 300);
    msg_draw__15GlowingCylinder(obj);
    if (calls.find("amb(399,-7,119,0);") != 0 || calls.find("scale(0.8,0.8,1.4);") == std::string::npos) { std::puts(calls.c_str()); std::puts("fail: n = 4"); bad = 1; }
    // the third angle truncates towards zero for a negative frame: -5 * 630 / 4 = -787 (not -788)
    reset(30, 0, 0, 0);
    frameNm = -5;
    msg_draw__15GlowingCylinder(obj);
    if (calls.find("rot(2,-787);") == std::string::npos || calls.find("rot(2,725);") == std::string::npos || calls.find("rot(2,-640);") == std::string::npos) { std::puts(calls.c_str()); std::puts("fail: negative frame angles"); bad = 1; }
    return bad;
}
