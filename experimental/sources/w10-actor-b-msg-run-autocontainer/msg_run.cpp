#include "../w10-actor-b-common/fields.hpp"
// Observed msg_run__13AutoContainer at 0x221a20, 4124 bytes. Fields: velocity singles +296/+300,
// signed byte +312 (in water), flag word +292, countdown +304, sound handle +308, state word +316,
// model word +144, resource list pointer +216 (its signed byte +18 is the entry count), kind
// signed byte +288, extents singles +28/+32, byte +40 (size class), names at +109/+224/+256.
// 1. State 4 returns at once. Without bit 8 of the flags, genericMove(object, {vx, vy, 0}, null).
// 2. Water: with +312 nonzero, a squared speed above 0.001 calls worldPerturbWater(world, position,
//    300, 12) and otherwise worldCheckForWater(world, position, 12, null); a zero result clears
//    +312. With +312 clear and frameNm & 7 == 0, a nonzero worldCheckForWater(world, position, 12,
//    null) sets +312, advances the generator once, plays "foly_wtrsplash_sml1.anm" from "fx.lmp"
//    at gain 0.3 and calls worldPerturbWater(world, position, 1000, 24).
// 3. The velocity is scaled by 0.86. With state 0 the object leaves the run list once the squared
//    speed is below 0.01, and the call ends.
// 4. With flag bit 4 (a fuse): a positive countdown plays "expl_fuse.anm" (gain 1.0, handle to +308)
//    at exactly 180, is decremented, sets state 2 and, at zero or below, shakes the camera
//    (10.0, false) and sets state 1; two particle bursts from the model attachment point (angle
//    minus 16384) follow (unit-ball vectors scaled 5, 5, 25 and 1.28, 1.28, 30.5). With a
//    countdown of zero or below and state 1: the explosion sound "expl_sweeten.anm", a second sound
//    by the kind byte (0 "crsh_barrel.anm", 1 "crsh_claypot.anm", 2 "crsh_basket.anm"),
//    makeExplosion at the position raised by 14.7 (scale 2 when byte +40 is below 37, else 4),
//    radial damage (15, 60 with levelFlags 0x10000, 220 with 0x8000, 4 when levelName+81 is
//    "cellar1"; times 3 at gameDifficulty 2 and 6 at 3; radii 40 and 3 * byte, flags 0x40800), a
//    floor decal (extent min(2 * byte, 96), kind 9), a 188-byte light effect, a PhProp per
//    resource entry (skipped half the time when the tag "reduceCarnage" is nonzero; one in eight
//    of the rest set on fire), stopping the sound at +308 and deleting the object through virtual slot 8
//    with flag 3.
// 5. Without bit 4, with bit 1 and state 1: a sound by the kind byte as above, six debris effects,
//    a PhProp per resource entry (step 1 or 2 by flag 0x200), then state 4 and the flag word 0x4003;
//    gold with probability 24000/65536 (eRand below 24000, amount from gameTreasureRamp); and with a
//    second draw below 9000, one to three random items (arrows below 10000, a potion below 25000,
//    otherwise four equipment drops), each non-dropped item becoming a PhProp thrown along the facing.
// Unknown: the roles of the flag bits and of the names at +109/+224/+256. Particle definitions are
// file-local in the original unit (the same two objects the AutoChest run uses) and are imported
// under the placeholder names of that unit.
extern "C" {
extern int frameNm;
extern unsigned world;
extern int levelFlags;
extern int gameDifficulty;
extern char levelName[];
extern unsigned char norrath_auto_chest_msg_run_fuseParticle_bytes[80];
extern unsigned char norrath_auto_chest_msg_run_smokeParticle_bytes[80];
extern unsigned char global_item_view[] asm("_4item$globalItem");
void genericMove__10GameObjectG6Point3P10GameObject(void* self, const float* move, void* other);
int worldPerturbWater__FP12_worldHeaderG6Point3ff(void* world, const float* position, float a, float b);
int worldCheckForWater__FP12_worldHeaderG6Point3fPf(void* world, const float* position, float range, float* out);
int sprintf(char* out, const char* format, ...);
int strcmp(const char* left, const char* right);
void* lumpFindResource__FPcT0(char* lump, char* name);
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume);
void soundStopSound__Fi(int handle);
void objectRemoveFromRunList__FP10GameObject(void* object);
void gameSetCameraShake__Ffb(float amount, int flag);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, const float* point, short angle, void* state, int index, float* out, float* other, float scale);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, float* position, float* velocity, float age);
void makeExplosion__FG6Point3f(const float* position, float scale);
void objectRadialDamage__FP10GameObjectG6Point3iffiii(void* self, const float* position, int damage, float a, float b,
                                                      int c, int d, int e);
