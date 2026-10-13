// Host contract for terminate__21PoisonArrowProjectilei; not an EE input. 32-bit words are offsets from a host base and the
// callees log. The authored function was also compared with the original in a private R5900 run (72 scenarios over the done word,
// owner presence, allocation, level number and slot results; every original word covered).
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstring>
#include <string>
unsigned char* w13_spell_msg_b_host_base;
static std::string calls, lump_b;
static int slot_result[4];
unsigned w13_spell_msg_b_host_call(unsigned fn, void*, int, unsigned long a1, unsigned long a2) {
    if (fn == 0x710) { calls += "s88(" + std::to_string(a1) + "," + std::to_string(a2) + ");"; return slot_result[a1]; }
    return 0;
}
static int particles;
static float first_vx, first_vy, last_vx;
static bool alloc_ok;
static Point3 ctor_where;
static int ctor_a, ctor_b, ctor_c, ctor_d;
static void* ctor_owner;
extern "C" {
unsigned char poisonPoof[8];
float icos__Fi(int a) { return static_cast<float>(a); }
float isin__Fi(int a) { return static_cast<float>(a) * 2.0f; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float age) {
    if (d != poisonPoof || age != 0.0f || p->x != 1.0f || v->z != 0.0f) calls += "bad;";
    if (particles++ == 0) { first_vx = v->x; first_vy = v->y; }
    last_vx = v->x;
}
static unsigned char heap[112];
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return alloc_ok ? heap : nullptr; }
void poisonGasSpell_construct(void*, Point3 where, int a, int b, int c, void* owner, int d) __asm__("__14poisonGasSpellG6Point3iiiP10GameObjecti");
void poisonGasSpell_construct(void*, Point3 where, int a, int b, int c, void* owner, int d) {
    calls += "ctor;"; ctor_where = where; ctor_a = a; ctor_b = b; ctor_c = c; ctor_owner = owner; ctor_d = d;
}
void* lumpFindResource__FPcT0(const char* a, const char* b) { calls += "lump;"; lump_b = b; return a ? nullptr : nullptr; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void*, float v) { calls += v == 1.0f ? "sound;" : "badvolume;"; }
void objectAddToDeleteList__FP10GameObject(void*) { calls += "del;"; }
}
#include "terminate.cpp"

static unsigned char arena[0x2000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
enum { O = 0x100, Q = 0x500, W = 0x600, S = 0xe00, TS = 0x1000 };
static void reset(unsigned done, bool owner) {
    w13_spell_msg_b_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    calls.clear(); particles = 0; alloc_ok = true; slot_result[2] = 5; slot_result[3] = 7;
    put32(O + 96, done); put32(O + 76, owner ? Q : 0); put32(Q + 12, W); put32(O + 172, S); put32(S + 96, TS); put32(TS + 88 + 4, 0x710);
    put32(O + 104, 4);
    const float p[3] = {1.0f, 2.0f, 3.0f};
    std::memcpy(arena + O + 16, p, 12);
}
int main() {
    unsigned char* const o = arena + O;
    reset(9, true);
    terminate__21PoisonArrowProjectilei(o, 1);
    if (!calls.empty() || particles != 0) return 1;
    reset(0, false);
    terminate__21PoisonArrowProjectilei(o, 1);
    if (calls != "lump;sound;del;" || arena[O + 96] != 1 || lump_b != "spel_we_poisarrow_hit.anm") return 2;
    if (particles != 213 || first_vx != 0.0f || first_vy != 0.0f || last_vx != 9.0f * 65508.0f) return 3;
    reset(0, true);
    terminate__21PoisonArrowProjectilei(o, 1);
    if (calls != "alloc112;s88(2,4);s88(3,4);ctor;lump;sound;del;") { return 4; }
    if (ctor_where.x != 1.0f || ctor_where.y != 2.0f || ctor_where.z != 3.0f || ctor_a != 7 * 60 || ctor_b != 5 * 12 || ctor_c != 0 ||
        ctor_owner != arena + W || ctor_d != 5) return 5;
    reset(0, true); alloc_ok = false;
    terminate__21PoisonArrowProjectilei(o, 1);
    if (calls != "alloc112;lump;sound;del;") return 6;
    return 0;
}
