// Host contract for __8SkelightG6Point3sPPcci; not an EE input. 32-bit words are offsets from a host arena; every callee appends a
// token to a call log and records the arguments that matter. The authored function was also compared with the original in a private
// R5900 run (40 scenarios, every original word covered; mutants of the stored values, the sign extension of the byte flag, the
// difficulty restore, the store order around animInitState and the weapon-slot loops are caught there).
#define W14_PROJECTILE_A_HOST 1
#include "../w14-projectile-a-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w14_projectile_a_host_base;
unsigned w14_projectile_a_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
void w14_projectile_a_host_hurt(unsigned, void*, float, void*, int) {}
static unsigned char arena[0x2000] __attribute__((aligned(16)));
static std::string calls;
static int rand_value;
static Point3 got_position; static short got_type; static void* got_map; static float got_scale; static char** got_args;
static float difficulty_at_params; static float params[5]; static int params_flag;
static unsigned long long chunks_stored_at_animinit, chunks_value = 0x1122334455667788ull;
static void* chunks_header; static int chunks_mask;
static void *anim_state, *anim_header; static int anim_flags, state_arg;
struct Event { int a; float fa; int b; float fb; int c; float fc; int d; float fd; int e; };
static Event events[2]; static int event_count;
static unsigned weapon_called_with_208;
extern "C" {
unsigned char skelightResourceMap[16];
int frameNm;
float worldDifficulty;
unsigned char vt_Skelight[16] __asm__("_vt$8Skelight");
void creature_construct(void* self, Point3 position, short type, void* map, float scale, char** arguments)
    __asm__("__8CreatureG6Point3sPQ210GameObject11resourceMapfPPc");
void creature_construct(void* self, Point3 position, short type, void* map, float scale, char** arguments) {
    calls += "creature;"; got_position = position; got_type = type; got_map = map; got_scale = scale; got_args = arguments;
    std::memset(static_cast<unsigned char*>(self) + 1000, 0x77, 200);  // stale slot bytes the constructor must overwrite
}
unsigned eRand__10GameObject(void*) { calls += "rand;"; return static_cast<unsigned>(rand_value); }
void initWeapon__8Skelight(void* self) { calls += "weapon;"; weapon_called_with_208 = *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 208); }
void setParameters__8Creaturefffffi(void*, float a, float b, float c, float d, float e, int flag) {
    calls += "params;"; difficulty_at_params = worldDifficulty; params[0] = a; params[1] = b; params[2] = c; params[3] = d; params[4] = e; params_flag = flag;
}
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* header, int mask) { calls += "chunks;"; chunks_header = header; chunks_mask = mask; return chunks_value; }
void animInitState__FP14AnimationState(void* state) {
    calls += "animinit;"; anim_state = state;
    std::memcpy(&chunks_stored_at_animinit, arena + 0x100 + 552, 8);
}
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags) { calls += "animadd;"; anim_header = header; anim_flags = flags; if (state != anim_state) calls += "BADSTATE;"; }
void setState__8Skelighti(void*, int state) { calls += "setstate;"; state_arg = state; }
void objectAddToRunList__FP10GameObject(void*) { calls += "run;"; }
void setAnimEventFlags__8Creatureififififi(void*, int a, float fa, int b, float fb, int c, float fc, int d, float fd, int e) {
    calls += "events;"; if (event_count < 2) events[event_count] = {a, fa, b, fb, c, fc, d, fd, e}; ++event_count;
}
void finishConstruction__8Creature(void*) { calls += "finish;"; }
}
#include "ctor.cpp"

