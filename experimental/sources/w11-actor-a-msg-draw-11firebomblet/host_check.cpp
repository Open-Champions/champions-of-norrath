// Host contract for msg_draw__11FireBomblet; not an EE input. 32-bit words are offsets from a host base; callees
// log their arguments and tag the matrix so the order of the matrix calls is visible. The authored function was
// also compared with the original in a private R5900 run. Returns non-zero when wrong.
#define W11_ACTOR_A_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w11_actor_a_host_base;
#include "msg_draw.cpp"

static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x400;
static std::string calls;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static float* lastMatrix;
extern "C" {
void lightSetAmbientOffset__FPs(short* v) { calls += "amb(" + num(v[0]) + "," + num(v[1]) + "," + num(v[2]) + "," + num(v[3]) + ");"; }
void lightSetAmbientOnly__Fv() { calls += "only;"; }
void matIdent__FP8Matrix34(void* m) { lastMatrix = static_cast<float*>(m); for (int i = 0; i < 12; ++i) lastMatrix[i] = static_cast<float>(i); calls += "ident;"; }
void matTrans__FP8Matrix34fff(void* m, float x, float y, float z) {
    calls += std::string(m == lastMatrix ? "" : "BADM") + "trans(" + num(x) + "," + num(y) + "," + num(z) + ");"; lastMatrix[3] = 77;
}
void matRot__FP8Matrix34is(void* m, int axis, short a) {
    calls += std::string(m == lastMatrix ? "" : "BADM") + "rot(" + num(axis) + "," + num(a) + "," + num(lastMatrix[3]) + ");"; lastMatrix[3] += 1;
}
void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 p, void* m, void* anim, unsigned long long flags, void* rec, int extra) {
    calls += "draw(" + num(off(model)) + "," + num(off(texture)) + "," + num(mode) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," +
             (m == lastMatrix ? "m" + num(lastMatrix[3]) : "BADM") + "," + (anim ? "anim" : "null") + "," + std::to_string(flags) + "," +
             (rec ? "rec" : "null") + "," + num(extra) + ");";
}
void lightResetOptionalParams__Fv() { calls += "reset;"; }
}
int main() {
    int bad = 0;
    for (int k = 0; k < 3; ++k) {
        std::memset(arena, 0, sizeof arena); w11_actor_a_host_base = arena; calls.clear();
        float pos[3] = {1.5f + k, -2.25f, 3.0f * k}; std::memcpy(obj + 16, pos, 12);
        short rz = static_cast<short>(100 + k), rx = static_cast<short>(-7 * k - 1); std::memcpy(obj + 56, &rz, 2); std::memcpy(obj + 168, &rx, 2);
        unsigned group = 0x800; std::memcpy(obj + 36, &group, 4);
        unsigned model = 0x900 + k, texture = 0xa00 + k; std::memcpy(arena + group + 12, &model, 4); std::memcpy(arena + group + 20, &texture, 4);
        unsigned long long fl = 0x123456789abcdefull + k; std::memcpy(obj + 176, &fl, 8);
        msg_draw__11FireBomblet(obj);
        std::string want = "amb(16,16,16,0);only;ident;trans(" + num(pos[0]) + "," + num(pos[1]) + "," + num(pos[2]) + ");rot(2," + num(rz) + ",77);rot(0," + num(rx) +
                           ",78);draw(" + num(0x900 + k) + "," + num(0xa00 + k) + ",0," + num(pos[0]) + ":" + num(pos[1]) + ":" + num(pos[2]) + ",m79,null," +
                           std::to_string(fl) + ",null,0);reset;";
        if (calls != want) { std::printf("fail k=%d\n  want %s\n  got  %s\n", k, want.c_str(), calls.c_str()); bad = 1; }
    }
    return bad;
}
