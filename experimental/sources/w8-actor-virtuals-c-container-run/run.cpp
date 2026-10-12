#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Observed msg_run__9Container at 0x217a38, 3416 bytes.
// Fields: velocity floats at +108/+112, previous-water byte at +124 (signed), flag
// word at +104, countdown word at +116, state word at +128, sound handle at +120,
// model at +96, three resource names at +76/+88/+92, extents at +28/+32.
//
// 1. genericMove(object, {+108, +112, 0}, null).
// 2. Water: with +124 nonzero, the squared horizontal speed above 0.001 calls
//    worldPerturbWater(world, position, 300, 12) and otherwise worldCheckForWater
//    (world, position, 12, null); a zero result clears +124. With +124 clear and
//    frameNm & 7 == 0, a nonzero worldCheckForWater result sets +124, advances the
//    generator once, plays the sound named by formatting "foly_wtrsplash_sml%d.anm"
//    with 1 from the lump "fx.lmp" at gain 0.3 and calls worldPerturbWater(world,
//    position, 1000, 24).
// 3. The velocity is scaled by 0.86. With state +128 zero the object leaves the run
//    list once the squared speed is below 0.01, and the call ends.
// 4. With bit 0x4 of +104 (a fuse): a positive countdown starts the fuse sound at
//    exactly 180 (stored at +120), is decremented, sets state 2, and at zero shakes
//    the camera (10.0, false) and sets state 1; two particle bursts from the model
//    attachment point at +96 follow (random unit-ball vectors scaled 5/5/25 and
//    1.28/1.28/30.5 on the third). With a zero countdown and state 1 the object
//    explodes: sounds, makeExplosion, radial damage ((worldDifficulty-2)*20+15),
//    floor decal, a light effect, one PhProp per resource entry, stops the sound at
//    +120 and deletes itself through virtual slot 8 with 3.
// 5. Without bit 0x4, with bit 0x1 and state 1 (opened): unless the level string at
//    levelName+81 equals "tutor" and unless bit 0x20 is set, gold and possibly random
//    items drop; a sound for bit 0x100, 0x40 or 0x80 plays, six debris effects are
//    added, a PhProp per resource entry is created, and the object deletes itself.
// Unknown: the roles of the flag bits, the fuse timing, the constants and strings
// beyond what the callee names show. The unmodeled stack bytes of the item
// description stay unwritten except where the image writes them.
// Inferred: callee names as in the other sources of this package.
extern "C" void genericMove__10GameObjectG6Point3P10GameObject(void* self, const void* move, void* other);
extern "C" int worldPerturbWater__FP12_worldHeaderG6Point3ff(void* world, const void* position, float a, float b);
extern "C" int worldCheckForWater__FP12_worldHeaderG6Point3fPf(void* world, const void* position, float range, float* out);
extern "C" unsigned norrath_actor_remove_from_run_list(void* object);
extern "C" unsigned norrath_actor_random(void* object);
extern "C" unsigned char* norrath_block_alloc(int size);
extern "C" void* norrath_lump_find_resource(const char* lump_name, const char* resource_name);
extern "C" int norrath_sound_play_sequence(void* animation_header, void* object, float gain);
extern "C" int sprintf(char* out, const char* format, ...);
extern "C" int strcmp(const char* left, const char* right);
extern "C" char* strstr(const char* haystack, const char* needle);
extern "C" void gameSetCameraShake__Ffb(float amount, int flag);
extern "C" void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, const void* position, short angle, void* state, int index, void* out, void* other, float scale);
extern "C" void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, void* position, void* velocity, float age);
extern "C" void makeExplosion__FG6Point3f(const void* position, float scale);
extern "C" void objectRadialDamage__FP10GameObjectG6Point3iffiii(void* self, const void* position, int damage, float a,
                                                                 float b, int c, int d, int e);
extern "C" void floorAdd__FG6Point3sfii(const void* position, short value, float size, int a, int b);
extern "C" void __11LightEffectG6Point3ffffffffffffP10GameObjecti(
    void* block, const float* point, float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10,
    float p11, float p12, float p13, float p14, void* object, int flag);
