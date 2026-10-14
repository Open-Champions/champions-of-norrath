#define NORRATH_W8C_HOST 1
#include "run.cpp"
#include "../w8-actor-virtuals-c-bytes/host_vcall.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
int frameNm = 0;
float worldDifficulty = 1.0f;
char levelName[400];
unsigned char norrath_actors_b_view_4844a0[4];
unsigned char norrath_player_view_31f540[2000];
unsigned char norrath_container_msg_run_fuseParticle_bytes[80];
unsigned char norrath_container_msg_run_smokeParticle_bytes[80];
static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static int counts[32];
enum { MOVE, PERTURB, CHECK_WATER, REMOVE, RAND, ALLOC, LUMP, SOUND, SPRINTF, SHAKE, ATTACH, PARTICLE, EXPLOSION, DAMAGE,
       FLOOR, LIGHT, PROP, STOP, GOLD, CREATE, POTION, PROXY, INIT, EFFECT, DELETE_CALL };
static int check_answer = 0, perturb_answer = 0;
static unsigned rand_fixed = 0;
static int effects[16], effect_count = 0, props[8][2], prop_count = 0, shake_flag = -1;
static float last_damage = 0;
static int damage_arg = 0;
static unsigned char header_block[64], block_store[8][700];
static int block_next = 0;
static char sprintf_text[64];
static void* deleted_object = nullptr;
extern "C" void genericMove__10GameObjectG6Point3P10GameObject(void*, const void* m, void*) { ++counts[MOVE]; float v[3]; std::memcpy(v, m, 12); last_damage = v[2]; }
extern "C" int worldPerturbWater__FP12_worldHeaderG6Point3ff(void*, const void*, float, float) { ++counts[PERTURB]; return perturb_answer; }
extern "C" int worldCheckForWater__FP12_worldHeaderG6Point3fPf(void*, const void*, float, float*) { ++counts[CHECK_WATER]; return check_answer; }
extern "C" unsigned norrath_actor_remove_from_run_list(void*) { ++counts[REMOVE]; return 0; }
static bool rand_cycle = false;
static unsigned lcg = 12345u;
extern "C" unsigned norrath_actor_random(void*) {
    ++counts[RAND];
    if (!rand_cycle) return rand_fixed;
    lcg = lcg * 1664525u + 1013904223u;
    return lcg >> 16;
}
extern "C" unsigned char* norrath_block_alloc(int) { ++counts[ALLOC]; return block_store[block_next++ % 8]; }
extern "C" void* norrath_lump_find_resource(const char*, const char*) { ++counts[LUMP]; return header_block; }
extern "C" int norrath_sound_play_sequence(void*, void*, float) { ++counts[SOUND]; return 77; }
extern "C" int sprintf(char* out, const char* fmt, ...) { ++counts[SPRINTF]; std::strcpy(out, fmt); std::strcpy(sprintf_text, fmt); return 0; }
extern "C" int strcmp(const char* a, const char* b) { std::size_t i = 0; while (a[i] && a[i] == b[i]) ++i; return (unsigned char)a[i] - (unsigned char)b[i]; }
extern "C" char* strstr(const char* h, const char* n) { std::size_t m = std::strlen(n); for (; *h; ++h) if (!std::strncmp(h, n, m)) return const_cast<char*>(h); return nullptr; }
extern "C" void gameSetCameraShake__Ffb(float, int flag) { ++counts[SHAKE]; shake_flag = flag; }
extern "C" void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void*, const void*, short, void*, int, void* out, void*, float) {
    ++counts[ATTACH]; float a[3] = {1.0f, 2.0f, 3.0f}; std::memcpy(out, a, 12); }
