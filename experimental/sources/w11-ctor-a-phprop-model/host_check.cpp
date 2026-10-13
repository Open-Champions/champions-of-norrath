// Host contract for the PhProp (model and bone placement) constructor; not an EE input. Stubs log
// the call order with their arguments; the expected log and stored fields are written out
// independently.
#define W11_HOST 1
#include "../w11-ctor-a-common/host_support.hpp"
#include "ctor.cpp"
#include <cstdarg>
#include <cstdio>
unsigned char vt_PhProp[8];
using namespace w11_host;
static unsigned char* the_obj;
static std::string num(float f) { return std::to_string(f); }
static std::string id(const void* p) { return std::to_string(p == the_obj); }
extern "C" void prop_construct(void* self, Point3 p, char* a, char* b, char* c, long n, int flags) {
    calls() += "prop(" + id(self) + "," + num(p.x) + "," + num(p.y) + "," + num(p.z) + "," + a + b + c + "," + std::to_string(n) + "," + std::to_string(flags) + ");";
    w11::f32(self, 16) = p.x + 0.5f;
    w11::f32(self, 20) = p.y;
    w11::f32(self, 24) = p.z;
}
extern "C" void initStuff__6PhPropi(void* self, int flags) { calls() += "init(" + id(self) + "," + std::to_string(flags) + ")[" + std::to_string(w11::u32(self, 12)) + "];"; }
extern "C" int modelGetPrimaryBone__FP12_modelHeaderi(void* model, int index) {
    calls() += "bone(" + std::to_string(model == at(0x800)) + "," + std::to_string(index) + ");";
    return index + 10;
}
extern "C" void animGetBoneMatrix__FP14AnimationStateiP8Matrix34(void* anim, int bone, Matrix34* out) {
    calls() += "abm(" + std::to_string(anim == at(0x900)) + "," + std::to_string(bone) + ");";
    for (int i = 0; i < 12; i++) out->m[i] = 1.0f + i;
}
extern "C" void matMakeTransYawScale__FP8Matrix34G6Point3sf(Matrix34* out, Point3 t, short yaw, float scale) {
    calls() += "mk(" + num(t.x) + "," + num(t.y) + "," + num(t.z) + "," + std::to_string(yaw) + "," + num(scale) + ");";
    for (int i = 0; i < 12; i++) out->m[i] = 100.0f + i;
}
extern "C" void matMul__FP8Matrix34N20(Matrix34* out, Matrix34* a, Matrix34* b) {
    calls() += "mul(" + std::to_string(out == a) + "," + num(a->m[0]) + "," + num(b->m[0]) + ");";
    for (int i = 0; i < 12; i++) out->m[i] = a->m[i] + b->m[i];
}
extern "C" void matTransform__FP6Point3P8Matrix34T0(Point3* out, Matrix34* m, Point3* in) {
    calls() += "xf(" + std::to_string(out == in) + "," + num(m->m[0]) + "," + num(in->x) + ");";
    out->x = in->x * 2;
    out->y = in->y * 3;
    out->z = in->z * 4;
}
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) { calls() += std::string("lump(") + lump + "," + name + ");"; return at(0x40); }
extern "C" int w11_sprintf(char* out, const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    const int v = va_arg(ap, int);
    va_end(ap);
    return std::snprintf(out, 64, format, v);
}
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) {
    calls() += "snd(" + std::to_string(h == at(0x40)) + "," + id(o) + "," + num(v) + ");";
    return 0;
}
int main() {
    the_obj = at(0x100);
    int bad = 0;
    struct Case { bool description; int flags; unsigned long long q; signed char flag568; short yaw; };
    const Case cases[] = {{false, 0, 0, 0, 0}, {true, 0x19, 0x8000000000000000ull, 0, 100}, {true, 0x20, 5, 0, 32767}, {false, 0x8, 0x12345, 3, -16384}};
    unsigned char description[72];
    for (int i = 0; i < 72; i++) description[i] = static_cast<unsigned char>(i + 1);
    for (const Case& k : cases) {
        std::memset(arena, 0x5a, sizeof arena);
        calls().clear();
        w11::u32(the_obj, 12) = 0x1;
        w11::u32(the_obj, 68) = 3;
        w11::u8(the_obj, 568) = static_cast<unsigned char>(k.flag568);
        *reinterpret_cast<unsigned long long*>(the_obj + 232) = k.q;
        char a[] = "a", b[] = "b", c[] = "c";
        __6PhPropG6Point3sP12_modelHeaderP14AnimationStatePcN25liPQ24item11descriptionf(the_obj, Point3{1.0f, -2.0f, 3.5f}, k.yaw, at(0x800), at(0x900), a, b, c, 7,
                                                                                         k.flags, k.description ? description : nullptr, 2.5f);
        const bool flag = k.description || k.flag568 != 0;
        const bool snd = flag && (k.flags & 0x19) != 0;
        int top = -1;
        for (unsigned long long q = k.q; q; q >>= 1) top++;
        std::string want = "prop(1,1.000000,-2.000000,3.500000,abc,7," + std::to_string(k.flags | 4) + ");";
        want += "bone(1," + std::to_string(top) + ");abm(1," + std::to_string(top + 10) + ");";
        want += "mk(0.000000,0.000000,0.000000," + std::to_string(static_cast<short>(k.yaw + 16384)) + ",2.500000);mul(1,1.000000,100.000000);";
        want += "xf(1,101.000000,0.500000);init(1," + std::to_string(k.flags) + ")[";
        unsigned w12 = 0x1 | 0x4000;
        if (k.description) w12 = (w12 | 0x100000) & ~0x800u;
        want += std::to_string(w12) + "];";
        unsigned seed = 3 * 0x19660du + 0x3c6ef35fu;
        if (snd) want += "lump(fx.lmp,game_swish_chest" + std::to_string((seed >> 16) % 3 + 1) + ".anm);snd(1,1,1.000000);";
        if (calls() != want) { std::puts(calls().c_str()); std::puts(want.c_str()); bad = 1; }
        // d = (0.5 + 1.0 - 1.0, 0, 0)*... base leaves (1.5,-2,3.5): d = (0.5,0,0) -> (1.0,0,0); result P + d
        if (w11::f32(the_obj, 16) != 2.0f || w11::f32(the_obj, 20) != -2.0f || w11::f32(the_obj, 24) != 3.5f) { std::puts("position"); bad = 1; }
        if (w11::u32(the_obj, 12) != (w12 | 0x2000) || w11::u32(the_obj, 72) != w11::word(vt_PhProp) || w11::f32(the_obj, 576) != 2.5f ||
            w11::i32(the_obj, 556) != top || w11::i32(the_obj, 564) != 0 || w11::u8(the_obj, 570) != 0 || w11::u32(the_obj, 68) != (snd ? seed : 3u) ||
            w11::u8(the_obj, 568) != (k.description ? 1 : k.flag568) ||
            (!k.description && (w11::u8(the_obj, 361) != 0xff || w11::i16(the_obj, 388) != -1 || w11::u16(the_obj, 344) != 0 || w11::u16(the_obj, 358) != 0)) ||
            (k.description && std::memcmp(the_obj + 328, description, 72) != 0)) {
            std::puts("fields");
            bad = 1;
        }
    }
    return bad;
}