extern "C" void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, const void* position, const char* a,
                                                              const char* b, const char* c, long layer, int id,
                                                              void* description, float scale);
extern "C" void soundStopSound__Fi(int handle);
extern "C" float gameTreasureRamp__Fv();
extern "C" void __4GoldG6Point3i(void* self, const void* position, int amount);
extern "C" void create__4itemPcPQ24item11descriptionii(const char* name, void* description, int a, int b);
extern "C" void makeRandomPotion__4itemPQ24item11description(void* description);
extern "C" int getProxyMeshId__4itemPQ24item11description(void* description);
extern "C" void initStuff__6PhPropi(void* self, int value);
extern "C" float icos__Fi(int angle);
extern "C" float isin__Fi(int angle);
extern "C" void addEffect__FiG6Point3(int effect, const void* position);
extern "C" int frameNm;
extern "C" float worldDifficulty;
extern "C" char levelName[];
extern "C" unsigned char norrath_actors_b_view_4844a0[];
extern "C" unsigned char norrath_player_view_31f540[];
extern "C" unsigned char norrath_container_msg_run_fuseParticle_bytes[80];
extern "C" unsigned char norrath_container_msg_run_smokeParticle_bytes[80];

namespace {
using namespace w8c;

typedef void (*DeleteSlot)(void*, int);

__attribute__((always_inline)) inline int signed_remainder(int value, int divisor) {
#if defined(__mips__)
    int remainder;
    __asm__("div $0, %1, %2\n\tmfhi %0" : "=r"(remainder) : "r"(value), "r"(divisor) : "hi", "lo");
    return remainder;
#else
    return value % divisor;
#endif
}

__attribute__((always_inline)) inline void* world() {
    return pointer(read_word(address(norrath_actors_b_view_4844a0)));
}

__attribute__((always_inline)) inline void copy_position(Address base, float* out) {
    out[0] = read_single(base + 16u);
    out[1] = read_single(base + 20u);
    out[2] = read_single(base + 24u);
}

__attribute__((always_inline)) inline float unit_component(void* object) {
    float value = static_cast<float>(signed_word(norrath_actor_random(object)));
    value = value * single_from_bits(0x37800000u);
    value = value + value;
    return value - 1.0f;
}

// Redraws until the vector lies inside the unit ball (squared length 1.0 allowed).
__attribute__((always_inline)) inline void random_ball(void* object, float* out) {
    do {
        out[0] = unit_component(object);
        out[1] = unit_component(object);
        out[2] = unit_component(object);
    } while (1.0f < out[0] * out[0] + out[1] * out[1] + out[2] * out[2]);
}

__attribute__((always_inline)) inline void make_resource_props(void* self, Address base, int layer_id) {
    const char* const lump_name = static_cast<const char*>(pointer(read_word(base + 76u)));
    void* const header = norrath_lump_find_resource(lump_name,
                                                    static_cast<const char*>(pointer(read_word(base + 88u))));
    const unsigned char* const bytes_header = static_cast<const unsigned char*>(header);
    int index = 0;
    if (!(static_cast<signed char>(bytes_header[18]) > 0)) return;
    do {
        unsigned char* const block = norrath_block_alloc(584);
        if (block != nullptr) {
            float position[3];
            copy_position(base, position);
            __6PhPropG6Point3PcN22liPQ24item11descriptionf(
                block, position, static_cast<const char*>(pointer(read_word(base + 76u))),
                static_cast<const char*>(pointer(read_word(base + 88u))),
                static_cast<const char*>(pointer(read_word(base + 92u))), static_cast<long>(1u << (index & 31)),
                layer_id, nullptr, 1.0f);
        }
        index += (read_word(base + 104u) & 0x200u) != 0u ? 1 : 2;
    } while (index < static_cast<signed char>(bytes_header[18]));
    (void)self;
}

__attribute__((always_inline)) inline void delete_self(void* self) {
    if (self == nullptr) return;
    void* adjusted;
    DeleteSlot function = slot_function<DeleteSlot>(self, 8u, &adjusted);
    function(adjusted, 3);
}
}

