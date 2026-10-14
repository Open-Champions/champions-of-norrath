// Observed msg_run__10BlackWidow at 0x00209fa0, 3244 bytes. Offsets are into the object. M is the
// word at +12 of the structure pointed to by the 32-bit pointer at +36 (the model, from the callees'
// declared parameter types); W is the Web at +976; F the 64-bit value at +160; h the signed
// halfword at +56; y the float at +28; S the signed halfword at +572; A the result of
// creatureAdvanceAnimation; H the head node word at +576 (a list head; node+12 is an object
// pointer whose +12 is a flags word and +756 a float). setState(n) is the object's virtual
// function at vtable byte offset 184 called with n. Creature, Web and other names below are the
// callees' own.
//   Entry: when bit 24 of F is set, the float at +24 gets the float at +972 added.
//   newDamageHandler(object, M); when everyFrame(object, M) is nonzero, return with no more
//   changes. When objectActivationCheck(object, 600) is zero, return. A = creatureAdvanceAnimation(
//   object, 1.0); slideBack(object, 0.5); the virtual function at vtable offset 168 (no arguments);
//   W.run(point (x, y, z + 10), word +808); W.draw(point (x, y, z + 10)).
//   S above 16 (unsigned) and 2, 4, 7, 11, 12 do nothing more; the others:
//   0: enemyInZone(300, 32767) or enemyInZone(300, 18000): setState(3); else when the word at +256
//      is below the word at +808 (signed): setState(1).
//   1: the same two tests: setState(3); else smartWander.
//   3: when H is zero: setState(0). Otherwise enemyInZone(y + 30, 2000): a roll (eRand % 6) picks
//      15 (zero) or 6; else, when the object O2 at H+12 has bit 5 of its +12 word set, its float
//      at +756 is zero, the word at +960 is below the word at +808 (signed) and enemyInZone(500,
//      3000): setState(13); otherwise enemyInZone(y + 30, 32767) then turnToFaceEnemy, else
//      steerAndRouteToEnemy.
//   5: the same first test and roll as 3; then, with the O2 conditions of 3 (H nonzero, bit 5 of
//      O2's +12, float zero, +960 below +808, enemyInZone(500, 3000)): setState(13); finally
//      bExitStatePersueIdle nonzero: setState(3).
//   6: when H is nonzero: bit 1 of A: produceMeleeDamage(36.0, halfwords +200, +202, +204, 4,
//      null); bit 3 of the halfword at +294: turnToFaceEnemy. Then, when bit 3 of the halfword at
//      +294 is clear the word at +256 is decremented; when that word is not positive: setState(3).
//   15: when H is nonzero and the word at +964 is nonzero: V = (cos h, sin h, 0) * the float at +176
//      scaled by (60 - signed byte +800) / 60 when the byte +800 is 1..60; move2(object, V); when the
//      result has bit 1 set the word at +964 is cleared. When H is nonzero and bit 1 of A is set: the
//      word at +964 is cleared, a local damage descriptor is built (first of four 8-byte slots whose
//      id halfword is -1 or 23 gets id 23 and the halfwords +200 and +202 and zero) and
//      produceMeleeDamage(36.0, +200, +202, +204, 0x10004, descriptor). Then the tail of 6.
//   13: when H is nonzero, bit 5 of O2's +12 and bit 0 of A: W.start(point (x, y, z + 10), O2, 300.0,
//      word +808). Then when W is not active: setState(3); else when the word at +988 is nonzero:
//      setState(14).
//   14: when W is not active: setState(3). When H is nonzero: enemyInZone(y + 30, 2000) true ->
//      W.breakWeb, setState(15); then turnToFaceEnemy.
//   9: setState(0) when the word at +804 is below the word at +808 (signed).
//   8: setState(3) unless bit 4 of the halfword at +294 is set.
//   10: bit 2 of A and bit 1 of the word at +12 clear: floorAdd(copy of the +16 point, (short)
//      one roll, (50 + a second roll % 10) as a float, 6, 1). Then, when bit 24 of F is set: with
//      the word at +964 zero, bit 7 of the halfword at +294 sets +964 to 1 and +968 to the word
//      at +808; with +964 nonzero, loseBodyPart(object, k) is called for each k in 0..7 for which
//      the word at +968 equals the word at +808 - 10 * (k + 1), three particle bursts are added when
//      (+808 - +968) is above 80 (each: two random vectors inside the unit ball by rejection, the
//      first scaled by 20.75 with z * 0.75 + 0.07, offset by the object position, the second by
//      4.75 with z zero, P_AddParticle(ectopileParticle1, ...)), then +972 grows by 0.15 and the
//      float at +60 becomes z - the new +972; and objectAddToDeleteList when (+808 - +968) is at
//      least 241. With bit 24 of F clear, bit 7 of the halfword at +294 calls
//      objectRemoveFromRunList.
//   16: when bit 0 of A is set, bit 21 of F is cleared; when bit 7 of the halfword at +294 is
//      set: bit 0 of the word at +12 and bit 11 of F are cleared and setState(0).
//   Exit (all paths that did not return early): when bit 24 of F is set, the float at +24 loses
//   the float at +972.
// Unknown: what the zone tests, A's bits, the web, the list nodes and the numeric states mean.
// The host check defines NORRATH_W10AC_HOST so that 32-bit address words map to host memory.
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void* self, void* model);
int everyFrame__8CreatureP12_modelHeader(void* self, void* model);
int objectActivationCheck__8Creaturei(void* self, int range);
int creatureAdvanceAnimation__8Creaturef(void* self, float step);
void slideBack__8Creaturef(void* self, float amount);
void run__3WebG6Point3i(void* web, const void* position, int value);
void draw__3WebG6Point3(void* web, const void* position);
void start__3WebG6Point3P8Creaturefi(void* web, const void* position, void* creature, float range, int value);
int bActive__3Web(void* web);
void breakWeb__3Web(void* web);
int move2__8CreatureG6Point3(void* self, const void* movement);
void smartWander__8Creature(void* self);
void turnToFaceEnemy__8Creature(void* self);
void steerAndRouteToEnemy__8Creature(void* self);
int bExitStatePersueIdle__8Creature(void* self);
int enemyInZone__8Creaturefs(void* self, float range, short zone);
int produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float amount, int a, int b, int c, int flags,
                                                              void* description);