extern "C" void P_AddParticle__FP11ParticleDefP6Point3T1f(void* def, void*, void* vel, float) {
    ++counts[PARTICLE];
    float v[3]; std::memcpy(v, vel, 12);
    expect(v[0] * v[0] + v[1] * v[1] <= 25.0f * 25.0f * 2.0f + 1.0f, "velocity is bounded");
    expect(v[2] >= 0.0f, "vertical component made non-negative");
    (void)def;
}
extern "C" void makeExplosion__FG6Point3f(const void*, float) { ++counts[EXPLOSION]; }
extern "C" void objectRadialDamage__FP10GameObjectG6Point3iffiii(void*, const void*, int damage, float, float, int, int, int) { ++counts[DAMAGE]; damage_arg = damage; }
extern "C" void floorAdd__FG6Point3sfii(const void*, short, float, int, int) { ++counts[FLOOR]; }
extern "C" void __11LightEffectG6Point3ffffffffffffP10GameObjecti(void*, const float*, float, float, float, float, float, float, float, float, float, float, float, float, void*, int) { ++counts[LIGHT]; }
extern "C" void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void*, const void*, const char*, const char*, const char*, long layer, int id, void*, float) {
    ++counts[PROP]; if (prop_count < 8) { props[prop_count][0] = static_cast<int>(layer); props[prop_count][1] = id; ++prop_count; }
}
extern "C" void soundStopSound__Fi(int h) { ++counts[STOP]; expect(h == 0x1234, "sound handle from +120"); }
extern "C" float gameTreasureRamp__Fv() { return 2.0f; }
extern "C" void __4GoldG6Point3i(void*, const void*, int amount) { ++counts[GOLD]; expect(amount == static_cast<int>((rand_fixed & 3u) * 2.0f) + 1, "gold amount"); }
extern "C" void create__4itemPcPQ24item11descriptionii(const char*, void*, int, int) { ++counts[CREATE]; }
extern "C" void makeRandomPotion__4itemPQ24item11description(void*) { ++counts[POTION]; }
extern "C" int getProxyMeshId__4itemPQ24item11description(void*) { ++counts[PROXY]; return 0; }
extern "C" void initStuff__6PhPropi(void*, int) { ++counts[INIT]; }
extern "C" float icos__Fi(int) { return 1.0f; }
extern "C" float isin__Fi(int) { return 0.0f; }
extern "C" void addEffect__FiG6Point3(int e, const void*) { ++counts[EFFECT]; if (effect_count < 16) effects[effect_count++] = e; }
static void delete_slot(void* self, int how) { ++counts[DELETE_CALL]; deleted_object = self; expect(how == 3, "delete flag 3"); }