void floorAdd__FG6Point3sfii(const float* point, short angle, float extent, int kind, int extra);
void* blockAlloc__Fi(int bytes);
void __11LightEffectG6Point3ffffffffffffP10GameObjecti(
    void* block, const float* point, float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10,
    float p11, float p12, float p13, float p14, void* object, int flag);
char* findTagString__10GameObjectPci(void* object, char* tag, int occurrence);
int findTagInt__10GameObjectPci(void* object, char* tag, int occurrence);
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, const float* position, char* a, char* b, char* c,
                                                   long layer, int id, void* description, float scale);
void setOnFire__6PhPropi(void* prop, int value);
void initStuff__6PhPropi(void* prop, int value);
void addEffect__FiG6Point3(int effect, const float* position);
unsigned eRand__10GameObject(void* self);
float gameTreasureRamp__Fv();
void __4GoldG6Point3i(void* gold, const float* position, int amount);
void create__4itemPcPQ24item11descriptionii(char* name, void* description, int a, int b);
void makeRandomPotion__4itemPQ24item11description(void* description);
int getProxyMeshId__4itemPQ24item11description(void* description);
void dropTreasure__4itemG6Point3iffffffPci(const float* position, int angle, float a, float b, float c, float d,
                                           float e, float f, char* name, int extra);
float icos__Fi(int angle);
float isin__Fi(int angle);
}

namespace {
using namespace actor_b;
using rtti_c::single_from_bits;

__attribute__((always_inline)) inline void copy_position(void* self, float* out) {
    out[0] = field<Single>(self, 16);
    out[1] = field<Single>(self, 20);
    out[2] = field<Single>(self, 24);
}
__attribute__((always_inline)) inline void* the_world() { return rtti_c::pointer(world); }

__attribute__((always_inline)) inline float unit_component(void* self) {
    float value = static_cast<float>(static_cast<int>(eRand__10GameObject(self)));
    value = value * single_from_bits(0x37800000u);
    value = value + value;
    return value - 1.0f;
}
__attribute__((always_inline)) inline void random_ball(void* self, float* out) {
    do {
        out[0] = unit_component(self);
        out[1] = unit_component(self);
        out[2] = unit_component(self);
    } while (1.0f < out[0] * out[0] + out[1] * out[1] + out[2] * out[2]);
}

// Plays the sound chosen by the kind byte, when it is 0, 1 or 2.
__attribute__((always_inline)) inline void kind_sound(void* self) {
    const char* name = nullptr;
    const int kind = field<SByte>(self, 288);
    if (kind == 0) name = "crsh_barrel.anm";
    else if (kind == 1) name = "crsh_claypot.anm";
    else if (kind == 2) name = "crsh_basket.anm";
    if (name == nullptr) return;
    char buffer[64];
    sprintf(buffer, name);  // the name itself is the format
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), buffer), self, 1.0f);
}

// One PhProp per resource entry; the opened path steps 1 or 2 by flag 0x200, the destruction path
// always steps 1 and sets one prop in eight on fire (after any carnage skip). The layer bit is a
// 64-bit shift on the destruction path and a 32-bit one on the opened path.
__attribute__((always_inline)) inline void make_props(void* self, int id, bool destruction, bool carnage) {
    if (field<Word>(self, 216) == 0) return;
    int index = 0;
    if (!(field<SByte>(link(self, 216), 18) > 0)) return;
    do {
        if (!carnage || (eRand__10GameObject(self) & 1u) != 0) {
            void* block = blockAlloc__Fi(584);
            if (block != nullptr) {
                float where[3];
                copy_position(self, where);
                __6PhPropG6Point3PcN22liPQ24item11descriptionf(
                    block, where, reinterpret_cast<char*>(static_cast<unsigned char*>(self) + 109),
                    reinterpret_cast<char*>(static_cast<unsigned char*>(self) + 224),
                    reinterpret_cast<char*>(static_cast<unsigned char*>(self) + 256), destruction ? static_cast<long>(1ull << (index & 63)) : static_cast<long>(1u << (index & 31)), id, nullptr, 1.0f);
                if (destruction && (eRand__10GameObject(self) & 7u) == 0) setOnFire__6PhPropi(block, 1);
            }
        }
        if (!destruction) index += (field<Word>(self, 292) & 0x200u) != 0 ? 1 : 2;
        else index += 1;
        if (field<Word>(self, 216) == 0) break;
    } while (index < field<SByte>(link(self, 216), 18));
}

