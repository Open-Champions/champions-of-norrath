// Host contract for msg_run__9FireStorm; not an EE input. 32-bit words are offsets from a host base and the callees log.
// The authored function was also compared with the original in a private R5900 run (113 scenarios over frameNm, the start and
// end words, the owner word, radius, growth, list contents, roll lists and allocation failure; every original word covered
// except seven that are slots after the first of the description), including the exact rsqrt.s result.
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w13_spell_msg_b_host_base;
static std::string calls;
static int e112_result = 40, w160_result = 1;
static std::vector<int> rolls;
static unsigned roll_index;
unsigned w13_spell_msg_b_host_call(unsigned fn, void*, int, unsigned long a1, unsigned long a2) {
    if (fn == 0x700) { calls += "w160;"; return w160_result; }
    if (fn == 0x710) { calls += "e112(" + std::to_string(static_cast<int>(a1)) + "," + std::to_string(static_cast<int>(a2)) + ");"; return e112_result; }
    calls += "entry" + std::to_string(fn) + "(" + std::to_string(a1) + ");";
    return 0;
}
static std::vector<unsigned> listed;
static int attacks;
static short slot[4];
static std::string lump_a, lump_b;
static Point3 ctor_position, box_low;
static bool alloc_ok = true;
static unsigned char block[112];
static float height_arg;
extern "C" {
int frameNm;
void* world = reinterpret_cast<void*>(0x44);
unsigned char vt_14FireStormFlare[8] __asm__("_vt$14FireStormFlare");
unsigned eRand__10GameObject(void*) { return rolls[roll_index++ % rolls.size()]; }
float isin__Fi(int a) { calls += "sin" + std::to_string(a) + ";"; return 0.5f; }
float worldFindHeight__FP12_worldHeaderG6Point3f(void*, Point3, float radius) { height_arg = radius; return 7.25f; }
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return alloc_ok ? block : nullptr; }
unsigned __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* self, Point3 p, void* map, char** args) {
    calls += "ctor;"; ctor_position = p; if (map || args) calls += "BADCTOR;";
    unsigned flags = 1; std::memcpy(static_cast<unsigned char*>(self) + 12, &flags, 4);
    return 0;
}
void objectUpdateInGrid__FP10GameObject(void*) { calls += "grid;"; }
void objectAddToRunList__FP10GameObject(void*) { calls += "runlist;"; }
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3, void** found, int capacity) {
    calls += "box;"; box_low = low; if (capacity != 64) calls += "BADCAP;";
    for (size_t i = 0; i < listed.size(); ++i) found[i] = w13_spell_msg_b_host_base + listed[i];
    found[listed.size()] = nullptr;
}
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void*, void*, float x, float y, int p, int, int, int s, void* description, bool) {
    ++attacks; if (x != 0.0f || y != 0.0f || p != 100000 || s != 64) calls += "BADATTACK;";
    std::memcpy(slot, static_cast<unsigned char*>(description) + 36, 8);
    return 0;
}
void* lumpFindResource__FPcT0(char* a, char* b) { lump_a = a; lump_b = b; calls += "lump;"; return reinterpret_cast<void*>(0x55); }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void*, float v) { calls += "sound" + std::to_string(static_cast<int>(v)) + ";"; }
}
#include "msg_run.cpp"