static unsigned char object[800], table[256];
static void setup(unsigned flags, unsigned state, int countdown) {
    using namespace w8c;
    std::memset(object, 0, sizeof object); std::memset(table, 0, sizeof table);
    std::memset(counts, 0, sizeof counts); std::memset(block_store, 0, sizeof block_store);
    host_install_slot(object, table, 8u, 4, reinterpret_cast<void*>(delete_slot));
    write_word(address(object) + 104u, flags); write_word(address(object) + 128u, state);
    write_word(address(object) + 116u, static_cast<unsigned>(countdown));
    write_single(address(object) + 28u, 5.0f); write_single(address(object) + 32u, 4.0f);
    write_single(address(object) + 108u, 4.0f); write_single(address(object) + 112u, 0.0f);
    write_word(address(object) + 120u, 0x1234u);
    header_block[18] = 3;
    effect_count = prop_count = 0; check_answer = 0; perturb_answer = 0; rand_fixed = 5; block_next = 0; deleted_object = nullptr;
    std::strcpy(levelName + 81, "grove");
}
int main() {
    using namespace w8c;
    // idle, state 0, slow: damped and removed from the run list
    setup(0, 0, 0); write_single(address(object) + 108u, 0.05f); write_single(address(object) + 112u, 0.0f);
    msg_run__9Container(object);
    expect(counts[MOVE] == 1 && counts[REMOVE] == 1 && read_single(address(object) + 108u) < 0.05f, "slow idle leaves the run list");
    setup(0, 0, 0);
    msg_run__9Container(object);
    expect(counts[REMOVE] == 0 && std::fabs(read_single(address(object) + 108u) - 3.44f) < 1e-5f, "fast idle is damped and stays");
    // water: a moving object over water keeps its flag via perturb; none clears it
    setup(0, 0, 0); object[124] = 1; perturb_answer = 0;
    msg_run__9Container(object);
    expect(counts[PERTURB] == 1 && counts[CHECK_WATER] == 0 && object[124] == 0, "perturb path clears when zero");
    setup(0, 0, 0); write_single(address(object) + 108u, 0.0f); object[124] = 1; check_answer = 1;
    msg_run__9Container(object);
    expect(counts[CHECK_WATER] == 1 && counts[PERTURB] == 0 && object[124] == 1, "slow object checks water");
    setup(0, 0, 0); frameNm = 8; check_answer = 1;
    msg_run__9Container(object);
    expect(object[124] == 1 && counts[PERTURB] == 1 && counts[SPRINTF] == 1 && std::strcmp(sprintf_text, "foly_wtrsplash_sml%d.anm") == 0 && counts[SOUND] == 1, "splash on entry every eighth frame");
    // fuse: countdown 180 plays the fuse sound, decrements, state 2, spawns two particles
    rand_cycle = true;
    setup(4, 1, 180); frameNm = 1;
    msg_run__9Container(object);
    expect(counts[SOUND] == 1 && counts[LUMP] == 1 && read_word(address(object) + 120u) == 77u, "fuse sound stored at +120");
    expect(read_word(address(object) + 116u) == 179u && read_word(address(object) + 128u) == 2u && counts[PARTICLE] == 2 && counts[ATTACH] == 1, "fuse tick");
    setup(4, 1, 1); frameNm = 1;
    msg_run__9Container(object);
    expect(read_word(address(object) + 116u) == 0u && read_word(address(object) + 128u) == 1u && counts[SHAKE] == 1 && shake_flag == 0, "last tick shakes the camera and arms the blast");
    rand_cycle = false;
    // explode: countdown 0 and state 1
    setup(4, 1, 0); frameNm = 1; worldDifficulty = 3.0f;
    msg_run__9Container(object);
    expect(counts[EXPLOSION] == 1 && counts[DAMAGE] == 1 && damage_arg == 35 && counts[FLOOR] == 1 && counts[LIGHT] == 1, "blast effects and damage 35");
    expect(counts[PROP] == 2 && props[0][0] == 1 && props[1][0] == 4 && props[0][1] == 534, "props for entries 0 and 2 (step 2)");
    expect(counts[STOP] == 1 && counts[DELETE_CALL] == 1 && deleted_object == object + 4, "sound stopped, object deleted");
    setup(4, 2, 0); frameNm = 1;
    msg_run__9Container(object);
    expect(counts[EXPLOSION] == 0 && counts[DELETE_CALL] == 0, "state 2 with no countdown does nothing");
    // opened: flag 1 with state 1; loot on a normal level, none on the tutorial level
    setup(0x101, 1, 0); frameNm = 1; rand_fixed = 7;
    msg_run__9Container(object);
    expect(counts[GOLD] == 1 && counts[CREATE] == 1 && counts[DELETE_CALL] == 1 && counts[EFFECT] == 6 && counts[PROP] == 3, "open drops gold and one item, six effects, three props, deletes");
    expect(counts[SPRINTF] == 1 && std::strcmp(sprintf_text, "crsh_barrel.anm") == 0, "barrel sound for bit 0x100");
    setup(0x1, 1, 0); frameNm = 1; rand_fixed = 7;
    std::strcpy(levelName + 81, "tutor");
    msg_run__9Container(object);
    expect(counts[GOLD] == 0 && counts[CREATE] + counts[POTION] == 0 && counts[EFFECT] == 6, "tutorial level drops nothing");
    setup(0x21, 1, 0); frameNm = 1; rand_fixed = 7;
    msg_run__9Container(object);
    expect(counts[GOLD] == 0 && counts[EFFECT] == 6, "bit 0x20 skips the loot");
    setup(0x401 | 0x40, 1, 0); frameNm = 1; rand_fixed = 100;
    msg_run__9Container(object);
    expect(counts[GOLD] == 1 && counts[CREATE] + counts[POTION] >= 1 && effects[0] == 8 && std::strcmp(sprintf_text, "crsh_claypot.anm") == 0, "items, claypot sound and effect 8 for bit 0x400");
    setup(0x2, 1, 0);
    msg_run__9Container(object);
    expect(counts[DELETE_CALL] == 0 && counts[GOLD] == 0, "no open bit, no flag 4: nothing");
    return failures ? 1 : 0;
}
