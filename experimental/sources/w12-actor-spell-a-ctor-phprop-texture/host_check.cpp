// Host contract for the PhProp (model and texture) constructor; not an EE input. Stubs log the call order with
// their arguments; the expected log and stored fields are written out independently.
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
extern "C" void prop_construct_texture(void* self, Point3 p, void* model, void* texture, long n, int flags) {
    calls() += "prop(" + id(self) + "," + num(p.x) + "," + num(p.y) + "," + num(p.z) + "," + std::to_string(model == at(0x800)) + std::to_string(texture == at(0x900)) + "," +
               std::to_string(n) + "," + std::to_string(flags) + ");";
}
extern "C" void initStuff__6PhPropi(void* self, int flags) { calls() += "init(" + id(self) + "," + std::to_string(flags) + ")[" + std::to_string(w11::u32(self, 12)) + "];"; }
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
    struct Case { bool description; int flags; signed char flag568; unsigned seed; };
    const Case cases[] = {{false, 0, 0, 3}, {true, 0x19, 0, 3}, {true, 0x20, 0, 3}, {false, 0x8, 3, 3}, {true, 0x1, 0, 0x12345678}, {false, 0x10, 0, 3}, {false, 0x2, 1, 3}};
    unsigned char description[72];
    for (int i = 0; i < 72; i++) description[i] = static_cast<unsigned char>(i + 1);
    for (const Case& k : cases) {
        std::memset(arena, 0x5a, sizeof arena);
        calls().clear();
        w11::u32(the_obj, 12) = 0x1;
        w11::u32(the_obj, 68) = k.seed;
        w11::u8(the_obj, 568) = static_cast<unsigned char>(k.flag568);
        w11::i32(the_obj, 556) = 0x1234;
        __6PhPropG6Point3P12_modelHeaderP8_textureliPQ24item11descriptionf(the_obj, Point3{1.0f, -2.0f, 3.5f}, at(0x800), at(0x900), 7, k.flags,
                                                                          k.description ? description : nullptr, 2.5f);
        const bool flag = k.description || k.flag568 != 0;
        const bool snd = flag && (k.flags & 0x19) != 0;
        std::string want = "prop(1,1.000000,-2.000000,3.500000,11,7," + std::to_string(k.flags) + ");init(1," + std::to_string(k.flags) + ")[";
        unsigned w12 = 0x1 | 0x4000;
        if (k.description) w12 = (w12 | 0x100000) & ~0x800u;
        want += std::to_string(w12) + "];";
        const unsigned seed = k.seed * 0x19660du + 0x3c6ef35fu;
        if (snd) want += "lump(fx.lmp,game_swish_chest" + std::to_string((seed >> 16) % 3 + 1) + ".anm);snd(1,1,1.000000);";
        if (calls() != want) { std::puts(calls().c_str()); std::puts(want.c_str()); bad = 1; }
        if (w11::u32(the_obj, 12) != w12 || w11::u32(the_obj, 72) != w11::word(vt_PhProp) || w11::f32(the_obj, 576) != 2.5f ||
            w11::i32(the_obj, 556) != 0x1234 || w11::i32(the_obj, 564) != 0 || w11::u8(the_obj, 570) != 0 || w11::u32(the_obj, 68) != (snd ? seed : k.seed) ||
            w11::u8(the_obj, 568) != (k.description ? 1 : k.flag568) ||
            (!k.description && (w11::u8(the_obj, 361) != 0xff || w11::u8(the_obj, 360) != 0xff || w11::i16(the_obj, 388) != -1 || w11::i16(the_obj, 364) != -1 ||
                                w11::u16(the_obj, 344) != 0 || w11::u16(the_obj, 358) != 0)) ||
            (k.description && std::memcmp(the_obj + 328, description, 72) != 0)) {
            std::puts("fields");
            bad = 1;
        }
    }
    if (!bad) std::puts("phprop texture ctor ok");
    return bad;
}
