// Host contract for msg_draw__8Skelight; not an EE input. Words that hold addresses are offsets from a host
// base; callees record their arguments in order.
#define W10_ACTOR_M_HOST 1
#include <cstring>
#include <initializer_list>
#include <string>
#include "../w10-actor-m-common/access.hpp"
unsigned char* w10_actor_m_host_base;
unsigned w10_actor_m_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::string order; static int nlump;
static void *fx_a, *fx_b, *fx_c, *fx_d, *fx_e, *fx_mat; static Point3 fx_p; static long fx_ang, fx_flag, fx_zero; static float fx_f1, fx_f2;
static void *dr_a, *dr_b, *dr_c, *dr_d, *at_a, *at_b, *at_d, *at_rec; static long at_x, at_y, at_z, at_w;
static const char* lumps[4][2]; static void* lump_ret[4]; static void* ret_record;
extern "C" char* lumpFindResource__FPcT0(const char* l, const char* n) { order += "L;"; lumps[nlump][0] = l; lumps[nlump][1] = n; return static_cast<char*>(lump_ret[nlump++]); }
extern "C" void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* d, void* m, void* s, Point3 p, short ang, void* m2, void* an, float a, int flag, int zero, float b, void* mat) {
    order += "F;"; fx_a = d; fx_b = m; fx_c = s; fx_p = p; fx_ang = ang; fx_d = m2; fx_e = an; fx_f1 = a; fx_f2 = b; fx_flag = flag; fx_zero = zero; fx_mat = mat;
}
extern "C" void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* m, void* t, void* x) { order += "D;"; dr_a = self; dr_b = m; dr_c = t; dr_d = x; return ret_record; }
extern "C" void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* m, void* t, int a, void* m2, void* rec, unsigned long b, int c, int d) {
    order += "A;"; at_a = m; at_b = t; at_x = a; at_d = m2; at_rec = rec; at_y = static_cast<long>(b); at_z = c; at_w = d;
}
#include "msg_draw.cpp"
int main() {
    int bad = 0;
    alignas(16) static unsigned char arena[0x4000];
    w10_actor_m_host_base = arena;
    const unsigned O = 0x100, T = 0x2000;
    ret_record = arena + 0x3000;
    for (unsigned w : {0u, 1u, 0x1a60000u}) for (short st : {6, 5, 0}) {
        std::memset(arena, 0, sizeof arena);
        std::memcpy(arena + O + 36, &T, 4);
        unsigned m = 0x3100, tx = 0x3200; std::memcpy(arena + T + 12, &m, 4); std::memcpy(arena + T + 20, &tx, 4);
        std::memcpy(arena + O + 960, &w, 4);
        Point3 pos{1.5f, -2.0f, 7.0f}; std::memcpy(arena + O + 16, &pos, 12);
        short ang = 301; std::memcpy(arena + O + 56, &ang, 2); std::memcpy(arena + O + 572, &st, 2);
        unsigned char ref[sizeof arena]; std::memcpy(ref, arena, sizeof arena);
        lump_ret[0] = arena + 0x3400; lump_ret[1] = arena + 0x3500; lump_ret[2] = arena + 0x3600; lump_ret[3] = arena + 0x3700;
        order.clear(); nlump = 0;
        msg_draw__8Skelight(arena + O);
        if (w == 0) {
            if (order != "D;") bad = 1;
        } else {
            if (order != "L;F;D;L;L;A;") bad = 1;
            if (fx_a != arena + O + 1032 || fx_b != lump_ret[0] || fx_c != arena + O + 968 || std::memcmp(&fx_p, &pos, 12) || fx_ang != ang || fx_d != arena + m ||
                fx_e != arena + O + 288 || fx_f1 != 0.5f || fx_f2 != 1.0f || fx_flag != (st == 6) || fx_zero != 0 || fx_mat != nullptr) bad = 1;
            for (int i = 0; i < 3; ++i) if (std::strcmp(lumps[i][0], "equipmnt.lmp")) bad = 1;
            if (std::strcmp(lumps[0][1], "long_sword1.vif") || std::strcmp(lumps[1][1], "long_sword1.vif") || std::strcmp(lumps[2][1], "long_sword1.tex")) bad = 1;
            if (at_a != lump_ret[1] || at_b != lump_ret[2] || at_x != 0 || at_d != arena + m || at_rec != ret_record || at_y != -1 || at_z != -1 || at_w != 0) bad = 1;
        }
        if (dr_a != arena + O || dr_b != arena + m || dr_c != arena + tx || dr_d != nullptr) bad = 1;
        if (std::memcmp(arena, ref, sizeof arena)) bad = 1;
    }
    return bad;
}
