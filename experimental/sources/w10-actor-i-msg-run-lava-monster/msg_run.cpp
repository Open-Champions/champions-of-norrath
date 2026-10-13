// Observed msg_run__11LavaMonster at 0x001ebaf8, 5540 bytes: the per-frame routine of LavaMonster.
// "object" is the argument; offsets are bytes. W(x) is the word at +x, h56 the halfword at +56 (the
// heading), h294 the halfword at +294. R is the result of creatureAdvanceAnimation. "SS(n)" is virtual
// slot 184 (table entry at word(+72) + 184: a signed halfword adjustment, then a function word at +4)
// called with the adjusted object and n; slot 128 gives the model header (getModel__11LavaMonster in
// this class); slot 168 and slot 176 are called as noted. t is the resource block pointed to by W(+36).
// The state is the halfword at +572; target is W(+576), a list node whose word +12 is the object.
// eiz(r, s) is enemyInZone(object, r, s).
//  1. When bit 11 of the 64-bit word at +160 is clear, newDamageHandler(object, slot-128 result) is
//     called. Then everyFrame(object, slot-128 result): nonzero returns at once. A zero
//     objectActivationCheck(object, 600) returns at once. R = creatureAdvanceAnimation(object, 1.0f);
//     slideBack(object, 0.2f); slot 168 runs with the adjusted object; move2 is called with a zero vector.
//  2. A state of 16 or more (halfword compared unsigned) skips the case. Otherwise it selects one
//     (2, 4, 7, 11 and 12 do nothing).
// Cases (face = with a target, h56 = iatan2(target.y - y, target.x - x) using the singles at +20 and +16):
//   0: bit 11 of the word at +160 is cleared; unless W(+1048) is 1: with h294 bit 5 set, face; else
//      eiz(400, 32767) or eiz(500, 16000) -> SS(3); otherwise W(+256) < W(+808) -> SS(1).
//   1: eiz(300, 32767) or eiz(400, 16000) -> SS(0); otherwise smartWander.
//   3: no target -> SS(0). eiz(A + 40, 6000) -> SS(6) (A is the single at +28); eiz(150, 2000) -> SS(14);
//      when (float)(int)W(+964) is below the single at +560 and the signed byte at +568 is nonzero:
//      eiz(400, 8000) -> SS(15), else steerAndRouteToEnemy; otherwise eiz(A + 20, 32767) true ->
//      turnToFaceEnemy, false -> steerAndRouteToEnemy.
//   5: eiz(A + 40, 6000) -> SS(6); eiz(400, 8000) -> SS(15); bExitStatePersueIdle nonzero -> SS(3).
//   6 and 14: with a target and R bit 0: produceMeleeDamage (6: 36.0f, h200, h202, h204, flag 1, the
//      description at +976; 14: 60.0f with flags 0x80001 when eRand % 3 is zero, otherwise 0x40001);
//      then h294 bit 3 clear -> SS(3) at once; else turnToFaceEnemy. Last, h294 bit 3 clear -> SS(3).
//   8: h294 bit 4 clear -> SS(3).   9: W(+804) < W(+808) -> SS(3).   13: face; eiz(300, 32767) or
//      eiz(400, 16000) -> SS(0).
//  15: no target: only the final step. Otherwise a lead position is found with findEnemyLeadPosition
//      (400.0f) from the point 300 ahead of the heading. R bit 0: W(+960) = 1. Else R bit 1: W(+960) = 0
//      and a lava bomb (MissileWeapon with the vt$8LavaBomb table, W(t + 252) stored at +456) is launched
//      from the attachment position with a heading limited to 4000 around h56 and jittered, a velocity
//      of 400 along it and a vertical part from the distance to the lead position; its sound
//      (t[244]) is played and W(+256) = eRand % 30 + 30. Final step: with h294 bit 3 set, slot 176 runs
//      with (adjusted object, 0, the lead position) and a still-set bit goes on; any clear bit -> SS(3).
//  10: the byte at +40 becomes 80. With W(+1052) nonzero and R bit 2: the sound t[252] plays, sparks are
//      added at random shadow points, 15 PhProp pieces are built, bit 14 and 1 of W(+12) and bit 41
//      of the word at +160 are set and the object leaves the run list. With W(+1052) zero: the single at
//      +968 above 0.3 shrinks by 0.005 and, when W(+808) & 3 is zero, particles are added at shadow
//      points; otherwise (single not above 0.3) the object leaves the run list.
//  3. After the case: unless the state is 10 or W(+808) & 3 is nonzero, six drip particles are added at
//     random shadow points. Then, with W(+960) set, seven and one more particle burst is added around
//     an attachment position.
// The three particle definitions are file-local objects of the image (lavaSparkParticle 0x7d6d30,
// lavamonsterParticle2 0x7d6ce0 and lavamonsterParticle1 0x7d6c90, 80 bytes each).
// Inferred: modelGetShadowPos picks a point of the model by index; the rejection loops build random unit
// vectors. Unknown: what the states mean. Nothing here ran on the EE.
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* header);
int everyFrame__8CreatureP12_modelHeader(void* self, void* header);
int objectActivationCheck__8Creaturei(void* self, int range);
unsigned creatureAdvanceAnimation__8Creaturef(void* self, float amount);
void slideBack__8Creaturef(void* self, float amount);
void move2__8CreatureG6Point3(void* self, const void* delta);
int enemyInZone__8Creaturefs(void* self, float range, short angle);
int bExitStatePersueIdle__8Creature(void* self);
void smartWander__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
void findEnemyLeadPosition__8CreaturefP6Point3(void* self, float range, void* target);
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float amount, int a, int b, int c, int d, void* description);
unsigned eRand__10GameObject(void* self);
float icos__Fi(int angle);
float isin__Fi(int angle);
int iatan2__Fff(float y, float x);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* header, const void* point, short heading, void* animation, int index, void* position, void* other, float scale);
int modelGetNmShadowPoints__FP12_modelHeader(void* header);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* header, void* animation, const void* point, short heading, int index, void* position, float* other, void* matrix, float scale);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, void* position, void* velocity, float value);
void* blockAlloc__Fi(int size);
void objectRemoveFromRunList__FP10GameObject(void* object);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void missile_weapon_construct(void* self, int zero, void* model, void* texture, const void* position, const void* velocity, int angle,
                              void* creature, void* sound_a, void* sound_b, int a, int b, int c, int d, int e, int f, void* description)
    __asm__("__13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description");
