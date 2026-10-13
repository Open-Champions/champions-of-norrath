// Host contract for terminate__19MultiFireProjectilei; not an EE input. The callees log. The authored function was also compared
// with the original in a private R5900 run (150 scenarios over the done word, level flags, argument and rolls; every original
// word covered).
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_spell_msg_b_host_base;
unsigned w13_spell_msg_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::string calls, lump_a, lump_b;
static int roll;
static Point3 effect_at;
static float sound_volume;
extern "C" {
int levelFlags;
int libc_sprintf(char* b, const char* f, ...) __asm__("sprintf");
int libc_sprintf(char* b, const char* f, ...) { va_list ap; va_start(ap, f); const int n = std::vsnprintf(b, 200, f, ap); va_end(ap); calls += "sprintf;"; return n; }
int eRand__10GameObject(void*) { calls += "rand;"; return roll; }
void P_DeleteTrail__Fi(int handle) { calls += "trail" + std::to_string(handle) + ";"; }
void addEffect__FiG6Point3(int effect, Point3 p) { calls += "effect" + std::to_string(effect) + ";"; effect_at = p; }
void* lumpFindResource__FPcT0(const char* a, const char* b) { calls += "lump;"; lump_a = a; lump_b = b; return nullptr; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void*, float v) { calls += "sound;"; sound_volume = v; }
void objectAddToDeleteList__FP10GameObject(void*) { calls += "del;"; }
}
#include "terminate.cpp"

static unsigned char object[256] __attribute__((aligned(16)));
static void reset(unsigned done) {
    std::memset(object, 0, sizeof object);
    calls.clear(); roll = 0; levelFlags = 0;
    std::memcpy(object + 96, &done, 4);
    const unsigned trail = 0x305;
    std::memcpy(object + 116, &trail, 4);
    const float p[3] = {1.0f, 2.0f, 3.0f};
    std::memcpy(object + 16, p, 12);
}
int main() {
    reset(1);
    terminate__19MultiFireProjectilei(object, 1);
    if (!calls.empty()) return 1;
    reset(0);
    terminate__19MultiFireProjectilei(object, 0);
    if (calls != "trail773;del;" || object[96] != 1) return 2;
    reset(0); roll = 6;
    terminate__19MultiFireProjectilei(object, 3);
    if (calls != "trail773;rand;sprintf;effect2;lump;sound;del;" || lump_a != "fx.lmp" || lump_b != "impct_mtl_stone3.anm") return 3;
    if (effect_at.x != 1.0f || effect_at.y != 2.0f || effect_at.z != 3.0f || sound_volume != 1.0f) return 4;
    reset(0); roll = 0x13; levelFlags = 0x40000;
    terminate__19MultiFireProjectilei(object, 1);
    if (calls != "trail773;rand;sprintf;effect8;lump;sound;del;" || lump_b != "impct_mtl_ice4.anm") return 5;
    reset(0); levelFlags = ~0x40000;
    terminate__19MultiFireProjectilei(object, 1);
    if (lump_b != "impct_mtl_stone1.anm") return 6;
    return 0;
}
