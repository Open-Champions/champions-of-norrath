// Host contract for terminate__22ExplodeArrowProjectilei; not an EE input. The callees log. The authored function was also compared
// with the original in a private R5900 run (360 scenarios over the done word, level flags, argument, level number, roll and
// allocation; every original word covered).
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_spell_msg_b_host_base;
unsigned w13_spell_msg_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::string calls, lump_b1, lump_b2;
static int roll, lumps;
static bool alloc_ok;
static float expl_scale, shake, le_f[12];
static Point3 expl_at, le_at;
static int shake_flag, le_n;
static void* le_object = reinterpret_cast<void*>(1);
extern "C" {
int levelFlags;
int libc_sprintf(char* b, const char* f, ...) __asm__("sprintf");
int libc_sprintf(char* b, const char* f, ...) { va_list ap; va_start(ap, f); const int n = std::vsnprintf(b, 200, f, ap); va_end(ap); calls += "sprintf;"; return n; }
int eRand__10GameObject(void*) { calls += "rand;"; return roll; }
void addEffect__FiG6Point3(int effect, Point3) { calls += "effect" + std::to_string(effect) + ";"; }
void* lumpFindResource__FPcT0(const char*, const char* b) { calls += "lump;"; (lumps++ == 0 ? lump_b1 : lump_b2) = b; return nullptr; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void*, float v) { calls += v == 1.0f ? "sound;" : "badvolume;"; }
void makeExplosion__FG6Point3f(Point3 p, float s) { calls += "explode;"; expl_at = p; expl_scale = s; }
static unsigned char heap[188];
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return alloc_ok ? heap : nullptr; }
void LightEffect_construct(void*, Point3 p, float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, void* object,
                           int n) __asm__("__11LightEffectG6Point3ffffffffffffP10GameObjecti");
void LightEffect_construct(void*, Point3 p, float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, void* object,
                           int n) {
    calls += "light;"; le_at = p; le_object = object; le_n = n;
    const float v[12] = {a, b, c, d, e, f, g, h, i, j, k, l};
    std::memcpy(le_f, v, sizeof v);
}
void gameSetCameraShake__Ffb(float amount, int skip) { calls += "shake;"; shake = amount; shake_flag = skip; }
void objectAddToDeleteList__FP10GameObject(void*) { calls += "del;"; }
}
#include "terminate.cpp"

static unsigned char object[256] __attribute__((aligned(16)));
static void reset(unsigned done, int n) {
    std::memset(object, 0, sizeof object);
    calls.clear(); roll = 0; levelFlags = 0; lumps = 0; alloc_ok = true;
    std::memcpy(object + 96, &done, 4);
    std::memcpy(object + 104, &n, 4);
    const float p[3] = {1.0f, 2.0f, 3.0f};
    std::memcpy(object + 16, p, 12);
}
int main() {
    reset(1, 4);
    terminate__22ExplodeArrowProjectilei(object, 1);
    if (!calls.empty()) return 1;
    reset(0, 4);
    terminate__22ExplodeArrowProjectilei(object, 0);
    if (calls != "del;" || object[96] != 1) return 2;
    reset(0, 30); roll = 6;
    terminate__22ExplodeArrowProjectilei(object, 2);
    if (calls != "rand;sprintf;effect2;lump;sound;lump;sound;explode;alloc188;light;shake;del;") return 3;
    if (lump_b1 != "impct_mtl_stone3.anm" || lump_b2 != "spel_we_explarrow_hit.anm") return 4;
    if (expl_at.y != 2.0f || expl_scale < 2.99f || expl_scale > 3.01f || shake != 15.0f || shake_flag != 0) return 5;
    if (le_at.z != 3.0f || le_object != nullptr || le_n != 0) return 6;
    const float expect[12] = {255, 10, 10, 0, 0, 0, 0, 0, 120, 60, 0.5f, 0.25f};
    for (int i = 0; i < 12; ++i) if (le_f[i] != expect[i]) return 10 + i;
    reset(0, 3); levelFlags = 0x40000; roll = 0x12; alloc_ok = false;
    terminate__22ExplodeArrowProjectilei(object, 1);
    if (calls != "rand;sprintf;effect8;lump;sound;lump;sound;explode;alloc188;shake;del;" || lump_b1 != "impct_mtl_ice3.anm") return 30;
    if (expl_scale != 1.0f || shake != 1.0f) return 31;
    reset(0, -5);
    terminate__22ExplodeArrowProjectilei(object, 1);
    if (expl_scale != 1.0f || shake != -2.0f) return 32;
    return 0;
}
