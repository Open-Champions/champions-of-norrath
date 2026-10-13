// Host contract for msg_draw__9Savepoint; not an EE input. Words holding addresses are offsets
// from a host base. Callees log; the contract walks the zero and nonzero w cases (including the
// clamp of the ambient offset at 16), the matrix contents seen by the draw calls and the position
// copied to object+184.
#define W10_ACTOR_L_HOST 1
#include <cstring>
#include <string>
unsigned char* w10_actor_l_host_base;
unsigned w10_actor_l_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::string calls; static int nm = 3; static float sin_ret = 0.5f; static int seen_angle;
static unsigned off(const void* p) { return p ? static_cast<unsigned>(static_cast<const unsigned char*>(p) - w10_actor_l_host_base) : 0u; }
extern "C" {
void* haloTexture2 = reinterpret_cast<void*>(0x77);
void lightSetAmbientOffset__FPs(short* v) { calls += "amb(" + std::to_string(v[0]) + "," + std::to_string(v[1]) + "," + std::to_string(v[2]) + "," + std::to_string(v[3]) + ");"; }
void matMakeTransYaw__FP8Matrix34G6Point3s(void* m, const void* p, short yaw) { float v[3]; std::memcpy(v, p, 12); calls += "mat(" + std::to_string(v[0]) + "," + std::to_string(v[2]) + "," + std::to_string(yaw) + ");"; float* f = static_cast<float*>(m); for (int i = 0; i < 12; ++i) f[i] = 100.0f + i; }
void* modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(void* model, int v, void* m, void* state, char last) { calls += "shadow(" + std::to_string(off(model)) + "," + std::to_string(v) + "," + std::to_string(static_cast<float*>(m)[5]) + "," + std::to_string(off(state)) + "," + std::to_string(last) + ");"; return reinterpret_cast<void*>(0x55); }
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(void* model, void* tex, int mode, const void* p, void* m, void* anim, unsigned long long flags, void* rec, int last) {
    float v[3]; std::memcpy(v, p, 12);
    calls += "draw(" + std::to_string(off(model)) + "," + std::to_string(off(tex)) + "," + std::to_string(mode) + "," + std::to_string(v[1]) + "," + std::to_string(static_cast<float*>(m)[11]) + "," + std::to_string(off(anim)) + "," + std::to_string(flags) + "," + std::to_string(reinterpret_cast<unsigned long>(rec)) + "," + std::to_string(last) + ");"; return 0; }
int modelGetNmShadowPoints__FP12_modelHeader(void*) { calls += "nm;"; return nm; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void*, void* state, const void* p, short yaw, int idx, void* out, float* scalar, void* m, float s) {
    float v[3]; std::memcpy(v, p, 12); calls += "spos(" + std::to_string(off(state)) + "," + std::to_string(v[0]) + "," + std::to_string(yaw) + "," + std::to_string(idx) + "," + std::to_string(m == nullptr) + "," + std::to_string(s) + ");";
    float o3[3] = {7.0f, 8.0f, 9.0f}; std::memcpy(out, o3, 12); *scalar = 1.0f; }
float isin__Fi(int a) { seen_angle = a; calls += "sin;"; return sin_ret; }
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const void* p, float a, float b, int c, int d, int e, int f, void* t, short g, int h) {
    float v[3]; std::memcpy(v, p, 12); calls += "face(" + std::to_string(v[0]) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(e) + "," + std::to_string(f) + "," + std::to_string(reinterpret_cast<unsigned long>(t)) + "," + std::to_string(g) + "," + std::to_string(h) + ");"; }
void lightUpdatePosition__FP5Light(void* l) { calls += "upd(" + std::to_string(off(l)) + ");"; }
void lightResetOptionalParams__Fv() { calls += "reset;"; }
}
#include "msg_draw.cpp"
alignas(16) static unsigned char arena[0x2000];
int main() {
    w10_actor_l_host_base = arena; unsigned char* const o = arena + 0x100; int bad = 0;
    auto setup = [&](int w) {
        std::memset(arena, 0, sizeof arena); calls.clear();
        unsigned g = 0x800, m = 0xa00, t = 0xb00; std::memcpy(o + 36, &g, 4); std::memcpy(arena + g + 12, &m, 4); std::memcpy(arena + g + 20, &t, 4);
        float p[3] = {1.5f, 2.5f, 3.5f}; std::memcpy(o + 16, p, 12); short yaw = -4; std::memcpy(o + 56, &yaw, 2);
        std::memcpy(o + 128, &w, 4); unsigned long long d = 0x1122334455667788ull; std::memcpy(o + 120, &d, 8);
    };
    setup(0); msg_draw__9Savepoint(o);
    if (calls != "amb(-32,-32,-32,0);mat(1.500000,3.500000,-4);shadow(2560,0,105.000000,0,0);draw(2560,2816,0,2.500000,111.000000,0,1234605616436508552,85,0);reset;") bad = 1;
    for (int w : {1, 2, 12, 13, 100}) {
        setup(w); sin_ret = 0.5f; msg_draw__9Savepoint(o);
        const int t = w * 4 - 32 > 16 ? 16 : w * 4 - 32;
        const std::string a = "amb(" + std::to_string(t) + "," + std::to_string(t) + "," + std::to_string(t) + ",0);";
        const std::string rest = "mat(1.500000,3.500000,-4);shadow(2560,0,105.000000,332,0);draw(2560,2816,0,2.500000,111.000000,332,1234605616436508552,85,0);nm;spos(332,1.500000,-4,2,1,1.000000);sin;face(7.000000,18.000000,19000.000000,100,100,188,64,119,0,2);upd(" + std::to_string(0x100 + 136) + ");reset;";
        if (calls != a + rest || seen_angle != (w << 9)) bad = 1;
        float out[3]; std::memcpy(out, o + 184, 12); if (out[0] != 7.0f || out[1] != 8.0f || out[2] != 9.0f) bad = 1;
    }
    return bad;
}
