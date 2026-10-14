// Host contract for msg_run__12ShockOfFrost; not an EE input. 32-bit words are offsets from a host base and the callees log. The
// authored function was also compared with the original in a private R5900 run (random rolls with rejected draws, move result bits,
// frame limits, chunk counts, hit-scan results, caster counts and allocation results; every original word covered).
#define W12_SPELL_C_HOST 1
#include "msg_run.cpp"
#include <cmath>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w12_spell_c_host_base;
static std::string calls;
static void* entry_object;
unsigned w12_spell_c_host_call(unsigned fn, void* object, int count, unsigned long a1, unsigned long) {
    calls += "entry" + std::to_string(fn) + "(" + std::to_string(count) + "," + std::to_string(a1) + ");";
    entry_object = object;
    return 0;
}
static int caster_count, caster_mode, caster_halfword;
static void *caster_object, *caster_address;
int w14b_host_call3(unsigned, void* adjusted, int first, int second, void* third) {
    calls += "count;"; caster_object = adjusted; caster_mode = first; caster_halfword = second; caster_address = third; return caster_count;
}
static std::vector<int> rolls;
static size_t rolls_used;
struct Added { Point3 where, velocity; void* def; float age; };
static std::vector<Added> added;
static int move_result;
static bool alloc_ok;
static float hitscan_result;
static unsigned char blocks[8][600] __attribute__((aligned(16)));
static int blocks_used;
static std::vector<int> block_sizes;
struct Prop { void* block; Point3 where; std::string a, b, c; long kind; int flags; void* description; float scale; };
static std::vector<Prop> props;
static std::vector<void*> inited, runlist, linked;
static std::vector<void*> owners;
static Point3 transformed[2];
static std::string lump_a, lump_b;
static int effect_kind;
static float sound_volume;
extern "C" {
int frameNm;
void* world;
unsigned char norrath_w13_spell_msg_a_view_8d80a0[80];
unsigned char norrath_w13_spell_msg_a_view_8d80f0[80];
unsigned char norrath_w13_spell_msg_a_view_8d8140[80];
unsigned char vt_FrostShard[8] __asm__("_vt$10FrostShard");
int eRand__10GameObject(void*) { return rolls[rolls_used++ % rolls.size()]; }
float icos__Fi(int) { return 2.0f; }
float isin__Fi(int) { return 3.0f; }
void lightUpdatePosition__FP5Light(void*) { calls += "light;"; }
void matMakeTransYaw__FP8Matrix34G6Point3s(void*, Point3, short yaw) { calls += "yaw" + std::to_string(yaw) + ";"; }
void matTransform__FP6Point3P8Matrix34T0(Point3* out, void*, Point3* in) { calls += "xform;"; *out = *in; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float f) { added.push_back({*p, *v, d, f}); }
int move__10ProjectileG6Point3(void*, Point3) { calls += "move;"; return move_result; }
void* lumpFindResource__FPcT0(char* a, char* b) { calls += "lump;"; lump_a = a; lump_b = b; return nullptr; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void*, float v) { calls += "sound;"; sound_volume = v; }
void addEffect__FiG6Point3(int kind, Point3) { calls += "effect;"; effect_kind = kind; }
void* blockAlloc__Fi(int size) {
    calls += "alloc" + std::to_string(size) + ";"; block_sizes.push_back(size);
    if (!alloc_ok) return nullptr;
    return blocks[blocks_used++];
}
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, Point3 p, char* a, char* b, char* c, long kind, int flags, void* d, float s) {
    props.push_back({self, p, a, b, c, kind, flags, d, s});
    const float old252 = 10.0f, old256 = 20.0f, old48 = 30.0f;
    std::memcpy(static_cast<unsigned char*>(self) + 252, &old252, 4);
    std::memcpy(static_cast<unsigned char*>(self) + 256, &old256, 4);
    std::memcpy(static_cast<unsigned char*>(self) + 48, &old48, 4);
}
void initStuff__6PhPropi(void* self, int) { calls += "init;"; inited.push_back(self); }
float worldHitscan__FP12_worldHeaderG6Point3T1fi(void*, Point3, Point3, float limit, int mode) { calls += "scan;"; return limit == 2.0f && mode == 0 ? hitscan_result : 99.0f; }
void __10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap(void* self, Point3, void* owner, void* map) { calls += "shard;"; owners.push_back(owner); (void)self; (void)map; }
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void*) { calls += "link;"; linked.push_back(head); (void)link; }
void objectAddToRunList__FP10GameObject(void* o) { calls += "run;"; runlist.push_back(o); }
}
static unsigned char arena[0x3000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static void put16(unsigned off, short v) { std::memcpy(arena + off, &v, 2); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
static unsigned get32(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
static float blockf(int b, int off) { float v; std::memcpy(&v, blocks[b] + off, 4); return v; }
static short blockh(int b, int off) { short v; std::memcpy(&v, blocks[b] + off, 2); return v; }
static unsigned blockw(int b, int off) { unsigned v; std::memcpy(&v, blocks[b] + off, 4); return v; }
enum { O = 0x100, VT = 0x800, OWNN = 0x900, OWN = 0x940, CASTER = 0xa00 };
static void reset(int frame, int end) {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena); std::memset(blocks, 0, sizeof blocks);
    calls.clear(); added.clear(); props.clear(); inited.clear(); runlist.clear(); linked.clear(); owners.clear(); block_sizes.clear();
    rolls = {49152}; rolls_used = 0; entry_object = nullptr; move_result = 0; alloc_ok = true; hitscan_result = 1.0f; caster_count = 0; blocks_used = 0;
    frameNm = frame;
    putf(O + 16, 1); putf(O + 20, 2); putf(O + 24, 3); put16(O + 56, 1000);
    put32(O + 192, static_cast<unsigned>(end)); put32(O + 196, 0); put32(O + 152, 5000);
    putf(O + 156, 4); putf(O + 160, 5);
    put32(O + 72, VT); put32(VT + 8 + 4, 0x55);
    put32(O + 200, CASTER); put32(O + 204, 0x1234); put16(O + 148, 6); put32(O + 84, OWNN); put32(OWNN + 12, OWN);
}
int main() {
    // flight only: the light follows, the matrix is built from the heading less 32767, the two orbit points rotate, 64 bursts, the
    // counter drops by 2000, the move runs and the function ends because the end frame is in the future
    reset(7, 100);
    msg_run__12ShockOfFrost(arena + O);
    if (calls != "light;yaw-31767;xform;xform;move;") return 1;
    if (getf(O + 136) != 1.0f || getf(O + 144) != 3.0f || getf(O + 172) != 12.0f || getf(O + 176) != 18.0f || getf(O + 184) != -12.0f || getf(O + 188) != -18.0f) return 2;
    if (static_cast<int>(get32(O + 152)) != 3000 || added.size() != 64) return 3;
    for (int i = 0; i < 30; ++i) if (added[i].def != norrath_w13_spell_msg_a_view_8d80a0) return 4;
    for (int i = 30; i < 50; ++i) if (added[i].def != norrath_w13_spell_msg_a_view_8d80f0) return 5;
    for (int i = 50; i < 64; ++i) if (added[i].def != norrath_w13_spell_msg_a_view_8d8140) return 6;
    // first particle: 0.5 * K1 + orbit offset (2 * 12) + P.x; velocity 0.5 * K2
    if (std::fabs(added[0].where.x - (0.5f * 9.0369f + 24.0f + 1.0f)) > 1e-2 || std::fabs(added[0].where.y - (0.5f * 9.0369f + 36.0f + 2.0f)) > 1e-2) return 7;
    if (std::fabs(added[0].velocity.x - 0.5f * 31.198f) > 1e-2 || added[0].age != 0.0f) return 8;
    // third definition: no orbit offset, bases are the two rotated points, velocity 0.5 * 5
    if (std::fabs(added[50].where.x - (0.5f * 3.0f + 0.0f)) > 1e-3 || std::fabs(added[50].where.y - (0.5f * 3.0f + 12.0f)) > 1e-3 || std::fabs(added[51].where.y - (0.5f * 3.0f - 12.0f)) > 1e-3 || added[50].velocity.x != 2.5f) return 9;
    // collision bits end the flight: bit 1 always, bit 2 only with the word at +196
    reset(7, 100); move_result = 1;
    msg_run__12ShockOfFrost(arena + O);
    if (get32(O + 192) != 0 || calls.find("lump;") == std::string::npos) return 10;
    reset(7, 100); move_result = 2;
    msg_run__12ShockOfFrost(arena + O);
    if (get32(O + 192) != 100 || calls.find("lump;") != std::string::npos) return 11;
    reset(7, 100); move_result = 2; put32(O + 196, 1);
    msg_run__12ShockOfFrost(arena + O);
    if (get32(O + 192) != 0 || calls.find("lump;") == std::string::npos) return 12;
    if (lump_a != "fx.lmp" || lump_b != "spel_ew_shockfrost_hit.anm" || sound_volume != 1.0f || effect_kind != 10) return 13;
    // three ice chunks (0.75 * 4): kinds 1, 2, 4, flags 274, the strings, the sizes and the scaled singles
    if (props.size() != 3 || props[0].kind != 1 || props[1].kind != 2 || props[2].kind != 4 || props[0].flags != 274 || props[0].scale != 1.0f ||
        props[0].a != "fx.lmp" || props[0].b != "iceshard.vif" || props[0].c != "iceshard.tex" || props[0].description != nullptr) return 14;
    if (std::fabs(blockf(0, 252) - (10.0f * 0.3f + (4.0f * 0.3f) * 1.716f)) > 1e-3 || std::fabs(blockf(0, 256) - (20.0f * 0.3f + (5.0f * 0.3f) * 1.716f)) > 1e-3 ||
        std::fabs(blockf(0, 48) - 30.0f * 0.6f) > 1e-3) return 15;
    if (inited.size() != 3 || block_sizes[0] != 584) return 16;
    // allocation failure skips the chunks
    reset(7, 100); move_result = 1; alloc_ok = false;
    msg_run__12ShockOfFrost(arena + O);
    if (!props.empty() || !inited.empty()) return 17;
    // shards: the caster count n gives n shards; hit-scan results of zero take the sample heading, positive keep the first
    reset(7, 100); move_result = 1; caster_count = 2; rolls = {49152};
    msg_run__12ShockOfFrost(arena + O);
    if (caster_mode != 1 || caster_halfword != 6 || caster_object != arena + CASTER || caster_address != arena + O + 84) return 18;
    if (runlist.size() != 2 || owners.size() != 2 || owners[0] != arena + OWN || linked.size() != 2) return 19;
    const unsigned char* shard = blocks[3];
    (void)shard;
    short yaw0 = static_cast<short>(1000 + (49152u % 32767u) - 16384);
    if (blockh(3, 56) != yaw0 || blockh(3, 88) != 6 || blockw(3, 72) != w12c::to_word(vt_FrostShard) || blockw(3, 84) != 0 || blockw(3, 108) != 67 || blockw(3, 112) != 0 ||
        blockw(3, 116) != CASTER || blockw(3, 120) != 0x1234) return 20;
    if (blockf(3, 96) != 2.0f * 350.0f || blockf(3, 100) != 3.0f * 350.0f || blockf(3, 104) != 0.0f) return 21;
    reset(7, 100); move_result = 1; caster_count = 1; hitscan_result = 0.0f;
    msg_run__12ShockOfFrost(arena + O);
    if (blockh(3, 56) != static_cast<short>(49152)) return 22;
    // finally the entry at offset 8 with 3
    if (calls.substr(calls.size() - 13) != "entry85(1,3);" || entry_object != arena + O) return 23;
    return 0;
}