__attribute__((always_inline)) inline void init_description(unsigned char* description) {
    for (unsigned k = 0; k < 4; ++k) field<Half>(description, 60 - 8 * k) = 0xffffu;
    description[33] = 0xff;
    description[32] = 0xff;
    for (unsigned k = 0; k < 4; ++k) {
        field<Half>(description, 16 + 2 * k) = 0;
        field<Half>(description, 24 + 2 * k) = 0;
    }
}
}

extern "C" void msg_run__13AutoContainer(void* self) {
    if (field<Word>(self, 316) == 4) return;
    if ((field<Word>(self, 292) & 8u) == 0) {
        float move[3] = {field<Single>(self, 296), field<Single>(self, 300), 0.0f};
        genericMove__10GameObjectG6Point3P10GameObject(self, move, nullptr);
    }
    if (field<SByte>(self, 312) != 0) {
        const float vx = field<Single>(self, 296);
        const float vy = field<Single>(self, 300);
        float position[3];
        copy_position(self, position);
        int result;
        if (single_from_bits(0x3a83126eu) < vx * vx + vy * vy)
            result = worldPerturbWater__FP12_worldHeaderG6Point3ff(the_world(), position, single_from_bits(0x43960000u), single_from_bits(0x41400000u));
        else
            result = worldCheckForWater__FP12_worldHeaderG6Point3fPf(the_world(), position, single_from_bits(0x41400000u), nullptr);
        if (result == 0) field<Byte>(self, 312) = 0;
    } else if ((frameNm & 7) == 0) {
        float position[3];
        copy_position(self, position);
        if (worldCheckForWater__FP12_worldHeaderG6Point3fPf(the_world(), position, single_from_bits(0x41400000u), nullptr) != 0) {
            field<Byte>(self, 312) = 1;
            eRand__10GameObject(self);
            char name[64];
            sprintf(name, "foly_wtrsplash_sml%d.anm", 1);
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), name), self, single_from_bits(0x3e999999u));
            float splash[3];
            copy_position(self, splash);
            worldPerturbWater__FP12_worldHeaderG6Point3ff(the_world(), splash, single_from_bits(0x447a0000u), single_from_bits(0x41c00000u));
        }
    }
    const float damping = single_from_bits(0x3f5c28f5u);
    const float new_x = field<Single>(self, 296) * damping;
    const float new_y = field<Single>(self, 300) * damping;
    field<Single>(self, 296) = new_x;
    field<Single>(self, 300) = new_y;
    const unsigned state = field<Word>(self, 316);
    if (state == 0) {
        if (new_x * new_x + new_y * new_y < single_from_bits(0x3c23d70au)) objectRemoveFromRunList__FP10GameObject(self);
        return;
    }
    const unsigned flags = field<Word>(self, 292);
    if ((flags & 4u) != 0) {
        int countdown = static_cast<int>(field<Word>(self, 304));
        if (countdown > 0) {
            if (countdown == 180) {
                const int handle = soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("expl_fuse.anm")), self, 1.0f);
                field<Word>(self, 308) = static_cast<unsigned>(handle);
                countdown = static_cast<int>(field<Word>(self, 304));
            }
            countdown = countdown - 1;
            field<Word>(self, 316) = 2;
            field<Word>(self, 304) = static_cast<unsigned>(countdown);
            if (!(countdown > 0)) {
                gameSetCameraShake__Ffb(single_from_bits(0x41200000u), 0);
                field<Word>(self, 316) = 1;
            }
            float anchor[3];
            float spot[3];
            copy_position(self, spot);
            modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
                link(self, 144), spot, static_cast<short>(field<Half>(self, 56) - 16384u), nullptr, 0, anchor, nullptr, 1.0f);
            float velocity[3];
            random_ball(self, velocity);
            if (velocity[2] < 0.0f) velocity[2] = -velocity[2];
            const float five = single_from_bits(0x40a00000u);
            velocity[0] = velocity[0] * five;
            velocity[1] = velocity[1] * five;
            velocity[2] = velocity[2] * five;
            velocity[2] = velocity[2] * five;
            P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_auto_chest_msg_run_fuseParticle_bytes, anchor, velocity, 0.0f);
            random_ball(self, velocity);
            if (velocity[2] < 0.0f) velocity[2] = -velocity[2];
            const float spread = single_from_bits(0x3fa3d70au);
            velocity[0] = velocity[0] * spread;
            velocity[1] = velocity[1] * spread;
            velocity[2] = velocity[2] * spread;
            velocity[2] = velocity[2] * single_from_bits(0x41c5c28fu);
            P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_auto_chest_msg_run_smokeParticle_bytes, anchor, velocity, 0.0f);
            return;
        }
        if (state != 1) return;
        float blast[3] = {field<Single>(self, 16), field<Single>(self, 20), 0.0f};
        blast[2] = field<Single>(self, 24) + single_from_bits(0x416b3333u);
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("expl_sweeten.anm")), self, 1.0f);
        kind_sound(self);
        makeExplosion__FG6Point3f(blast, field<Byte>(self, 40) < 37u ? 2.0f : 4.0f);
        int damage = 15;
        if ((levelFlags & 0x10000) != 0) damage = 60;
        if ((levelFlags & 0x8000) != 0) damage = 220;
        if (strcmp(levelName + 81, "cellar1") == 0) damage = 4;
        if (gameDifficulty == 2) damage = damage * 3;
        else if (gameDifficulty == 3) damage = damage * 6;
        const int size = field<Byte>(self, 40);
        float centre[3];
        copy_position(self, centre);
        objectRadialDamage__FP10GameObjectG6Point3iffiii(self, centre, damage, 40.0f, static_cast<float>(size * 3), 0x40800, 0, 0);
        float decal[3] = {field<Single>(self, 16), field<Single>(self, 20), 0.0f};
        decal[2] = field<Single>(self, 24) + 1.0f;
        const short angle = static_cast<short>(eRand__10GameObject(self));
        const int span = field<Byte>(self, 40) * 2;
        floorAdd__FG6Point3sfii(decal, angle, static_cast<float>(span < 96 ? span : 96), 9, 0);
        void* block = blockAlloc__Fi(188);
        if (block != nullptr) {
            const int byte = field<Byte>(self, 40);
            float where[3];
            copy_position(self, where);
            const int reach = byte * 4;
            __11LightEffectG6Point3ffffffffffffP10GameObjecti(
                block, where, 128.0f, 100.0f, 78.0f, -32.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                static_cast<float>(300 < reach ? 300 : reach), static_cast<float>(byte), 1.0f, single_from_bits(0x3eb33333u), nullptr, 0);
        }
        bool carnage = false;
        char* tag = const_cast<char*>("reduceCarnage");
        if (findTagString__10GameObjectPci(self, tag, 0) != nullptr) carnage = findTagInt__10GameObjectPci(self, tag, 0) != 0;
        make_props(self, 534, true, carnage);
        soundStopSound__Fi(static_cast<int>(field<Word>(self, 308)));
        void* adjusted;
        rtti_c::slot_function<void (*)(void*, int)>(self, 8, &adjusted)(adjusted, 3);
        return;
    }
    if ((flags & 1u) == 0) return;
    if (state != 1) return;
    kind_sound(self);
    int step = 5;
    do {
        const unsigned r1 = eRand__10GameObject(self);
        --step;
        const float radius = field<Single>(self, 28);
        const float vx = static_cast<float>(static_cast<int>(r1) % static_cast<int>(radius + radius)) - radius;
        const unsigned r2 = eRand__10GameObject(self);
        const float radius2 = field<Single>(self, 28);
        const float vy = static_cast<float>(static_cast<int>(r2) % static_cast<int>(radius2 + radius2)) - radius2;
        const unsigned r3 = eRand__10GameObject(self);
        const float depth = field<Single>(self, 32);
        const int effect = (field<Word>(self, 292) & 0x400u) == 0 ? 2 : 8;
        const float vz = static_cast<float>(static_cast<int>(r3) % static_cast<int>(depth));
        float at[3];
        at[0] = field<Single>(self, 16) + vx;
        at[1] = field<Single>(self, 20) + vy;
        at[2] = field<Single>(self, 24) + vz;
        addEffect__FiG6Point3(effect, at);
    } while (step >= 0);
    make_props(self, 526, false, false);
    field<Word>(self, 316) = 4;
    field<Word>(self, 12) = 16387;
    if (eRand__10GameObject(self) < 24000u) {
        void* gold = blockAlloc__Fi(192);
        if (gold != nullptr) {
            const unsigned pick = eRand__10GameObject(self);
            const float ramp = gameTreasureRamp__Fv();
            float where[3];
            copy_position(self, where);
            const int amount = static_cast<int>(static_cast<float>(static_cast<int>(pick & 3u)) * ramp) + 1;
            __4GoldG6Point3i(gold, where, amount);
        }
    }
    if (!(static_cast<int>(eRand__10GameObject(self)) < 9000)) return;
    unsigned char description[64];
    init_description(description);
    const int roll = static_cast<int>(eRand__10GameObject(self));
    int count = 1;
    if (0xf617 < roll) count = (0xffdb < roll) ? 3 : 2;
    const int choice = static_cast<int>(eRand__10GameObject(self));
    if (count <= 0) return;
    const bool make_arrows = choice < 10000;
    int remaining = count;
    do {
        bool dropped = false;
        if (make_arrows) {
            create__4itemPcPQ24item11descriptionii(const_cast<char*>("arrows"), description, -1, 0);
        } else if (choice < 25000) {
            makeRandomPotion__4itemPQ24item11description(description);
        } else {
            struct Drop { float strength; float chance; };
            const Drop drops[4] = {{1.5f, 1.0f}, {2.5f, single_from_bits(0x3c23d70au)}, {3.5f, single_from_bits(0x3a83126eu)}, {4.5f, single_from_bits(0x38d1b717u)}};
            for (const Drop& drop : drops) {
                float where[3];
                copy_position(self, where);
                dropTreasure__4itemG6Point3iffffffPci(where, field<SHalf>(self, 56), 36.0f, 2.0f, 0.0f, 0.0f, drop.strength, drop.chance, nullptr, 0);
            }
            dropped = true;
        }
        if (!dropped) {
            void* prop = blockAlloc__Fi(584);
            void* made = nullptr;
            if (prop != nullptr) {
                float spot[3];
                spot[0] = field<Single>(self, 16) + 0.0f;
                spot[1] = field<Single>(self, 20);
                spot[2] = field<Single>(self, 24) + single_from_bits(0x41400000u);
                void* mesh = global_item_view + 16 * getProxyMeshId__4itemPQ24item11description(description);
                void* texture = global_item_view + 4 + 16 * getProxyMeshId__4itemPQ24item11description(description);
                __6PhPropG6Point3PcN22liPQ24item11descriptionf(
                    prop, spot, const_cast<char*>("equipmnt.lmp"), static_cast<char*>(rtti_c::pointer(field<Word>(mesh, 0))),
                    static_cast<char*>(rtti_c::pointer(field<Word>(texture, 0))), 1, 1089, description, 1.0f);
                made = prop;
            }
            if (made != nullptr) {
                const int offset = static_cast<int>(eRand__10GameObject(self) & 0x3fffu) - 8192;
                const float c = icos__Fi(field<SHalf>(self, 56) + offset);
                const float s = isin__Fi(field<SHalf>(self, 56) + offset);
                field<Single>(made, 252) = c * single_from_bits(0x41c00000u);
                field<Single>(made, 256) = s * single_from_bits(0x41c00000u);
                initStuff__6PhPropi(made, 1089);
            }
        }
        --remaining;
    } while (remaining != 0);
}