void loseBodyPart__10BlackWidowi(void* self, int part);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, void* position, void* velocity, float value);
void objectAddToDeleteList__FP10GameObject(void* object);
void objectRemoveFromRunList__FP10GameObject(void* object);
void floorAdd__FG6Point3sfii(const void* position, short angle, float size, int a, int b);
float icos__Fi(int angle);
float isin__Fi(int angle);
int eRand__10GameObject(void* object);
extern unsigned char ectopileParticle1[];
#if defined(NORRATH_W10AC_HOST)
extern unsigned char* norrath_w10ac_host_base;
void* norrath_w10ac_host_function(unsigned word);
#endif
}

namespace {
typedef unsigned long long DWord __attribute__((may_alias));
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
short read_half(const unsigned char* p) { return static_cast<short>(p[0] | (p[1] << 8)); }
void write_half(unsigned char* p, int value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
}
unsigned read_uhalf(const unsigned char* p) { return static_cast<unsigned>(p[0] | (p[1] << 8)); }
float read_float(const unsigned char* p) {
    union { unsigned u; float f; } v;
    v.u = read_word(p);
    return v.f;
}
void write_float(unsigned char* p, float value) {
    union { unsigned u; float f; } v;
    v.f = value;
    write_word(p, v.u);
}
#if defined(NORRATH_W10AC_HOST)
// Host check only: pointer words are offsets from a base chosen by the check.
unsigned char* address_of(unsigned word) { return norrath_w10ac_host_base + word; }
void* function_of(unsigned word) { return norrath_w10ac_host_function(word); }
#else
unsigned char* address_of(unsigned word) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
}
void* function_of(unsigned word) { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word)); }
#endif
void copy_bytes(unsigned char* dest, const unsigned char* src, unsigned count) {
    for (unsigned i = 0; i < count; ++i) dest[i] = src[i];
}
const unsigned char* slot_entry(unsigned char* target, unsigned byte_offset) {
    return address_of(read_word(target + 72u)) + byte_offset;
}
void call_slot(unsigned char* target, unsigned byte_offset) {
    const unsigned char* entry = slot_entry(target, byte_offset);
    typedef void (*Function)(void*);
    reinterpret_cast<Function>(function_of(read_word(entry + 4)))(target + read_half(entry));
}
void set_state(unsigned char* target, int state) {
    const unsigned char* entry = slot_entry(target, 184);
    typedef void (*Function)(void*, int);
    reinterpret_cast<Function>(function_of(read_word(entry + 4)))(target + read_half(entry), state);
}
__attribute__((always_inline)) inline float bits_float(unsigned bits) {
    union { unsigned u; float f; } v;
    v.u = bits;
    return v.f;
}
// One random coordinate in [-1, 1): the roll as a float times 2^-16, doubled, minus one.
float random_coordinate(unsigned char* object) {
    float value = static_cast<float>(eRand__10GameObject(object));
    value = value * bits_float(0x37800000u);
    value = value + value;
    return value - 1.0f;
}
// Fills three floats with a point inside the unit ball by rejection sampling.
void random_in_ball(unsigned char* object, float* v) {
    do {
        v[0] = random_coordinate(object);
        v[1] = random_coordinate(object);
        v[2] = random_coordinate(object);
    } while (1.0f < v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}
// The tail shared by states 6 and 15: face the enemy and count the word at +256 down.
bool countdown_expired(unsigned char* object, bool face_enemy) {
    if (face_enemy && (read_uhalf(object + 294u) & 8u) != 0u) turnToFaceEnemy__8Creature(object);
    if ((read_uhalf(object + 294u) & 8u) == 0u) write_word(object + 256u, read_word(object + 256u) - 1u);
    return static_cast<int>(read_word(object + 256u)) <= 0;
}
}

