// Host contract for msg_draw__4Soul; not an EE input. Words that hold addresses are offsets from a host base.
// Callees record the call order and arguments, and matrix identities are checked by address.
#define W10_ACTOR_M_HOST 1
#include <cstring>
#include <initializer_list>
#include <string>
unsigned char* w10_actor_m_host_base;
unsigned w10_actor_m_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
#include "msg_draw.cpp"
static std::string order;
static void *minion_arg; static Matrix34 *attach_out, *getm_out, *inv_a, *inv_b, *mul_a, *mul_b, *mul_c, *mk_out, *rot_m, *sh_m, *md_m;
static void *getm_model, *getm_anim, *sh_model, *sh_anim, *md_model, *md_tex, *md_anim, *md_rec, *rec_ret, *draw_a, *draw_m, *draw_t, *draw_x;
static int getm_idx, sh_flag, md_flag, md_zero, rot_axis; static short rot_ang, mk_yaw; static float getm_scale, mk_scale; static char sh_extra;
static Point3 md_pos, mk_pos; static unsigned long long md_bits;
extern "C" void getSoulAttachMatrix__6MinionP8Matrix34(void* m, Matrix34* o) { order += "G;"; minion_arg = m; attach_out = o; }
extern "C" void modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(void* m, void* a, int i, Matrix34* o, float s) { order += "M;"; getm_model = m; getm_anim = a; getm_idx = i; getm_out = o; getm_scale = s; }
extern "C" void matInvert__FP8Matrix34T0(Matrix34* a, Matrix34* b) { order += "I;"; inv_a = a; inv_b = b; }
extern "C" void matMul__FP8Matrix34N20(Matrix34* a, Matrix34* b, Matrix34* c) { order += "X;"; mul_a = a; mul_b = b; mul_c = c; }
extern "C" void* modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(void* m, int f, Matrix34* x, void* a, char e) { order += "S;"; sh_model = m; sh_flag = f; sh_m = x; sh_anim = a; sh_extra = e; return rec_ret; }
extern "C" void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(void* m, void* t, int f, Point3 p, Matrix34* x, void* a, unsigned long long b, void* r, int z) {
    order += "W;"; md_model = m; md_tex = t; md_flag = f; md_pos = p; md_m = x; md_anim = a; md_bits = b; md_rec = r; md_zero = z; }
extern "C" void matMakeTransYawScale__FP8Matrix34G6Point3sf(Matrix34* o, Point3 p, short y, float s) { order += "K;"; mk_out = o; mk_pos = p; mk_yaw = y; mk_scale = s; }
extern "C" void matRot__FP8Matrix34is(Matrix34* m, int axis, short ang) { order += "R;"; rot_m = m; rot_axis = axis; rot_ang = ang; }
extern "C" void* draw__8CreatureP12_modelHeaderP8_textureT1(void* s, void* m, void* t, void* x) { order += "D;"; draw_a = s; draw_m = m; draw_t = t; draw_x = x; return nullptr; }
int main() {
    int bad = 0;
    alignas(16) static unsigned char arena[0x4000];
    w10_actor_m_host_base = arena;
    const unsigned O = 0x100, T = 0x2000, MIN = 0x1000;
    rec_ret = arena + 0x3800;
    for (int mode = 0; mode < 3; ++mode) for (short yaw : {0, 100, -5, 0x7fff, -32768}) for (short st : {17, 0, 5}) {
        std::memset(arena, 0, sizeof arena);
        unsigned minion = mode == 0 ? MIN : 0u; std::memcpy(arena + O + 960, &minion, 4);
        unsigned tb = T; std::memcpy(arena + O + 36, &tb, 4);
        for (unsigned off : {12u, 20u, 28u, 36u}) { unsigned v = 0x3000 + off; std::memcpy(arena + T + off, &v, 4); }
        unsigned mw = 0x3300; std::memcpy(arena + MIN + 12, &mw, 4);
        float x = 1.5f, y = -2.0f, z = 7.0f, w = 3.0f, sc = 2.5f; std::memcpy(arena + O + 16, &x, 4); std::memcpy(arena + O + 20, &y, 4);
        std::memcpy(arena + O + 24, &z, 4); std::memcpy(arena + O + 32, &w, 4); std::memcpy(arena + O + 224, &sc, 4);
        std::memcpy(arena + O + 56, &yaw, 2); std::memcpy(arena + O + 572, &st, 2);
        unsigned long long bits = 0x1122334455667788ull; std::memcpy(arena + O + 552, &bits, 8);
        unsigned char ref[sizeof arena]; std::memcpy(ref, arena, sizeof arena);
        order.clear();
        msg_draw__4Soul(arena + O);
        unsigned char* anim = arena + O + 288;
        auto model = [&](unsigned off) { return arena + 0x3000 + off; };
        if (mode == 0) {
            if (order != "G;M;I;X;S;W;") bad = 1;
            if (minion_arg != arena + mw || getm_model != model(28) || getm_anim != anim || getm_idx != 0 || getm_scale != 1.0f) bad = 1;
            if (inv_a != getm_out || inv_b != getm_out || mul_a != attach_out || mul_b != getm_out || mul_c != attach_out || attach_out == getm_out) bad = 1;
            if (sh_model != model(28) || sh_flag != 1 || sh_m != attach_out || sh_anim != anim || sh_extra != 0) bad = 1;
            if (md_model != model(28) || md_tex != model(36) || md_flag != 1 || md_m != attach_out || md_anim != anim || md_bits != bits || md_rec != rec_ret || md_zero != 0) bad = 1;
            if (md_pos.x != x || md_pos.y != y || md_pos.z != z + w * 0.5f) bad = 1;
        } else if (st == 17) {
            if (order != "K;R;S;W;") bad = 1;
            if (mk_pos.x != x || mk_pos.y != y || mk_pos.z != z || mk_yaw != static_cast<short>(static_cast<unsigned short>(yaw) + 24384u) || mk_scale != sc) bad = 1;
            if (rot_m != mk_out || rot_axis != 0 || rot_ang != 20384 || sh_m != mk_out || sh_model != model(28) || sh_flag != 1 || sh_anim != anim) bad = 1;
            if (md_m != mk_out || md_model != model(28) || md_tex != model(36) || md_flag != 1 || md_anim != anim || md_bits != bits || md_rec != rec_ret || md_zero != 0) bad = 1;
            if (md_pos.x != x || md_pos.y != y || md_pos.z != z + w * 0.5f) bad = 1;
        } else {
            if (order != "D;" || draw_a != arena + O || draw_m != model(12) || draw_t != model(20) || draw_x != nullptr) bad = 1;
        }
        if (std::memcmp(arena, ref, sizeof arena)) bad = 1;
    }
    return bad;
}
