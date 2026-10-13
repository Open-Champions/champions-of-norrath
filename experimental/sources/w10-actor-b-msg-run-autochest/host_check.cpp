// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
int frameNm;
int levelFlags;
int gameDifficulty;
unsigned char norrath_auto_chest_msg_run_fuseParticle_bytes[80];
unsigned char norrath_auto_chest_msg_run_smokeParticle_bytes[80];
}
static std::string calls;
static unsigned char* current;
static std::vector<unsigned> rolls;
static size_t next_roll;
static std::vector<std::vector<float>> particles;  // per call: tag, position xyz, velocity xyz, age
static std::vector<std::vector<float>> radial, explosions, decals, lights;
static float attach_in[3], attach_scale;
static short attach_angle;
static void* attach_model;
static std::string lump_seen, name_seen;
static int alloc_size, damage_seen, flags_seen;
static float radius_a, radius_b;
static int block_available;
static unsigned char block_storage[200];
static int anim_calls;
static void *anim_state, *anim_point;
static unsigned stamp_at_domy;
static int domy_arg;

extern "C" {
int animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void* state, void* point, void* other, short* extra) {
    ++anim_calls; anim_state = state; anim_point = point;
    calls += other == nullptr && extra == nullptr ? "adv;" : "adv?;";
    return -7;
}
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, const float* point, short angle, void* state, int index, float* out, float* other, float scale) {
    calls += state == nullptr && index == 0 && other == nullptr ? "attach;" : "attach?;";
    attach_model = model; std::memcpy(attach_in, point, 12); attach_angle = angle; attach_scale = scale;
    out[0] = 100.0f; out[1] = 200.0f; out[2] = 300.0f;
}
unsigned eRand__10GameObject(void* self) {
    calls += self == current ? "rand;" : "rand?;";
    return next_roll < rolls.size() ? rolls[next_roll++] : 0u;
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, float* position, float* velocity, float age) {
    calls += "particle;";
    particles.push_back({definition == norrath_auto_chest_msg_run_fuseParticle_bytes ? 1.0f : definition == norrath_auto_chest_msg_run_smokeParticle_bytes ? 2.0f : 0.0f,
                         position[0], position[1], position[2], velocity[0], velocity[1], velocity[2], age});
}
void* lumpFindResource__FPcT0(char* lump, char* name) { calls += "lump;"; lump_seen = lump; name_seen = name; return &alloc_size; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume) {
    calls += sound == &alloc_size && object == current && volume == 1.0f ? "sound;" : "sound?;";
}
void makeExplosion__FG6Point3f(const float* position, float scale) { calls += "explode;"; explosions.push_back({position[0], position[1], position[2], scale}); }
void objectRadialDamage__FP10GameObjectG6Point3iffiii(void* self, const float* position, int damage, float a, float b, int c, int d, int e) {
    calls += self == current && c == 0x40800 && d == 0 && e == 0 ? "radial;" : "radial?;";
    radial.push_back({position[0], position[1], position[2], static_cast<float>(damage), a, b});
}
void floorAdd__FG6Point3sfii(const float* position, short angle, float extent, int kind, int extra) {
    calls += kind == 9 && extra == 0 ? "floor;" : "floor?;";
    decals.push_back({position[0], position[1], position[2], static_cast<float>(angle), extent});
}
void* blockAlloc__Fi(int bytes) { calls += "alloc;"; alloc_size = bytes; return block_available ? block_storage : nullptr; }
void __11LightEffectG6Point3ffffffffffffP10GameObjecti(
    void* block, const float* point, float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10,
    float p11, float p12, float p13, float p14, void* object, int flag) {
    calls += block == block_storage && object == nullptr && flag == 0 ? "light;" : "light?;";
    lights.push_back({point[0], point[1], point[2], p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14});
}
void objectAddToDeleteList__FP10GameObject(void* object) { calls += object == current ? "delete;" : "delete?;"; }
void objectDoMyActions__FP10GameObjecti(void* object, int on) {
    calls += object == current ? "do;" : "do?;"; domy_arg = on;
    std::memcpy(&stamp_at_domy, current + 216, 4);
}
void spawnItems__9AutoChest(void* self) { calls += self == current ? "spawn;" : "spawn?;"; }
void objectRemoveFromRunList__FP10GameObject(void* object) { calls += object == current ? "unrun;" : "unrun?;"; }
}
#include "msg_run.cpp"