static unsigned char arena[0x4000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
static unsigned get32(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
enum { O = 0x100, Q = 0x900, W = 0xa00, B = 0xb00, TW = 0xc00, TB = 0xd00, OT = 0xe00 };
static void reset(int frame, int start, int end, int radius, float growth) {
    w13_spell_msg_b_host_base = arena;
    std::memset(arena, 0, sizeof arena); std::memset(block, 0, sizeof block);
    calls.clear(); listed.clear(); attacks = 0; roll_index = 0; rolls = {49152}; frameNm = frame; e112_result = 40; w160_result = 1; alloc_ok = true;
    put32(O + 152, start); put32(O + 148, end); put32(O + 76, Q); put32(Q + 12, W); put32(W + 12, 0x40);
    put32(O + 156, radius); putf(O + 160, growth); put32(O + 140, 6); put32(O + 144, B);
    put32(B + 96, TB); put32(TB + 112 + 4, 0x710); put32(W + 72, TW); put32(TW + 160 + 4, 0x700); put32(O + 72, OT); put32(OT + 8 + 4, 0x900);
    putf(O + 16, 1); putf(O + 20, 2); putf(O + 24, 3);
}
int main() {
    unsigned char* const o = arena + O;
    reset(10, 20, 100, 10, 0.0f);  // start is not below frameNm
    msg_run__9FireStorm(o);
    if (!calls.empty()) return 1;
    reset(150, 20, 100, 10, 0.0f);  // past the end: entry 8 with 3
    msg_run__9FireStorm(o);
    if (calls != "entry2304(3);") { std::puts(calls.c_str()); return 2; }
    reset(50, 20, 1000, 10, 0.0f); put32(O + 76, 0);  // no owner: also entry 8
    msg_run__9FireStorm(o);
    if (calls != "entry2304(3);") return 3;
    // a quiet frame, remaining time 61 or more: F = 2S + (int)((sin + sin) * (S / R * 12)) with S = 5, R = 4 -> 10 + 15
    reset(41, 20, 1000, 4, 5.0f);  // S >= R: no growth call
    msg_run__9FireStorm(o);
    if (calls != "sin123000;sin61500;" || getf(O + 108) != 10.0f + 15.0f || getf(O + 116) != 25.0f || getf(O + 112) != 1.0f / 625.0f || getf(O + 120) != getf(O + 112)) { std::puts(calls.c_str()); return 4; }
    // remaining below 61: F = 2S * (rem / 60)
    reset(970, 20, 1000, 4, 5.0f);
    msg_run__9FireStorm(o);
    if (calls != "" || getf(O + 108) != 10.0f * (30.0f * w13smb::bits(0x3c888888u))) { std::puts(calls.c_str()); return 5; }
    // S below R: the entry gives n = 40: frameNm - (end - 2400) is 31 or more, so S gains 1.0
    reset(41, 20, 1000, 10, 5.0f);
    msg_run__9FireStorm(o);
    if (calls.substr(0, 11) != "e112(2,6);s" || getf(O + 160) != 6.0f) { std::puts(calls.c_str()); return 6; }
    reset(41, 20, 100, 10, 5.0f); e112_result = 0;  // 41 - 100 < 31
    msg_run__9FireStorm(o);
    if (getf(O + 160) != 5.0f) return 7;
    // frame 45: a flare is spawned (45 % 15 == 0), the owner's flag bit 0x40 gives the larger size factor
    reset(45, 20, 1000, 10, 0.0f);
    msg_run__9FireStorm(o);
    if (calls.find("alloc112;ctor;grid;runlist;") == std::string::npos) { std::puts(calls.c_str()); return 8; }
    unsigned flags; std::memcpy(&flags, block + 12, 4);
    unsigned model; std::memcpy(&model, block + 72, 4);
    unsigned a76, b84, w108; std::memcpy(&a76, block + 76, 4); std::memcpy(&b84, block + 84, 4); std::memcpy(&w108, block + 108, 4);
    float size, scaled; std::memcpy(&size, block + 80, 4); std::memcpy(&scaled, block + 28, 4);
    if (flags != (1u | 0x421du) || a76 != 6u || b84 != static_cast<unsigned>(B) || w108 != 165u) return 9;
    const float expected_size = 6.0f * w13smb::bits(0x3d4cccccu) * 0.5f + w13smb::bits(0x3e999999u);
    if (size != expected_size || scaled != expected_size * 12.0f || ctor_position.z != w13smb::bits(0x437005b8u) + 3.0f) return 10;
    if (model != w13smb::to_word(vt_14FireStormFlare)) return 11;
    if (height_arg != 12.0f) return 12;
    // the small size factor when W lacks the flag bit; a failed allocation spawns nothing
    reset(45, 20, 1000, 10, 0.0f); put32(W + 12, 0);
    msg_run__9FireStorm(o);
    std::memcpy(&size, block + 80, 4);
    if (size != 6.0f * w13smb::bits(0x3ca3d70au) * 0.5f + w13smb::bits(0x3e999999u)) return 13;
    reset(45, 20, 1000, 10, 0.0f); alloc_ok = false;
    msg_run__9FireStorm(o);
    if (calls.find("ctor") != std::string::npos || calls.find("alloc112;") == std::string::npos) return 14;
    // frame 60: the pulse, two listed objects, the second refused by the 160 entry
    reset(60, 20, 1000, 10, 0.0f); listed = {0x1000, 0x1400};
    msg_run__9FireStorm(o);
    if (calls.find("box;w160;e112(0,6);") == std::string::npos || attacks != 2 || slot[0] != 21 || slot[1] != 20 || slot[2] != 20 + static_cast<int>(20.0f * 0.75f * w13smb::bits(0x3e4cccccu)) || slot[3] != 0) { std::puts(calls.c_str()); return 15; }
    if (calls.find("lump;sound1;") == std::string::npos || lump_a != "fx.lmp" || lump_b != "spel_ew_firestorm_hit.anm" || box_low.x != 1.0f - 10.0f) return 16;
    reset(60, 20, 1000, 10, 0.0f); listed = {0x1000}; w160_result = 0;
    msg_run__9FireStorm(o);
    if (attacks != 0 || calls.find("lump") != std::string::npos) return 17;
    reset(60, 20, 1000, 10, 0.0f);
    msg_run__9FireStorm(o);
    if (calls.find("box;") == std::string::npos || attacks != 0) return 18;
    return 0;
}
