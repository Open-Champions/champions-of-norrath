// Host contract for msg_draw__11DiseaseBolt; not an EE input. 32-bit words are offsets from a host arena; every callee logs its name and arguments.
// The authored function was also compared with the original in a private R5900 run (40 scenarios, every original word covered;
// mutants of the yaw offset and scale, the ambient colour, the resource names and their order and the draw arguments are caught there).
#define W12_SPELL_C_HOST 1
#include "../w12-actor-spell-c-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w12_spell_c_host_base;
unsigned w12_spell_c_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static unsigned char arena[0x200];
static std::string calls;
static Point3 got_first, got_second;
static short got_yaw, got_rot_angle; static int got_rot_axis; static float got_scale;
static short got_color[4];
static std::string lump_names[4];
static int lump_n;
static void* got_model; static void* got_texture; static int got_mode; static void* got_matrix; static void* got_animation; static unsigned long got_flags; static void* got_record; static int got_last;
static void* matrix_seen;
#include "msg_draw.cpp"
extern "C" {
void matMakeTransYawScale__FP8Matrix34G6Point3sf(Matrix34* m, Point3 p, short yaw, float scale) { calls += "make;"; matrix_seen = m; got_first = p; got_yaw = yaw; got_scale = scale; }
void lightSetAmbientColor__FPs(short* color) { calls += "ambient;"; std::memcpy(got_color, color, 8); }
void lightResetOptionalParams__Fv() { calls += "reset;"; }
void* lumpFindResource__FPcT0(char* lump, char* name) { calls += "lump;"; lump_names[lump_n++] = std::string(lump) + "/" + name; return arena + 0x100 + 0x10 * lump_n; }
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 p, Matrix34* m, void* animation, unsigned long flags, void* record, int last) {
    calls += "draw;"; got_model = model; got_texture = texture; got_mode = mode; got_second = p; got_matrix = m; got_animation = animation; got_flags = flags; got_record = record; got_last = last;
    if (m != matrix_seen) calls += "BADM;";
    return 0;
}
}
int main() {
    using namespace w12c;
    w12_spell_c_host_base = arena;
    int r = 0;
    for (int variant = 0; variant < 4; ++variant) {
        std::memset(arena, 0, sizeof arena);
        calls.clear(); lump_n = 0;
        unsigned char* o = arena + 0x40;
        real(o, 16) = 1.5f + variant; real(o, 20) = -2.0f; real(o, 24) = 3.25f;
        const short angle = static_cast<short>(variant == 0 ? 0 : variant == 1 ? 0x7fff : variant == 2 ? -32768 : 0x1234);
        half(o, 56) = angle;
        half(o, 152) = static_cast<short>(variant * 1111 - 700);
        msg_draw__11DiseaseBolt(o);
        if (calls != "make;ambient;lump;lump;draw;reset;") { std::puts(calls.c_str()); r = 1; break; }
        if (got_first.x != 1.5f + variant || got_first.y != -2.0f || got_first.z != 3.25f || got_second.x != got_first.x || got_second.z != 3.25f) { r = 2; break; }
        if (got_yaw != static_cast<short>(angle + 16612) || got_scale != 1.0f) { r = 3; break; }
        if (got_color[0] != 7 || got_color[1] != 17 || got_color[2] != 7 || got_color[3] != 0) { r = 5; break; }
        if (lump_names[0] != "fx.lmp/skull.vif" || lump_names[1] != "fx.lmp/skull.tex") { r = 6; break; }
        if (got_model != arena + 0x110 || got_texture != arena + 0x120 || got_mode != 3 || got_animation != nullptr || got_flags != static_cast<unsigned long>(-1) || got_record != nullptr || got_last != 0) { r = 7; break; }
    }
    return r;
}