void phprop_construct(void* self, const void* position, short heading, void* header, void* animation, void* first, void* second,
                      void* third, int count, int flags, void* description, float scale)
    __asm__("__6PhPropG6Point3sP12_modelHeaderP14AnimationStatePcN25liPQ24item11descriptionf");
extern unsigned char lavaSparkParticle[];
extern unsigned char lavamonsterParticle1[];
extern unsigned char lavamonsterParticle2[];
extern unsigned char lava_bomb_table[] __asm__("_vt$8LavaBomb");
}

namespace {
struct Point3 { float x, y, z; };
float bits(unsigned value) { return __builtin_bit_cast(float, value); }
unsigned& word(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned*>(o + offset); }
int sword(unsigned char* o, unsigned offset) { return *reinterpret_cast<int*>(o + offset); }
float& real(unsigned char* o, unsigned offset) { return *reinterpret_cast<float*>(o + offset); }
int half(unsigned char* o, unsigned offset) { return *reinterpret_cast<short*>(o + offset); }
unsigned uhalf(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned short*>(o + offset); }
unsigned char* pointer_at(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned char**>(o + offset); }

__attribute__((always_inline)) inline int signed_remainder(int value, int divisor) {
    int remainder;
    __asm__("div $0, %1, %2\n\tmfhi %0" : "=r"(remainder) : "r"(value), "r"(divisor) : "hi", "lo");
    return remainder;
}
__attribute__((always_inline)) inline float square_root(float value) {
    float result;
    __asm__("sqrt.s %0, %1" : "=f"(result) : "f"(value));
    return result;
}

void set_state(unsigned char* o, int n) {
    unsigned char* e = pointer_at(o, 72) + 184;
    reinterpret_cast<void (*)(void*, int)>(word(e, 4))(o + half(e, 0), n);
}
void* model_of(unsigned char* o) {
    unsigned char* e = pointer_at(o, 72) + 128;
    return reinterpret_cast<void* (*)(void*)>(word(e, 4))(o + half(e, 0));
}
unsigned char* target_object(unsigned char* o) { return pointer_at(pointer_at(o, 576), 12); }
bool in_zone(unsigned char* o, float range, short angle) { return enemyInZone__8Creaturefs(o, range, angle) != 0; }
void face_target(unsigned char* o) {
    unsigned char* t = target_object(o);
    const int angle = iatan2__Fff(real(t, 20) - real(o, 20), real(t, 16) - real(o, 16));
    *reinterpret_cast<unsigned short*>(o + 56) = static_cast<unsigned short>(angle);
}
// A random vector inside the unit ball by rejection: each part is (float)(int)eRand / 65536 * 2 - 1.
void unit_random(unsigned char* o, float* v) {
    const float scale = bits(0x37800000);
    const float one = bits(0x3f800000);
    do {
        v[0] = static_cast<float>(static_cast<int>(eRand__10GameObject(o))) * scale;
        v[0] = v[0] + v[0] - one;
        v[1] = static_cast<float>(static_cast<int>(eRand__10GameObject(o))) * scale;
        v[1] = v[1] + v[1] - one;
        v[2] = static_cast<float>(static_cast<int>(eRand__10GameObject(o))) * scale;
        v[2] = v[2] + v[2] - one;
    } while (one < v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}
// A random model point: shadow position at index eRand % count.
void random_shadow_point(unsigned char* o, int count, Point3* position) {
    const int index = signed_remainder(static_cast<int>(eRand__10GameObject(o)), count);
    const Point3 here = *reinterpret_cast<const Point3*>(o + 16);
    float other[3];
    modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
        model_of(o), o + 288, &here, static_cast<short>(half(o, 56)), index, position, other, nullptr, real(o, 224));
}
// One burst particle: velocity = unit vector scaled by ten, with the vertical part scaled twice.
void shadow_particle(unsigned char* o, int count, void* definition) {
    Point3 position;
    random_shadow_point(o, count, &position);
    float v[3];
    unit_random(o, v);
    const float ten = bits(0x41200000);
    const float vz = v[2] * ten;
    const float vx = v[0] * ten;
    const float vy = v[1] * ten;
    v[0] = vx;
    v[1] = vy;
    v[2] = vz * ten;
    P_AddParticle__FP11ParticleDefP6Point3T1f(definition, &position, v, 0.0f);
}

void final_particles(unsigned char* o) {
    const float twenty_five = bits(0x41c80000);
    Point3 anchor;
    anchor.x = real(o, 16) + icos__Fi(half(o, 56)) * twenty_five;
    anchor.y = real(o, 20) + isin__Fi(half(o, 56)) * twenty_five;
    anchor.z = real(o, 24) + bits(0x42a00000);
    {
        const Point3 here = *reinterpret_cast<const Point3*>(o + 16);
        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
            model_of(o), &here, static_cast<short>(half(o, 56)), o + 288, 0, &anchor, nullptr, 1.0f);
    }
    for (int pass = 0; pass < 2; ++pass) {
        const int count = pass == 0 ? 7 : 1;
        for (int i = 0; i < count; ++i) {
            float spread[3];
            unit_random(o, spread);
            const float scale = real(o, 28);
            Point3 position;
            position.x = spread[0] * scale + anchor.x;
            position.y = spread[1] * scale + anchor.y;
            position.z = spread[2] * scale + anchor.z;
            float v[3];
            unit_random(o, v);
            const float ten = bits(0x41200000);
            const float vx = v[0] * ten;
            const float vy = v[1] * ten;
            const float vz = v[2] * ten;
            v[0] = vx;
            v[1] = vy;
            v[2] = vz * ten;
            P_AddParticle__FP11ParticleDefP6Point3T1f(pass == 0 ? lavamonsterParticle1 : lavamonsterParticle2, &position, v, 0.0f);
        }
    }
}

