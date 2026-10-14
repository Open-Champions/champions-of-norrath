// Host contract for __18DarknessProjectileG6Point3T1P5SkilliP10GameObject; not an EE input. 32-bit words are offsets from a host
// arena; every callee logs its name and the arguments that matter. The authored function was also compared with the original in a
// private R5900 run (30 scenarios, every original word covered; mutants of the factor, the stored constants, the null-owner test and
// the ambient arithmetic are caught there).
#define W14_PROJECTILE_A_HOST 1
#include "../w14-projectile-a-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w14_projectile_a_host_base;
unsigned w14_projectile_a_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
void w14_projectile_a_host_hurt(unsigned, void*, float, void*, int) {}
static unsigned char arena[0x1000] __attribute__((aligned(16)));
static std::string calls;
static Point3 got_position;
static unsigned char light_at_add[64];
static unsigned flags_at_run_list;
static void* got_owner_sound; static void* got_header; static float got_volume;
static void *link_head, *link_other, *link_item;
static std::string lump_a, lump_b;
extern "C" {
unsigned char vt_DarknessProjectile[16] __asm__("_vt$18DarknessProjectile");
void game_object_construct(void* self, Point3 position, void* map, char** names) __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
void game_object_construct(void* self, Point3 position, void* map, char** names) {
    calls += "goc;"; got_position = position;
    if (map || names) calls += "BADARGS;";
    std::memset(static_cast<unsigned char*>(self) + 12, 0, 4);
}
void lightAdd__FP5Light(void* light) { calls += "light;"; std::memcpy(light_at_add, light, sizeof light_at_add); }
void* lumpFindResource__FPcT0(char* lump, char* name) { calls += "lump;"; lump_a = lump; lump_b = name; return arena + 0x800; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume) { calls += "sound;"; got_header = header; got_owner_sound = object; got_volume = volume; }
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* other, void* item) { calls += "link;"; link_head = head; link_other = other; link_item = item; }
void lightGetAmbient__FPs(short* out) { calls += "ambient;"; out[0] = 3; out[1] = static_cast<short>(0xffff); out[2] = 0; }
void objectAddToRunList__FP10GameObject(void* object) { calls += "run;"; flags_at_run_list = *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(object) + 12); }
}
#include "ctor.cpp"

static int run(void* owner) {
    using namespace w14pa;
    std::memset(arena, 0xee, sizeof arena);
    calls.clear();
    link_head = nullptr;
    unsigned char* o = arena + 0x100;
    Point3 p = {1.5f, -2.0f, 300.0f}, v = {60.0f, -120.0f, 30.0f};
    unsigned char* skill = arena + 0x400;
    __18DarknessProjectileG6Point3T1P5SkilliP10GameObject(o, p, v, skill, -7, owner);
    std::string want = std::string("goc;light;lump;sound;") + (owner ? "link;" : "") + "ambient;run;";
    if (calls != want) { std::puts(calls.c_str()); return 1; }
    if (got_position.x != 1.5f || got_position.y != -2.0f || got_position.z != 300.0f) return 2;
    if (word(o, 72) != to_word(vt_DarknessProjectile)) return 3;
    // the light record: filled before lightAdd and handed over at o+88
    const unsigned char* l = light_at_add;
    if (l[8] != 0 || l[9] != 0) return 4;
    for (int off : {10, 12, 14, 18, 20, 22}) if (*reinterpret_cast<const short*>(l + off) != -126) return 5;
    if (*reinterpret_cast<const short*>(l + 16) != 0 || *reinterpret_cast<const short*>(l + 24) != 0) return 6;
    if (*reinterpret_cast<const float*>(l + 28) != 168.0f || *reinterpret_cast<const float*>(l + 36) != 168.0f) return 7;
    if (*reinterpret_cast<const unsigned*>(l + 32) != 0x38149b92u || *reinterpret_cast<const unsigned*>(l + 40) != 0x38149b92u) return 8;
    if (*reinterpret_cast<const unsigned*>(l + 44) != 0) return 9;
    if (real(o, 136) != 1.5f || real(o, 140) != -2.0f || real(o, 144) != 300.0f) return 10;
    const float k = bits(0x3c888888u);
    if (real(o, 76) != 60.0f * k || real(o, 80) != -120.0f * k || real(o, 84) != 30.0f * k) return 11;
    if (word(o, 164) != 0 || word(o, 168) != 0 || word(o, 148) != 108 || word(o, 152) != static_cast<unsigned>(-7) || word(o, 156) != 1 || o[160] != 0) return 12;
    if (word(o, 172) != to_word(skill)) return 13;
    if (flags_at_run_list != 0x4001u || word(o, 12) != 0x4001u) return 14;
    if (lump_a != "fx.lmp" || lump_b != "spel_de_engulf.anm") return 15;
    if (got_header != arena + 0x800 || got_owner_sound != owner || got_volume != 1.0f) return 16;
    if (owner && (link_head != o + 168 || link_other != owner || link_item != owner)) return 17;
    if (half(o, 98) != static_cast<short>(1 - 3) || half(o, 100) != static_cast<short>(1 - 0xffff) || half(o, 102) != 1) return 18;
    return 0;
}
int main() {
    w14_projectile_a_host_base = arena;
    int r = run(arena + 0x600);
    return r ? r : run(nullptr);
}
