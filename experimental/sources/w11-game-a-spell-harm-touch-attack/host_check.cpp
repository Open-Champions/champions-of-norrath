// Host contract for SpellHarmTouch::attack; not an EE input.
#define NORRATH_HOST_CONTRACT 1
extern unsigned char arena[];
#define NORRATH_HOST_BASE reinterpret_cast<__UINTPTR_TYPE__>(arena)
#include "spell_harm_touch_attack.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
alignas(16) unsigned char arena[0x2000];
unsigned char harm1[16], harm2[16];
static unsigned char* self = arena + 0x000;
static unsigned char* caster = arena + 0x100;
static unsigned char* record = arena + 0x200;
static unsigned char* target = arena + 0x400;
static unsigned char* vt_self = arena + 0x600;
static unsigned char* vt_caster = arena + 0x700;
static unsigned char* vt_target = arena + 0x800;
static int erands, particles1, particles2, lights, notifies, shadow_calls, models;
static float last_dir1[3], last_dir2[3], last_out[3];
static int last_notify_arg;
static unsigned char* last_notify_object;
static float light_args[12];
static int alloc_result_null;
static float light_pos[3];
int eRand__5Spell(void*) { ++erands; return 65536 * 3 + 3; }
int modelGetNmShadowPoints__FP12_modelHeader(void*) { return 4; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void*, void*, const float*, short, int index, float* out, float*, void* matrix, float scale) {
    ++shadow_calls;
    out[0] = 30.0f + index; out[1] = 40.0f; out[2] = 5.0f;
    if (matrix || scale != 1.0f) out[0] = -1e9f;
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, float* p, float* v, float age) {
    if (age != 0.0f) return;
    if (d == harm1) { ++particles1; std::memcpy(last_dir1, v, 12); std::memcpy(last_out, p, 12); }
    else if (d == harm2) { ++particles2; std::memcpy(last_dir2, v, 12); }
}
void* blockAlloc__Fi(int size) { return size == 188 && !alloc_result_null ? arena + 0x1000 : nullptr; }
void __11LightEffectG6Point3ffffffffffffP10GameObjecti(void* block, const float* point, float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10, float p11, float p12, float p13, float p14, void* object, int flag) {
    ++lights;
    std::memcpy(light_pos, point, 12);
    float v[12] = {p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14};
    std::memcpy(light_args, v, sizeof v);
    if (block != arena + 0x1000 || object || flag) light_args[0] = -1.0f;
}
unsigned char* model_fn(unsigned char* adj) { ++models; return adj == target + 24 ? arena + 0x1800 : arena + 0x1900; }
int damage_fn(unsigned char* adj, int a, int b, unsigned char* c) { return (adj == caster + 8 && a == 0 && b == 5 && c == record) ? 100 : -777; }
void notify_fn(unsigned char* adj, int arg) { ++notifies; last_notify_object = adj; last_notify_arg = arg; }
void* host_function_table[4] = {nullptr, reinterpret_cast<void*>(model_fn), reinterpret_cast<void*>(damage_fn), reinterpret_cast<void*>(notify_fn)};
static void put32(unsigned char* base, int off, unsigned v) { std::memcpy(base + off, &v, 4); }
static void putf(unsigned char* base, int off, float v) { std::memcpy(base + off, &v, 4); }
static unsigned off(unsigned char* p) { return static_cast<unsigned>(p - arena); }
int main() {
    int bad = 0;
    int damage = -1, total = -1;
    *self = 5;
    put32(self, 8, off(caster)); put32(self, 12, off(record)); put32(self, 20, off(vt_self)); putf(self, 24, 0.25f);
    put32(vt_self, 8, 0x10 | 0); put32(vt_self, 12, 3);
    put32(caster, 96, off(vt_caster)); put32(vt_caster, 104, 8); put32(vt_caster, 108, 2);
    putf(record, 16, 10.0f); putf(record, 20, 20.0f);
    put32(target, 72, off(vt_target)); put32(vt_target, 128, 24); put32(vt_target, 132, 1);
    putf(target, 16, 1.0f); putf(target, 20, 2.0f); putf(target, 24, 3.0f);
    // no 0x20 flag: shorts only
    put32(target, 12, 0x1f);
    attack__14SpellHarmTouchP10GameObjectPiT2(self, target, &damage, &total);
    short s142, s146, s150; std::memcpy(&s142, record + 142, 2); std::memcpy(&s146, record + 146, 2); std::memcpy(&s150, record + 150, 2);
    if (s142 != 32 || s146 != 113 || s150 != 32 || erands != 0 || damage != -1 || total != -1) { std::puts("fail: no flag"); bad = 1; }
    // flag set: 102 rounds
    putf(self, 24, 7.0f);
    put32(target, 12, 0x20);
    attack__14SpellHarmTouchP10GameObjectPiT2(self, target, &damage, &total);
    std::memcpy(&s142, record + 142, 2);
    if (s142 != 128) { std::puts("fail: cap"); bad = 1; }
    if (shadow_calls != 102 || particles1 != 102 || particles2 != 102 || models != 204) { std::printf("fail: rounds %d %d %d %d\n", shadow_calls, particles1, particles2, models); bad = 1; }
    // eRand: 102 * 2 in the loop, plus the damage roll
    if (erands != 205) { std::printf("fail: erand count %d\n", erands); bad = 1; }
    float dx = last_out[0] - 10.0f, dy = 40.0f - 20.0f, len = std::sqrt(dx * dx + dy * dy);
    float want = dx / len * 53.435116f;
    if (std::fabs(last_dir1[0] - want) > 1e-3f || last_dir1[2] != 0.0f || std::fabs(last_dir2[0] - last_dir1[0] * 0.137f) > 1e-3f) { std::puts("fail: direction"); bad = 1; }
    if (lights != 1 || light_pos[0] != 1.0f || light_pos[1] != 2.0f || light_pos[2] != 76.0f || light_args[0] != 83.0f || light_args[1] != 218.0f || light_args[2] != 73.0f
        || light_args[7] != 0.0f || light_args[8] != 216.0f || light_args[9] != 60.0f || light_args[10] != 0.5f) { std::puts("fail: light"); bad = 1; }
    // roll = 3 -> scaled = 100 * 0.2 * (3*65536+3)/65536 = 60.0..; total = 100 + 60
    if (damage != 100 || total != 160 || notifies != 1 || last_notify_object != self + 16 || last_notify_arg != 3) { std::printf("fail: damage %d %d\n", damage, total); bad = 1; }
    alloc_result_null = 1;
    attack__14SpellHarmTouchP10GameObjectPiT2(self, target, &damage, &total);
    if (lights != 1 || notifies != 2) { std::puts("fail: null block"); bad = 1; }
    if (!bad) std::puts("SpellHarmTouch attack ok");
    return bad;
}
