// Host contract for terminate__19FireArrowProjectilei; not an EE input. The callees log. The authored function was also compared
// with the original in a private R5900 run (16 scenarios over the done word, allocation success and the argument; every original
// word covered).
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstring>
#include <string>
unsigned char* w13_spell_msg_b_host_base;
unsigned w13_spell_msg_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::string calls;
static bool alloc_ok;
static Point3 sf_where;
static float sf_a, sf_c;
static int sf_b;
static std::string lump_a, lump_b;
static float sound_volume;
static void* sound_object;
extern "C" {
static unsigned char heap[1192];
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return alloc_ok ? heap : nullptr; }
void StaticFire_construct(void*, Point3 where, float a, int b, float c) __asm__("__10StaticFireG6Point3fif");
void StaticFire_construct(void*, Point3 where, float a, int b, float c) { calls += "sf;"; sf_where = where; sf_a = a; sf_b = b; sf_c = c; }
void* lumpFindResource__FPcT0(const char* a, const char* b) { calls += "lump;"; lump_a = a; lump_b = b; return heap + 8; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) { calls += "sound;"; sound_object = o; sound_volume = v; (void)h; }
void objectAddToDeleteList__FP10GameObject(void*) { calls += "del;"; }
}
#include "terminate.cpp"

static unsigned char object[256] __attribute__((aligned(16)));
static void reset(unsigned done) {
    std::memset(object, 0, sizeof object);
    calls.clear(); alloc_ok = true;
    std::memcpy(object + 96, &done, 4);
    const float p[3] = {1.0f, 2.0f, 3.0f};
    std::memcpy(object + 16, p, 12);
}
int main() {
    reset(5);
    terminate__19FireArrowProjectilei(object, 1);
    if (!calls.empty() || object[96] != 5) return 1;
    reset(0);
    terminate__19FireArrowProjectilei(object, 1);
    if (calls != "alloc1192;sf;lump;sound;del;" || object[96] != 1) return 2;
    if (sf_where.x != 1.0f || sf_where.y != 2.0f || sf_where.z != 3.0f || sf_a != 300.0f || sf_b != 1020 || sf_c != 0.5f) return 3;
    if (lump_a != "fx.lmp" || lump_b != "spel_we_firearrow_hit.anm" || sound_volume != 1.0f || sound_object != object) return 4;
    reset(0); alloc_ok = false;
    terminate__19FireArrowProjectilei(object, 1);
    if (calls != "alloc1192;lump;sound;del;") return 5;
    reset(0);
    terminate__19FireArrowProjectilei(object, 2);
    if (calls != "lump;sound;del;" || object[96] != 1) return 6;
    reset(0);
    terminate__19FireArrowProjectilei(object, 0);
    if (calls != "del;" || object[96] != 1) return 7;
    return 0;
}