void post_case(unsigned char* o) {
    const int state = half(o, 572);
    if (state != 10 && (word(o, 808) & 3u) == 0u) {
        const int count = modelGetNmShadowPoints__FP12_modelHeader(model_of(o));
        for (int i = 0; i < 6; ++i) shadow_particle(o, count, lavamonsterParticle2);
    }
    if (word(o, 960) == 0u) return;
    final_particles(o);
}

void tail_bit3(unsigned char* o) {
    if ((uhalf(o, 294) & 8u) == 0u) set_state(o, 3);
}
}

extern "C" void msg_run__11LavaMonster(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned long long& flags = *reinterpret_cast<unsigned long long*>(o + 160);
    if ((flags & 0x800ull) == 0ull) newDamageHandler__8CreatureP12_modelHeader(self, model_of(o));
    if (everyFrame__8CreatureP12_modelHeader(self, model_of(o)) != 0) return;
    if (objectActivationCheck__8Creaturei(self, 600) == 0) return;
    const unsigned R = creatureAdvanceAnimation__8Creaturef(self, bits(0x3f800000));
    slideBack__8Creaturef(self, bits(0x3e4ccccc));
    {
        unsigned char* e = pointer_at(o, 72) + 168;
        reinterpret_cast<void (*)(void*)>(word(e, 4))(o + half(e, 0));
    }
    {
        const float zero_vector[3] = {0.0f, 0.0f, 0.0f};
        move2__8CreatureG6Point3(self, zero_vector);
    }
    const int state = half(o, 572);
    if (static_cast<unsigned>(state) < 16u) {
        switch (state) {
        case 13:
            if (word(o, 576) != 0u) face_target(o);
            if (in_zone(o, bits(0x43960000), 32767)) set_state(o, 0);
            else if (in_zone(o, bits(0x43c80000), 16000)) set_state(o, 0);
            break;
        case 0:
            flags &= ~0x800ull;
            if (sword(o, 1048) != 1) {
                if ((uhalf(o, 294) & 0x20u) != 0u) {
                    if (word(o, 576) != 0u) face_target(o);
                } else if (in_zone(o, bits(0x43c80000), 32767)) {
                    set_state(o, 3);
                } else if (in_zone(o, bits(0x43fa0000), 16000)) {
                    set_state(o, 3);
                } else if (sword(o, 256) < sword(o, 808)) {
                    set_state(o, 1);
                }
            }
            break;
        case 1:
            if (in_zone(o, bits(0x43960000), 32767)) set_state(o, 0);
            else if (in_zone(o, bits(0x43c80000), 16000)) set_state(o, 0);
            else smartWander__8Creature(self);
            break;
        case 3:
            if (word(o, 576) == 0u) {
                set_state(o, 0);
                break;
            }
            if (in_zone(o, real(o, 28) + bits(0x42200000), 6000)) {
                set_state(o, 6);
                break;
            }
            if (in_zone(o, bits(0x43160000), 2000)) {
                set_state(o, 14);
                break;
            }
            if (static_cast<float>(sword(o, 964)) < real(o, 560) && static_cast<signed char>(o[568]) != 0) {
                if (in_zone(o, bits(0x43c80000), 8000)) set_state(o, 15);
                else steerAndRouteToEnemy__8Creature(self);
                break;
            }
            if (in_zone(o, real(o, 28) + bits(0x41a00000), 32767)) turnToFaceEnemy__8Creature(self);
            else steerAndRouteToEnemy__8Creature(self);
            break;
        case 5:
            if (in_zone(o, real(o, 28) + bits(0x42200000), 6000)) set_state(o, 6);
            else if (in_zone(o, bits(0x43c80000), 8000)) set_state(o, 15);
            else if (bExitStatePersueIdle__8Creature(self) != 0) set_state(o, 3);
            break;
        case 6:
        case 14:
            if (word(o, 576) != 0u) {
                if ((R & 1u) != 0u) {
                    if (state == 6) {
                        produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, bits(0x42100000), half(o, 200), half(o, 202), half(o, 204), 1, o + 976);
                    } else {
                        const unsigned flags_value = eRand__10GameObject(self) % 3u == 0u ? 0x80001u : 0x40001u;
                        produceMeleeDamage__8CreaturefiiiiPQ24item11description(self, bits(0x42700000), half(o, 200), half(o, 202), half(o, 204),
                                                                                static_cast<int>(flags_value), o + 976);
                    }
                }
                if ((uhalf(o, 294) & 8u) == 0u) {
                    set_state(o, 3);
                    break;
                }
                turnToFaceEnemy__8Creature(self);
            }
            tail_bit3(o);
            break;
        case 8:
            if ((uhalf(o, 294) & 0x10u) == 0u) set_state(o, 3);
            break;
        case 9:
            if (sword(o, 804) < sword(o, 808)) set_state(o, 3);
            break;
        case 15: {
            if (word(o, 576) == 0u) {
                tail_bit3(o);
                break;
            }
            const float c = icos__Fi(half(o, 56));
            const float s = isin__Fi(half(o, 56));
            const float three_hundred = bits(0x43960000);
            const float ahead_x = c * three_hundred;
            const float ahead_y = s * three_hundred;
            Point3 target;
            target.x = real(o, 16) + ahead_x;
            target.y = real(o, 20) + ahead_y;
            target.z = real(o, 24);
            findEnemyLeadPosition__8CreaturefP6Point3(self, bits(0x43c80000), &target);
            if ((R & 1u) != 0u) {
                word(o, 960) = 1u;
            } else if ((R & 2u) != 0u) {
                word(o, 960) = 0u;
                const float twenty_five = bits(0x41c80000);
                Point3 p;
                p.x = real(o, 16) + icos__Fi(half(o, 56)) * twenty_five;
                p.y = real(o, 20) + isin__Fi(half(o, 56)) * twenty_five;
                p.z = real(o, 24) + bits(0x42a00000);
                void* header = model_of(o);
                {
                    const Point3 here = *reinterpret_cast<const Point3*>(o + 16);
                    modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
                        header, &here, static_cast<short>(half(o, 56)), o + 288, 0, &p, nullptr, 1.0f);
                }
                const int toward = iatan2__Fff(target.x - p.x, target.y - p.y);
                short aim = static_cast<short>(16384 - toward);
                const short delta = static_cast<short>(aim - uhalf(o, 56));
                if (delta >= 4001) aim = static_cast<short>(uhalf(o, 56) + 4000);
                else if (delta < -4000) aim = static_cast<short>(uhalf(o, 56) - 4000);
                unsigned char* t = target_object(o);
                if (word(t, 12) == 64u) {
                    const float a = real(t, 816);
                    const float b = real(t, 820);
                    const int reach = static_cast<int>(square_root(a * a + b * b) * bits(0x41a00000) + bits(0x3f800000));
                    const int remainder = signed_remainder(static_cast<int>(eRand__10GameObject(self)), reach * 2);
                    aim = static_cast<short>(aim + (remainder - reach));
                } else {
                    aim = static_cast<short>(aim + static_cast<int>(eRand__10GameObject(self) % 2000u) - 1000);
                }
                const float four_hundred = bits(0x43c80000);
                Point3 velocity;
                velocity.x = icos__Fi(aim) * four_hundred;
                velocity.y = isin__Fi(aim) * four_hundred;
                velocity.z = 0.0f;
                const float dx = real(o, 16) - target.x;
                const float dy = real(o, 20) - target.y;
                const float dz = real(o, 24) - target.z;
                const float distance = square_root(dx * dx + dy * dy + dz * dz) * bits(0x3b23d70a);
                velocity.z = (static_cast<float>(static_cast<int>(eRand__10GameObject(self) % 40u)) + bits(0x42a00000)) * distance;
                unsigned char* made = static_cast<unsigned char*>(blockAlloc__Fi(460));
                if (made != nullptr) {
                    unsigned char* block = pointer_at(o, 36);
                    const Point3 position = p;
                    const Point3 shot = velocity;
                    const unsigned extra = word(block, 252);
                    missile_weapon_construct(made, 0, reinterpret_cast<void*>(word(block, 228)), reinterpret_cast<void*>(word(block, 236)), &position,
                                             &shot, half(o, 56) - 16384, self, nullptr, nullptr, half(o, 200) * 2, half(o, 202) * 2, 0, 0,
                                             half(o, 204), 1, nullptr);
                    word(made, 456) = extra;
                    word(made, 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(lava_bomb_table));
                }
                soundPlaySequence__FP15AnimationHeaderP10GameObjectf(reinterpret_cast<void*>(word(pointer_at(o, 36), 244)), self, 1.0f);
                word(o, 256) = eRand__10GameObject(self) % 30u + 30u;
            }
            if ((uhalf(o, 294) & 8u) != 0u) {
                unsigned char* e = pointer_at(o, 72) + 176;
                reinterpret_cast<void (*)(void*, int, void*)>(word(e, 4))(o + half(e, 0), 0, &target);
                if ((uhalf(o, 294) & 8u) != 0u) break;
            }
            set_state(o, 3);
            break;
        }
        case 10: {
            const int stage = sword(o, 1052);
            o[40] = 80;
            if (stage == 0) {
                const float level = real(o, 968);
                if (!(bits(0x3e999999) < level)) {
                    objectRemoveFromRunList__FP10GameObject(self);
                    break;
                }
                real(o, 968) = level - bits(0x3ba3d70a);
                if ((word(o, 808) & 3u) != 0u) break;
                const float limit = real(o, 968) * bits(0x40c00000);
                int count = 0;
                const int points = modelGetNmShadowPoints__FP12_modelHeader(model_of(o));
                if (!(static_cast<float>(count) < limit)) break;
                do {
                    shadow_particle(o, points, lavamonsterParticle2);
                    ++count;
                } while (static_cast<float>(count) < limit);
                break;
            }
            if ((R & 4u) == 0u) break;
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(reinterpret_cast<void*>(word(pointer_at(o, 36), 252)), self, 1.0f);
            const int points = modelGetNmShadowPoints__FP12_modelHeader(model_of(o));
            for (int i = 0; i < points; ++i) {
                Point3 position;
                random_shadow_point(o, points, &position);
                float v[3];
                unit_random(o, v);
                const float scale = bits(0x41f2e147);
                const float vx = v[0] * scale;
                const float vy = v[1] * scale;
                const float vz = v[2] * scale;
                v[0] = vx;
                v[1] = vy;
                v[2] = vz + bits(0x414a6666);
                P_AddParticle__FP11ParticleDefP6Point3T1f(lavaSparkParticle, &position, v, 0.0f);
            }
            for (int bit = 5; bit < 20; ++bit) {
                void* made = blockAlloc__Fi(584);
                if (made == nullptr) continue;
                const Point3 here = *reinterpret_cast<const Point3*>(o + 16);
                unsigned char* block = pointer_at(o, 36);
                phprop_construct(made, &here, static_cast<short>(half(o, 56)), reinterpret_cast<void*>(word(block, 12)), o + 288,
                                 reinterpret_cast<void*>(word(block, 0)), reinterpret_cast<void*>(word(block, 8)),
                                 reinterpret_cast<void*>(word(block, 16)), static_cast<int>(1u << bit), 256, nullptr, bits(0x3f800000));
            }
            word(o, 12) |= 0x4002u;
            flags |= 0x8000ull << 26;
            objectRemoveFromRunList__FP10GameObject(self);
            break;
        }
        default:
            break;
        }
    }
    post_case(o);
}