extern "C" void msg_run__9Container(void* self) {
    const Address base = address(self);
    static char lump[] = {'f', 'x', '.', 'l', 'm', 'p', '\0'};
    float move[3] = {read_single(base + 108u), read_single(base + 112u), 0.0f};
    genericMove__10GameObjectG6Point3P10GameObject(self, move, nullptr);

    if (static_cast<signed char>(read_u8(base + 124u)) != 0) {
        const float vx = read_single(base + 108u);
        const float vy = read_single(base + 112u);
        float position[3];
        copy_position(base, position);
        int result;
        if (single_from_bits(0x3a83126eu) < vx * vx + vy * vy)
            result = worldPerturbWater__FP12_worldHeaderG6Point3ff(world(), position, single_from_bits(0x43960000u),
                                                                    single_from_bits(0x41400000u));
        else
            result = worldCheckForWater__FP12_worldHeaderG6Point3fPf(world(), position, single_from_bits(0x41400000u),
                                                                      nullptr);
        if (result == 0) write_u8(base + 124u, 0u);
    } else if ((frameNm & 7) == 0) {
        float position[3];
        copy_position(base, position);
        if (worldCheckForWater__FP12_worldHeaderG6Point3fPf(world(), position, single_from_bits(0x41400000u), nullptr) != 0) {
            write_u8(base + 124u, 1u);
            norrath_actor_random(self);
            char name[64];
            sprintf(name, "foly_wtrsplash_sml%d.anm", 1);
            norrath_sound_play_sequence(norrath_lump_find_resource(lump, name), self, single_from_bits(0x3e999999u));
            float splash[3];
            copy_position(base, splash);
            worldPerturbWater__FP12_worldHeaderG6Point3ff(world(), splash, single_from_bits(0x447a0000u),
                                                          single_from_bits(0x41c00000u));
        }
    }

    const float damping = single_from_bits(0x3f5c28f5u);
    const float new_x = read_single(base + 108u) * damping;
    const float new_y = read_single(base + 112u) * damping;
    write_single(base + 108u, new_x);
    write_single(base + 112u, new_y);
    const unsigned state = read_word(base + 128u);
    if (state == 0u) {
        if (new_x * new_x + new_y * new_y < single_from_bits(0x3c23d70au)) norrath_actor_remove_from_run_list(self);
        return;
    }

    const unsigned flags = read_word(base + 104u);
    if ((flags & 4u) != 0u) {
        int countdown = signed_word(read_word(base + 116u));
        if (countdown > 0) {
            if (countdown == 180) {
                static char fuse[] = {'e', 'x', 'p', 'l', '_', 'f', 'u', 's', 'e', '.', 'a', 'n', 'm', '\0'};
                void* const sound = norrath_lump_find_resource(lump, fuse);
                write_word(base + 120u, static_cast<unsigned>(norrath_sound_play_sequence(sound, self, 1.0f)));
                countdown = signed_word(read_word(base + 116u));
            }
            countdown = countdown - 1;
            write_word(base + 128u, 2u);
            write_word(base + 116u, static_cast<unsigned>(countdown));
            if (!(countdown > 0)) {
                gameSetCameraShake__Ffb(single_from_bits(0x41200000u), 0);
                write_word(base + 128u, 1u);
            }
            float anchor[3];
            float spot[3];
            copy_position(base, spot);
            modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
                pointer(read_word(base + 96u)), spot, static_cast<short>(read_i16(base + 56u)), nullptr, 0, anchor, nullptr, 1.0f);
            float velocity[3];
            random_ball(self, velocity);
            if (velocity[2] < 0.0f) velocity[2] = -velocity[2];
            const float five = single_from_bits(0x40a00000u);
            velocity[0] = velocity[0] * five;
            velocity[1] = velocity[1] * five;
            velocity[2] = velocity[2] * five;
            velocity[2] = velocity[2] * five;
            P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_container_msg_run_fuseParticle_bytes, anchor, velocity, 0.0f);
            random_ball(self, velocity);
            if (velocity[2] < 0.0f) velocity[2] = -velocity[2];
            const float spread = single_from_bits(0x3fa3d70au);
            velocity[0] = velocity[0] * spread;
            velocity[1] = velocity[1] * spread;
            velocity[2] = velocity[2] * spread;
            velocity[2] = velocity[2] * single_from_bits(0x41c5c28fu);
            P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_container_msg_run_smokeParticle_bytes, anchor, velocity, 0.0f);
            return;
        }
        if (read_word(base + 128u) != 1u) return;
        float blast[3] = {read_single(base + 16u), read_single(base + 20u), 0.0f};
        blast[2] = read_single(base + 24u) + single_from_bits(0x416b3333u);
        static char sweeten[] = {'e', 'x', 'p', 'l', '_', 's', 'w', 'e', 'e', 't', 'e', 'n', '.', 'a', 'n', 'm', '\0'};
        static char barrel[] = {'c', 'r', 's', 'h', '_', 'b', 'a', 'r', 'r', 'e', 'l', '.', 'a', 'n', 'm', '\0'};
        norrath_sound_play_sequence(norrath_lump_find_resource(lump, sweeten), self, 1.0f);
        norrath_sound_play_sequence(norrath_lump_find_resource(lump, barrel), self, 1.0f);
        makeExplosion__FG6Point3f(blast, single_from_bits(0x40000000u));
        float centre[3];
        copy_position(base, centre);
        const int damage = static_cast<int>((worldDifficulty - single_from_bits(0x40000000u)) * single_from_bits(0x41a00000u) +
                                            single_from_bits(0x41700000u));
        objectRadialDamage__FP10GameObjectG6Point3iffiii(self, centre, damage, single_from_bits(0x42200000u),
                                                         single_from_bits(0x43340000u), 0x40800, 0, 0);
        float decal[3] = {read_single(base + 16u), read_single(base + 20u), 0.0f};
        decal[2] = read_single(base + 24u) + 1.0f;
        floorAdd__FG6Point3sfii(decal, static_cast<short>(norrath_actor_random(self)), single_from_bits(0x42400000u), 9, 0);
        unsigned char* const block = norrath_block_alloc(188);
        if (block != nullptr) {
            float where[3];
            copy_position(base, where);
            __11LightEffectG6Point3ffffffffffffP10GameObjecti(
                block, where, single_from_bits(0x43000000u), single_from_bits(0x42c80000u), single_from_bits(0x429c0000u),
                single_from_bits(0xc2000000u), 0.0f, 0.0f, 0.0f, 0.0f, single_from_bits(0x43280000u),
                single_from_bits(0x42700000u), 1.0f, single_from_bits(0x3eb33333u), nullptr, 0);
        }
        make_resource_props(self, base, 534);
        soundStopSound__Fi(signed_word(read_word(base + 120u)));
        delete_self(self);
        return;
    }
    if ((flags & 1u) == 0u) return;
    if (read_word(base + 128u) != 1u) return;

    static char tutor[] = {'t', 'u', 't', 'o', 'r', '\0'};
    bool loot = strcmp(levelName + 81, tutor) != 0;
    bool skip_to_sound = false;
    if (loot) {
        if ((read_word(base + 104u) & 0x20u) != 0u) {
            skip_to_sound = true;
        }
    }
    if (loot && !skip_to_sound) {
        norrath_actor_random(self);
        unsigned char* const gold = norrath_block_alloc(192);
        if (gold != nullptr) {
            const unsigned pick = norrath_actor_random(self);
            const float ramp = gameTreasureRamp__Fv();
            float where[3];
            copy_position(base, where);
            const int amount = static_cast<int>(static_cast<float>(static_cast<int>(pick & 3u)) * ramp) + 1;
            __4GoldG6Point3i(gold, where, amount);
        }
        if (static_cast<int>(norrath_actor_random(self)) < 7500) {
            unsigned char description[64];
            for (unsigned k = 0u; k < 4u; ++k) write_u16(address(description) + 60u - 8u * k, 0xffffu);
            description[33] = 0xff;
            description[32] = 0xff;
            for (unsigned k = 0u; k < 4u; ++k) {
                write_u16(address(description) + 16u + 2u * k, 0u);
                write_u16(address(description) + 24u + 2u * k, 0u);
            }
            const int roll = static_cast<int>(norrath_actor_random(self));
            int count = 1;
            if (0xf617 < roll) count = (0xffdb < roll) ? 3 : 2;
            int limit = 10000;
            if (strstr(levelName + 81, "tower") != nullptr) {
                count += 2;
                limit = 30000;
            }
            const int choice = static_cast<int>(norrath_actor_random(self));
            if (count > 0) {
                const bool make_arrows = choice < limit;
                int remaining = count;
                do {
                    if (make_arrows) create__4itemPcPQ24item11descriptionii("arrows", description, -1, 0);
                    else makeRandomPotion__4itemPQ24item11description(description);
                    unsigned char* const prop = norrath_block_alloc(584);
                    unsigned char* made = nullptr;
                    if (prop != nullptr) {
                        float spot[3];
                        spot[0] = read_single(base + 16u) + 0.0f;
                        spot[1] = read_single(base + 20u);
                        spot[2] = read_single(base + 24u) + single_from_bits(0x41400000u);
                        const Address globalItem = address(norrath_player_view_31f540);
                        const Address mesh = globalItem + 16u * static_cast<unsigned>(getProxyMeshId__4itemPQ24item11description(description));
                        const Address texture = globalItem + 4u + 16u * static_cast<unsigned>(getProxyMeshId__4itemPQ24item11description(description));
                        __6PhPropG6Point3PcN22liPQ24item11descriptionf(
                            prop, spot, "equipmnt.lmp", static_cast<const char*>(pointer(read_word(mesh))),
                            static_cast<const char*>(pointer(read_word(texture))), 1, 1089, description, 1.0f);
                        made = prop;
                    }
                    if (made != nullptr) {
                        const int offset = static_cast<int>(norrath_actor_random(self) & 0x3fffu) - 8192;
                        const float c = icos__Fi(read_i16(base + 56u) + offset);
                        const float s = isin__Fi(read_i16(base + 56u) + offset);
                        write_single(address(made) + 252u, c * single_from_bits(0x41c00000u));
                        write_single(address(made) + 256u, s * single_from_bits(0x41c00000u));
                        initStuff__6PhPropi(made, 1089);
                    }
                    --remaining;
                } while (remaining != 0);
            }
        }
    }

    const unsigned sound_flags = read_word(base + 104u);
    const char* sound_name = nullptr;
    if ((sound_flags & 0x100u) != 0u) sound_name = "crsh_barrel.anm";
    else if ((sound_flags & 0x40u) != 0u) sound_name = "crsh_claypot.anm";
    else if ((sound_flags & 0x80u) != 0u) sound_name = "crsh_crate.anm";
    if (sound_name != nullptr) {
        char name[64];
        sprintf(name, sound_name);  // the name itself is the format
        norrath_sound_play_sequence(norrath_lump_find_resource(lump, name), self, 1.0f);
    }
    int step = 5;
    do {
        const unsigned r1 = norrath_actor_random(self);
        --step;
        const float radius = read_single(base + 28u);
        const float vx = static_cast<float>(signed_remainder(static_cast<int>(r1), static_cast<int>(radius + radius))) - radius;
        const unsigned r2 = norrath_actor_random(self);
        const float radius2 = read_single(base + 28u);
        const float vy = static_cast<float>(signed_remainder(static_cast<int>(r2), static_cast<int>(radius2 + radius2))) - radius2;
        const unsigned r3 = norrath_actor_random(self);
        const float depth = read_single(base + 32u);
        const int effect = (read_word(base + 104u) & 0x400u) == 0u ? 2 : 8;
        const float vz = static_cast<float>(signed_remainder(static_cast<int>(r3), static_cast<int>(depth)));
        float at[3];
        at[0] = read_single(base + 16u) + vx;
        at[1] = read_single(base + 20u) + vy;
        at[2] = read_single(base + 24u) + vz;
        addEffect__FiG6Point3(effect, at);
    } while (step >= 0);
    make_resource_props(self, base, 526);
    delete_self(self);
}