static void put(unsigned char* b, unsigned off, unsigned v) { std::memcpy(b + off, &v, 4); }
static unsigned get(const unsigned char* b, unsigned off) { unsigned v; std::memcpy(&v, b + off, 4); return v; }
static void put_f(unsigned char* b, unsigned off, float v) { std::memcpy(b + off, &v, 4); }
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (calls %s)\n", what, calls.c_str()); ++failures; } }
static void reset() {
    calls.clear(); particles.clear(); radial.clear(); explosions.clear(); decals.clear(); lights.clear();
    next_roll = 0; anim_calls = 0; domy_arg = -1; block_available = 1;
}
alignas(8) static unsigned char object[512];
alignas(8) static unsigned char model_storage[16];
static void setup(int flag228, int stamp232, int stamp224, unsigned phase) {
    rolls.clear();
    std::memset(object, 0x2d, sizeof object);
    put_f(object, 16, 1.5f); put_f(object, 20, 2.5f); put_f(object, 24, -3.5f);
    object[228] = static_cast<unsigned char>(flag228);
    put(object, 232, static_cast<unsigned>(stamp232)); put(object, 224, static_cast<unsigned>(stamp224));
    put(object, 216, phase); put(object, 144, 0);
    object[56] = 0x34; object[57] = 0xf2;  // negative halfword 0xf234
    object[158] = 0; object[159] = 0;
    frameNm = 1000; levelFlags = 0; gameDifficulty = 0;
    current = object;
    reset();
}
static float comp(unsigned roll) { return static_cast<float>(static_cast<int>(roll)) * 0.0000152587890625f * 2.0f - 1.0f; }