static int run(int flag, int roll) {
    using namespace w14pa;
    std::memset(arena, 0xee, sizeof arena);
    calls.clear(); event_count = 0; rand_value = roll; frameNm = 777; worldDifficulty = 2.5f;
    unsigned char* o = arena + 0x100;
    word(o, 36) = 0x800; word(arena + 0x800, 12) = 0x900; word(arena + 0x800, 60) = 0xa00;
    unsigned long long old160 = 0x0000000200000004ull;
    dword(o, 160) = old160;
    word(o, 208) = 0xdeadu;
    Point3 p = {1.5f, -2.0f, 300.0f};
    char* args[1] = {nullptr};
    // type 0x12345 is passed as an int: only the low halfword counts; flag 0x100 has a zero low byte
    __8SkelightG6Point3sPPcci(o, p, 0x12345, args, flag, -9);
    std::string want = std::string("creature;") + (static_cast<signed char>(flag) != 0 ? "rand;" : "") +
        "weapon;params;chunks;animinit;animadd;setstate;run;events;events;finish;";
    if (calls != want) { std::puts(calls.c_str()); return 1; }
    if (got_position.x != 1.5f || got_position.z != 300.0f || got_type != 0x2345 || got_map != skelightResourceMap || got_scale != 10.0f || got_args != args) return 2;
    if (word(o, 72) != to_word(vt_Skelight)) return 3;
    for (int off : {1092, 1084, 1076, 1068}) if (half(o, off) != -1) return 4;
    if (o[1064] != 0xff || o[1065] != 0xff) return 5;
    for (int off = 1048; off < 1064; off += 2) if (half(o, off) != 0) return 6;
    if (o[1066] != 0x77 || o[1067] != 0x77 || o[1047] != 0x77 || o[1064 + 62] != 0x77 || o[1100] != 0x77) return 7;  // untouched neighbours
    if (o[40] != 20 || dword(o, 160) != (old160 | 0x10000ull) || word(o, 248) != 777u || word(o, 168) != 0x3f999999u || real(o, 28) != 15.0f) return 8;
    if (word(o, 960) != 4 || word(o, 176) != 0 || word(o, 192) != 0 || half(o, 572) != 0 || real(o, 96) != 1.0f || real(o, 100) != 1.0f) return 9;
    if (static_cast<signed char>(flag) != 0) {
        if (word(o, 208) != static_cast<unsigned>(roll) % 3u + 1u || weapon_called_with_208 != word(o, 208)) return 10;
    } else if (word(o, 208) != 0xdeadu) return 11;
    if (word(o, 1104) != static_cast<unsigned>(-9) || difficulty_at_params != -9.0f || worldDifficulty != 2.5f) return 12;
    for (float v : params) if (v != 1.0f) return 13;
    if (params_flag != 1) return 14;
    if (word(o, 244) != 0 || chunks_header != arena + 0x900 || chunks_mask != 0) return 15;
    if (dword(o, 552) != chunks_value || chunks_stored_at_animinit != chunks_value) return 16;
    if (anim_state != o + 288 || anim_header != arena + 0xa00 || anim_flags != 12 || state_arg != 13) return 17;
    if (half(o, 792) != -1 || word(o, 236) != 1) return 18;
    const float s = bits(0x3e4ccccc);
    if (event_count != 2) return 19;
    const Event& e0 = events[0]; const Event& e1 = events[1];
    if (e0.a != 3 || e0.fa != s || e0.b != 1 || e0.fb != 0.0f || e0.c != 0 || e0.fc != 0.0f || e0.d != 0 || e0.fd != 0.0f || e0.e != 0) return 20;
    if (e1.a != 4 || e1.fa != s || e1.b != 1 || e1.fb != 1.0f || e1.c != 1 || e1.fc != 0.0f || e1.d != 0 || e1.fd != 0.0f || e1.e != 0) return 21;
    return 0;
}
int main() {
    w14_projectile_a_host_base = arena;
    int r;
    if ((r = run(1, 7))) return r;
    if ((r = run(0, 7))) return 30 + r;
    if ((r = run(0x100, 11))) return 60 + r;
    return (r = run(-1, 0xfffffffe)) ? 90 + r : 0;
}