extern "C" void msg_run__10BlackWidow(void* object_storage) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    DWord* flags = reinterpret_cast<DWord*>(object + 160);
    if ((*flags & 0x01000000ull) != 0ull)
        write_float(object + 24u, read_float(object + 24u) + read_float(object + 972u));
    newDamageHandler__8CreatureP12_modelHeader(object, address_of(read_word(address_of(read_word(object + 36u)) + 12u)));
    if (everyFrame__8CreatureP12_modelHeader(object, address_of(read_word(address_of(read_word(object + 36u)) + 12u))) != 0)
        return;
    if (objectActivationCheck__8Creaturei(object, 600) == 0) return;
    unsigned char* web = object + 976;
    int advance = creatureAdvanceAnimation__8Creaturef(object, 1.0f);
    slideBack__8Creaturef(object, 0.5f);
    call_slot(object, 168);
    float point[3];
    point[0] = read_float(object + 16u);
    point[1] = read_float(object + 20u);
    point[2] = read_float(object + 24u) + 10.0f;
    run__3WebG6Point3i(web, point, static_cast<int>(read_word(object + 808u)));
    float second_point[3];
    second_point[2] = read_float(object + 24u) + 10.0f;
    second_point[0] = read_float(object + 16u);
    second_point[1] = read_float(object + 20u);
    draw__3WebG6Point3(web, second_point);
    short state = read_half(object + 572u);
    if (static_cast<unsigned>(static_cast<int>(state)) < 17u) {
        switch (state) {
        case 16:
            if ((advance & 1) != 0) *flags &= 0xffffffffffdfffffull;
            if ((read_uhalf(object + 294u) & 0x80u) != 0u) {
                write_word(object + 12u, read_word(object + 12u) & 0xfffffffeu);
                *flags &= 0xfffffffffffff7ffull;
                set_state(object, 0);
            }
            break;
        case 0:
            if (enemyInZone__8Creaturefs(object, 300.0f, 32767) || enemyInZone__8Creaturefs(object, 300.0f, 18000))
                set_state(object, 3);
            else if (static_cast<int>(read_word(object + 256u)) < static_cast<int>(read_word(object + 808u)))
                set_state(object, 1);
            break;
        case 1:
            if (enemyInZone__8Creaturefs(object, 300.0f, 32767) || enemyInZone__8Creaturefs(object, 300.0f, 18000))
                set_state(object, 3);
            else
                smartWander__8Creature(object);
            break;
        case 3:
        case 5: {
            if (state == 3) {
                if (read_word(object + 576u) == 0u) {
                    set_state(object, 0);
                    break;
                }
            }
            if (enemyInZone__8Creaturefs(object, read_float(object + 28u) + 30.0f, 2000)) {
                set_state(object, (static_cast<unsigned>(eRand__10GameObject(object)) % 6u) == 0u ? 15 : 6);
                break;
            }
            unsigned head = read_word(object + 576u);
            bool web_ready = false;
            if (head != 0u) {
                unsigned char* other = address_of(read_word(address_of(head) + 12u));
                web_ready = (read_word(other + 12u) & 0x20u) != 0u && read_float(other + 756u) == 0.0f &&
                            static_cast<int>(read_word(object + 960u)) < static_cast<int>(read_word(object + 808u)) &&
                            enemyInZone__8Creaturefs(object, 500.0f, 3000) != 0;
            }
            if (web_ready) {
                set_state(object, 13);
                break;
            }
            if (state == 3) {
                if (enemyInZone__8Creaturefs(object, read_float(object + 28u) + 30.0f, 32767))
                    turnToFaceEnemy__8Creature(object);
                else
                    steerAndRouteToEnemy__8Creature(object);
            } else if (bExitStatePersueIdle__8Creature(object) != 0) {
                set_state(object, 3);
            }
            break;
        }
        case 6:
            if (read_word(object + 576u) != 0u) {
                if ((advance & 2) != 0)
                    produceMeleeDamage__8CreaturefiiiiPQ24item11description(
                        object, 36.0f, read_half(object + 200u), read_half(object + 202u), read_half(object + 204u), 4,
                        nullptr);
                if (countdown_expired(object, true)) set_state(object, 3);
            } else if (countdown_expired(object, false)) {
                set_state(object, 3);
            }
            break;
        case 15: {
            if (read_word(object + 576u) != 0u) {
                if (read_word(object + 964u) != 0u) {
                    unsigned char count = object[800];
                    float scale = read_float(object + 176u);
                    if (static_cast<unsigned>(count) - 1u < 60u) {
                        int remaining = 60 - static_cast<signed char>(count);
                        float fraction = static_cast<float>(remaining) * bits_float(0x3c888888u);
                        scale = scale * fraction;
                    }
                    float cosine = icos__Fi(read_half(object + 56u));
                    float along_x = cosine * scale;
                    float sine = isin__Fi(read_half(object + 56u));
                    float movement[3];
                    movement[0] = along_x;
                    movement[1] = sine * scale;
                    movement[2] = 0.0f;
                    if ((move2__8CreatureG6Point3(object, movement) & 2) != 0) write_word(object + 964u, 0u);
                }
                if ((advance & 2) != 0) {
                    write_word(object + 964u, 0u);
                    unsigned char description[64];
                    for (int slot = 0; slot < 4; ++slot) write_half(description + 36 + slot * 8, -1);
                    description[32] = 0xff;
                    description[33] = 0xff;
                    for (int index = 16; index < 32; index += 2) write_half(description + index, 0);
                    for (int slot = 0; slot < 4; ++slot) {
                        unsigned char* entry = description + 36 + slot * 8;
                        int id = read_half(entry);
                        if (id == -1 || id == 23) {
                            write_half(entry, 23);
                            write_half(entry + 2, read_half(object + 200u));
                            write_half(entry + 4, read_half(object + 202u));
                            write_half(entry + 6, 0);
                            break;
                        }
                    }
                    produceMeleeDamage__8CreaturefiiiiPQ24item11description(
                        object, 36.0f, read_half(object + 200u), read_half(object + 202u), read_half(object + 204u),
                        0x10004, description);
                }
                if (countdown_expired(object, true)) set_state(object, 3);
            } else if (countdown_expired(object, false)) {
                set_state(object, 3);
            }
            break;
        }
        case 13: {
            unsigned head = read_word(object + 576u);
            if (head != 0u) {
                unsigned char* other = address_of(read_word(address_of(head) + 12u));
                if ((read_word(other + 12u) & 0x20u) != 0u && (advance & 1) != 0) {
                    float start_point[3];
                    start_point[0] = read_float(object + 16u);
                    start_point[1] = read_float(object + 20u);
                    start_point[2] = read_float(object + 24u) + 10.0f;
                    start__3WebG6Point3P8Creaturefi(web, start_point, other, 300.0f,
                                                    static_cast<int>(read_word(object + 808u)));
                }
            }
            if (bActive__3Web(web) == 0) {
                set_state(object, 3);
            } else if (read_word(object + 988u) != 0u) {
                set_state(object, 14);
            }
            break;
        }
        case 14:
            if (bActive__3Web(web) == 0) set_state(object, 3);
            if (read_word(object + 576u) != 0u) {
                if (enemyInZone__8Creaturefs(object, read_float(object + 28u) + 30.0f, 2000)) {
                    breakWeb__3Web(web);
                    set_state(object, 15);
                }
                turnToFaceEnemy__8Creature(object);
            }
            break;
        case 9:
            if (static_cast<int>(read_word(object + 804u)) < static_cast<int>(read_word(object + 808u)))
                set_state(object, 0);
            break;
        case 8:
            if ((read_uhalf(object + 294u) & 0x10u) == 0u) set_state(object, 3);
            break;
        case 10: {
            if ((advance & 4) != 0 && (read_word(object + 12u) & 2u) == 0u) {
                int first = eRand__10GameObject(object);
                int second = eRand__10GameObject(object);
                unsigned char position[12];
                copy_bytes(position, object + 16u, 12u);
                floorAdd__FG6Point3sfii(position, static_cast<short>(first),
                                        static_cast<float>(static_cast<int>(static_cast<unsigned>(second) % 10u) + 50), 6, 1);
            }
            if ((*flags & 0x01000000ull) != 0ull) {
                if (read_word(object + 964u) == 0u) {
                    if ((read_uhalf(object + 294u) & 0x80u) != 0u) {
                        write_word(object + 964u, 1u);
                        write_word(object + 968u, read_word(object + 808u));
                    }
                } else {
                    for (int part = 0; part < 8; ++part)
                        if (read_word(object + 968u) == read_word(object + 808u) - 10u * static_cast<unsigned>(part + 1))
                            loseBodyPart__10BlackWidowi(object, part);
                    if (static_cast<int>(read_word(object + 808u) - read_word(object + 968u)) > 80) {
                        for (int burst = 0; burst < 3; ++burst) {
                            float position[3];
                            float velocity[3];
                            random_in_ball(object, position);
                            position[0] = position[0] * 20.75f;
                            position[1] = position[1] * 20.75f;
                            position[2] = (position[2] * 20.75f) * 0.75f + bits_float(0x3d8f5c28u);
                            random_in_ball(object, velocity);
                            velocity[0] = velocity[0] * 4.75f;
                            velocity[1] = velocity[1] * 4.75f;
                            velocity[2] = 0.0f;
                            position[0] = position[0] + read_float(object + 16u);
                            position[1] = position[1] + read_float(object + 20u);
                            position[2] = position[2] + read_float(object + 24u);
                            P_AddParticle__FP11ParticleDefP6Point3T1f(ectopileParticle1, position, velocity, 0.0f);
                        }
                        float grown = read_float(object + 972u) + bits_float(0x3e199999u);
                        float lowered = read_float(object + 24u) - grown;
                        write_float(object + 972u, grown);
                        write_float(object + 60u, lowered);
                    }
                    if (static_cast<int>(read_word(object + 808u) - read_word(object + 968u)) >= 241)
                        objectAddToDeleteList__FP10GameObject(object);
                }
            } else if ((read_uhalf(object + 294u) & 0x80u) != 0u) {
                objectRemoveFromRunList__FP10GameObject(object);
            }
            break;
        }
        default:
            break;
        }
    }
    if ((*flags & 0x01000000ull) != 0ull)
        write_float(object + 24u, read_float(object + 24u) - read_float(object + 972u));
}
