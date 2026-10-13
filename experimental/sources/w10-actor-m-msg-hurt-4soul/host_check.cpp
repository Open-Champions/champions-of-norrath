// Host contract for msg_hurt__4SoulfP10GameObjecti; not an EE input. Address-holding words are offsets from a
// host base. Callees are recorded stubs; eRand returns scripted rolls.
#define W10_ACTOR_M_HOST 1
#include <cmath>
#include <cstring>
#include <initializer_list>
#include <string>
#include <vector>
#include "../w10-actor-m-common/access.hpp"
unsigned char* w10_actor_m_host_base;
static const unsigned TYPE_FN = 0xaaaa0001u, MODEL_FN = 0xaaaa0002u;
static std::string order; static void* type_arg; static void* model_adj; static int ntype, nmodel;
unsigned w10_actor_m_host_call(unsigned fn, void* a0, int, unsigned long, unsigned long) {
    if (fn == TYPE_FN) { ++ntype; type_arg = a0; order += "T;"; return 0x1234; }
    if (fn == MODEL_FN) { ++nmodel; model_adj = a0; order += "m;"; return 0x3300; }
    order += "BADFN;"; return 0;
}
unsigned char soulParticleStorage[8];
extern "C" unsigned char soulParticle[8];
unsigned char soulParticle[8];
static int eq_ret, shadow_count; static const void *eq_a, *eq_b; static unsigned char demon_ti[4];
static std::vector<int> rolls; static size_t roll_i;
struct Shadow { void* model; void* anim; Point3 pos; short angle; int index; Point3* out; float* outv; void* matrix; float scale; };
struct Part { void* def; Point3 pos, vel; float v; };
static std::vector<Shadow> shadows; static std::vector<Part> parts; static int nbase, base_flag; static float base_amt; static void *base_self, *base_src; static int ncount_calls; static void* count_model;
static int thrown;
extern "C" __attribute__((noreturn)) void __throw_bad_typeid() { throw 7; }
extern "C" void* __tf5Demon() { order += "D;"; return demon_ti; }
extern "C" int __eq__C9type_infoRC9type_info(const void* a, const void* b) { order += "E;"; eq_a = a; eq_b = b; return eq_ret; }
extern "C" void msg_hurt__8CreaturefP10GameObjecti(void* s, float a, void* src, int f) { order += "H;"; ++nbase; base_self = s; base_amt = a; base_src = src; base_flag = f; }
extern "C" int modelGetNmShadowPoints__FP12_modelHeader(void* m) { order += "N;"; ++ncount_calls; count_model = m; return shadow_count; }
extern "C" void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* anim, Point3 pos, short angle, int index, Point3* out, float* outv, void* matrix, float scale) {
    order += "S;"; shadows.push_back(Shadow{model, anim, pos, angle, index, out, outv, matrix, scale});
    out->x = 100.0f + shadows.size(); out->y = 200.0f; out->z = 300.0f; *outv = 5.0f;
}
extern "C" void P_AddParticle__FP11ParticleDefP6Point3T1f(void* def, Point3* pos, Point3* vel, float v) { order += "P;"; parts.push_back(Part{def, *pos, *vel, v}); }
extern "C" int eRand__10GameObject(void*) { order += "R;"; return rolls[roll_i++ % rolls.size()]; }
#include "msg_hurt.cpp"
int main() {
    int bad = 0;
    alignas(16) static unsigned char arena[0x4000];
    w10_actor_m_host_base = arena;
    const unsigned O = 0x100, S = 0x1000, VT = 0x1800, SVT = 0x1c00;
    for (int srcMode : {1, 0}) for (int eq : {1, 0, 5, -1}) for (int count : {0, 1, 3, 7}) for (short yaw : {0, 100, -5, 0x4000, -32768}) {
        std::memset(arena, 0, sizeof arena);
        unsigned vt = VT, svt = SVT; std::memcpy(arena + O + 72, &vt, 4); std::memcpy(arena + S + 72, &svt, 4);
        short adj = 8; unsigned fn = MODEL_FN; std::memcpy(arena + VT + 128, &adj, 2); std::memcpy(arena + VT + 132, &fn, 4);
        short adj2 = 4; unsigned fn2 = TYPE_FN; std::memcpy(arena + SVT, &adj2, 2); std::memcpy(arena + SVT + 4, &fn2, 4);
        Point3 pos{1.5f, -2.0f, 7.0f}; std::memcpy(arena + O + 16, &pos, 12);
        std::memcpy(arena + O + 56, &yaw, 2); float sc = 3.25f; std::memcpy(arena + O + 224, &sc, 4);
        // rolls: one index roll, then three components; the first triple is rejected (all large), the second accepted.
        rolls.clear();
        for (int shot = 0; shot < 6; ++shot) { rolls.push_back(10 + shot); rolls.insert(rolls.end(), {65000, 65000, 65000, 40000 + shot * 1000, 20000, 33000}); }
        roll_i = 0; order.clear(); shadows.clear(); parts.clear(); nbase = ntype = nmodel = ncount_calls = 0; eq_ret = eq; shadow_count = count; thrown = 0;
        unsigned char ref[sizeof arena]; std::memcpy(ref, arena, sizeof arena);
        try { msg_hurt__4SoulfP10GameObjecti(arena + O, 2.5f, srcMode ? arena + S : nullptr, -7); } catch (int) { thrown = 1; }
        if (!srcMode) { if (!thrown || order != "") bad = 1; continue; }
        if (ntype != 1 || type_arg != arena + O) bad = 1;
        if (eq_a != reinterpret_cast<const void*>(static_cast<__UINTPTR_TYPE__>(0x1234)) || eq_b != demon_ti) bad = 1;
        if (eq == 0) { if (nbase || order != "T;D;E;") bad = 1; continue; }
        if (nbase != 1 || base_self != arena + O || base_src != arena + S || base_flag != -7 || base_amt != 2.5f) bad = 1;
        if (count == 0) { if (!shadows.empty() || !parts.empty()) bad = 1; continue; }
        if (shadows.size() != 6 || parts.size() != 6) { bad = 1; continue; }
        if (count_model != arena + 0x3300) bad = 1;
        for (int i = 0; i < 6; ++i) {
            const Shadow& sh = shadows[i];
            if (sh.model != arena + 0x3300 || sh.anim != arena + O + 288 || std::memcmp(&sh.pos, &pos, 12) || sh.angle != static_cast<short>(static_cast<unsigned short>(yaw) - 16384) ||
                sh.index != (10 + i) % count || sh.matrix != nullptr || sh.scale != sc) bad = 1;
            const Part& p = parts[i];
            const float c0 = 2.0f * (static_cast<float>(40000 + i * 1000) / 65536.0f) - 1.0f, c1 = 2.0f * (20000.0f / 65536.0f) - 1.0f;
            if (p.def != soulParticle || p.pos.x != 100.0f + (i + 1) || p.pos.y != 200.0f || p.pos.z != 300.0f || p.v != 0.0f) bad = 1;
            if (std::fabs(p.vel.x - c0 * 15.0f) > 1e-3f || std::fabs(p.vel.y - c1 * 15.0f) > 1e-3f || p.vel.z != 0.0f) bad = 1;
        }
        if (std::memcmp(arena, ref, sizeof arena)) bad = 1;
    }
    return bad;
}