int main() {
    // Waiting chest (+228 == 0).
    for (int stamp : {990, 1000, 1010}) for (unsigned phase : {0u, 1u, 2u, 5u}) for (unsigned half158 : {0u, 0x80u, 0xffffu, 0x7fu}) {
        setup(0, stamp, 0, phase);
        object[158] = half158 & 0xff; object[159] = half158 >> 8;
        msg_run__9AutoChest(object);
        const bool fire = phase != 2u && stamp < 1000;
        std::string want = "adv;";
        if (fire) want += "do;spawn;";
        if (half158 & 0x80u) want += "unrun;";
        expect(calls == want, "waiting chest call sequence");
        expect(static_cast<int>(get(object, 192)) == -7 && anim_state == object + 152 && anim_point == object + 16, "animation result stored");
        expect(get(object, 216) == (fire ? 2u : phase), "phase");
        if (fire) expect(domy_arg == 1 && stamp_at_domy == 2u, "actions run with phase already 2");
    }
    // Destroyed chest.
    for (int stamp : {990, 1000, 1010}) {
        setup(1, stamp, 1000, 0);
        rolls = {0x4000, 0x6000, 0x2000, 0x5000, 0x3000, 0x7000};
        msg_run__9AutoChest(object);
        expect(calls == (stamp < 1000 ? "adv;rand;rand;rand;particle;rand;rand;rand;particle;" : "adv;"), "destroyed chest, countdown wait / first bursts");
    }
    {
        setup(0xff, 990, 1000 - 90, 0);  // exactly 90 is not enough
        rolls = {0x4000, 0x6000, 0x2000, 0x5000, 0x3000, 0x7000};
        msg_run__9AutoChest(object);
        expect(calls == "adv;rand;rand;rand;particle;rand;rand;rand;particle;", "no explosion at exactly 90");
        expect(particles.size() == 2 && particles[0][0] == 1.0f && particles[1][0] == 2.0f, "particle definitions");
        expect(particles[0][1] == 1.5f && particles[0][2] == 2.5f && particles[0][3] == -3.5f && particles[0][7] == 0.0f, "anchor is the position without a model");
        expect(particles[0][4] == -2.5f && particles[0][5] == -1.25f && particles[0][6] == 18.75f, "fuse velocity: scale 5, 5, 25 with z flipped");
        const float sx = comp(0x5000) * 1.28f, sy = comp(0x3000) * 1.28f;
        float sz = comp(0x7000);
        if (sz < 0.0f) sz = -sz;
        sz = sz * 1.28f;
        sz = sz * 24.72f;
        expect(particles[1][4] == sx && particles[1][5] == sy && particles[1][6] == sz, "smoke velocity");
    }
    {
        setup(1, 990, 1000 - 91, 0);
        put(object, 144, static_cast<unsigned>(reinterpret_cast<unsigned long long>(model_storage)));
        rtti_c::host_note(model_storage);
        rolls = {0x7fffffff, 0x7fffffff, 0x7fffffff, 0x4000, 0x6000, 0x7000, 0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x6000, 0xfffe0001u};
        levelFlags = 0x10000; gameDifficulty = 3;
        msg_run__9AutoChest(object);
        expect(calls == "adv;attach;rand;rand;rand;rand;rand;rand;particle;rand;rand;rand;rand;rand;rand;particle;lump;sound;explode;radial;rand;floor;alloc;light;delete;"
               , "full destruction sequence (rejected draw retried)");
        expect(attach_model == model_storage && attach_in[0] == 1.5f && attach_in[2] == -3.5f && attach_scale == 1.0f, "attachment query");
        expect(attach_angle == static_cast<short>(0xf234), "angle is signed");
        expect(particles.size() == 2 && particles[0][1] == 100.0f && particles[0][3] == 300.0f, "anchor is the attachment position");
        expect(lump_seen == "fx.lmp" && name_seen == "spel_we_explarrow_hit.anm", "sound names");
        expect(explosions.size() == 1 && explosions[0][0] == 1.5f && explosions[0][1] == 2.5f && explosions[0][2] == -3.5f + 14.7f && explosions[0][3] == 2.0f, "explosion");
        expect(radial.size() == 1 && radial[0][3] == 360.0f && radial[0][4] == 40.0f && radial[0][5] == 180.0f && radial[0][0] == 1.5f, "damage 60 * 6");
        expect(decals.size() == 1 && decals[0][2] == -3.5f + 1.0f && decals[0][4] == 48.0f && static_cast<short>(decals[0][3]) == static_cast<short>(0xfffe0001u), "decal");
        expect(alloc_size == 188 && lights.size() == 1, "light effect");
        const std::vector<float> light = {1.5f, 2.5f, -3.5f, 128.0f, 100.0f, 78.0f, -32.0f, 0, 0, 0, 0, 168.0f, 60.0f, 1.0f, 0.35f};
        expect(lights[0] == light, "light effect arguments");
    }
    for (int flags : {0, 0x10000, 0x8000, 0x18000}) for (int difficulty : {0, 1, 2, 3, 4}) {
        setup(1, 990, 0, 0);
        rolls = {0x4000, 0x6000, 0x2000, 0x5000, 0x3000, 0x7000, 5};
        levelFlags = flags; gameDifficulty = difficulty;
        msg_run__9AutoChest(object);
        int base = (flags & 0x8000) ? 220 : (flags & 0x10000) ? 60 : 15;
        if (difficulty == 2) base *= 3;
        else if (difficulty == 3) base *= 6;
        expect(radial.size() == 1 && radial[0][3] == static_cast<float>(base), "damage table");
    }
    {
        setup(1, 990, 0, 0);
        rolls = {0x4000, 0x6000, 0x2000, 0x5000, 0x3000, 0x7000, 5};
        block_available = 0;
        msg_run__9AutoChest(object);
        expect(lights.empty() && calls.size() > 6 && calls.substr(calls.size() - 13) == "alloc;delete;", "no light effect when the allocation fails, still deleted");
    }
    return failures ? 1 : 0;
}
