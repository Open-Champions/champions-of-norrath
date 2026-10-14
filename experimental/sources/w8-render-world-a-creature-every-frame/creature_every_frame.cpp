// Observed everyFrame__8CreatureP12_modelHeader at 0x0010ebf8, 4136 bytes. The
// object is a Creature, the second argument its model (used only for the fire and
// shock effects). Offsets are from the object address. The result is always 0.
// Struct-by-value Point3 parameters are passed by address to a caller-made copy in
// this image, so the declarations take pointers. "Flags" are the doublewords at
// +160 and the word at +12.
//  1. Unless bit 0x40 of the word at +12: virtual entry at vtable (+72) + 136 is
//     called with the point (x, y, z + f32 * 0.5).
//  2. The eight halfwords at +148..+155 and +140..+147 move two toward zero, never
//     past it.
//  3. With bit 0x08000000 of the flags: +788 = 50.0, and the tag "PathName" names a
//     line. The path state is set up for it; when the object is within +788 (xy
//     distance) of the next vertex the target becomes the vertex after it, the
//     point at +776 and +272 follow, and +80 = +808 + 600, byte +84 = the
//     vertex byte 60; otherwise the point is that next vertex, and when +84 matches
//     the vertex byte and +80 < +808 the object moves there with
//     objectUpdateInGrid.
//  4. Halfwords +796 and +794 count down to zero. While +808 < +804 and
//     (frameNm & 3) == 0, effect 16 is added at (x, y, z + f32); with +796 nonzero
//     and the same frame test, effect 18.
//  5. Halfword +792, when nonzero, counts down (not from -1); at zero bits 0x00c00000
//     of the flags clear. While either bit is set the object tracks player index
//     (signed byte +85), re-picked with getClosestLivingPlayer(pos, 500.0) when it
//     is out of range or its object has bit 8 of its word +12; +776 becomes that
//     object's position moved by 30 * (cos, sin) of its facing, added when bit
//     0x00400000 is set and subtracted otherwise. +788 = 500.0 or 100.0 on that
//     bit. With +236 zero and the level name (levelName + 81) not "kelethin", up to
//     32 directions starting from the angle to +776 and stepping 2048 are probed
//     with worldHitscan; the first hit fraction in (0, 1) whose spot passes the
//     activation test (net mode) or lies 400 (squared 160000) or more from
//     worldViewCenter becomes the object position (objectUpdateInGrid,
//     resetRouting, +272 = +776).
//  6. +808 increases by one unless the signed byte at +800 is 60.
//  7. A float at +876 above zero sets halfwords +140, +144, +148, +152 to
//     int(min(1, f) * 128) and +146 to 32 and decreases by 1/60.
//  8. Floats at +856, +864, +860, +868, +872 are timers: fire (856) runs the fire
//     effect for the local creature and every 32 frames resolves an attack;
//     shock (864) runs the shock effect and clears words +836, +844, +852 when it
//     expires; slow (860) drives the byte at +800 (with the limits 30, 15 for
//     bosses, 60 - 60 * f92), word +924 sets it from the elapsed frames since +252,
//     and +868 and +872 resolve attacks every 32 frames with randomized damage in
//     halfword ranges +880..+882 and +884..+886.
//  9. With bit 1 of the flags and +236 nonzero, a local player within +788 sets bit
//     31, calls virtual entry (+72) + 152 whose text is copied to bossMeterName,
//     and sets bossMeterCurrent and bossMeterMax from the floats at +560 and +564;
//     when no player is in range, bit 31 is cleared and both meters are zeroed.
// 10. levelFlags bit 0x100 raises the byte at +800 to at least 20.
// 11. Last, a vertical spring: v = f64 + 3 * (f24 - f60); v -= sqrt(c1) * v;
//     f64 = v; f60 += v / 60.
// Inferred: the field names are unknown; the groups are status effects, path
// following and aggro handling of a Creature. Unknown: the meaning of the fields,
// the layout of the path state and attack description beyond the bytes the image
// writes, what a zero halfword range does (the image divides without a check).
#ifdef NORRATH_W8_HOST
extern void* norrath_w8_word_to_pointer(unsigned word);
extern void (*norrath_w8_word_to_function(unsigned word))(void*);
#define NORRATH_W8_POINTER(word) norrath_w8_word_to_pointer(word)
#define NORRATH_W8_FUNCTION(word) norrath_w8_word_to_function(word)
#else
#define NORRATH_W8_POINTER(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#define NORRATH_W8_FUNCTION(word) reinterpret_cast<void (*)(void*)>(static_cast<__UINTPTR_TYPE__>(word))
#endif

struct Point3 {
    float x;
    float y;
    float z;
};

template <class T>
static inline T& field(void* object, unsigned offset) {
    return *reinterpret_cast<T*>(static_cast<char*>(object) + offset);
}

static float float_bits(unsigned bits) {
    return __builtin_bit_cast(float, bits);
}

static float square_root(float value) {
#ifdef NORRATH_W8_HOST
    return __builtin_sqrtf(value);
#else
    float result;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(result) : "f"(value));
    return result;
#endif
}

extern "C" {
extern int frameNm;
extern int nmPlayers;
extern int levelFlags;
extern int localNmPlayersAlive;
extern int localPlayersAlive[];
extern void* world;
extern signed char netNetworkMode;
extern int bossMeterCurrent;
extern int bossMeterMax;
extern Point3 worldViewCenter;
extern unsigned char player[];
extern char bossMeterName[];
extern char levelName[];

int getClosestLivingPlayer__FG6Point3f(Point3* position, float range);
void gameLineInitPathState__FP17gameLinePathStatePcP6Point3(void* state, char* name, Point3* position);
void* gameLineGet__FPc(char* name);
void initPosition__FP17gameLinePathState(void* state);
float* gameLineGetVert__Fii(void* line, int index);
void objectUpdateInGrid__FP10GameObject(void* object);
void resetRouting__8Creature(void* object);
void addEffect__FiG6Point3(int kind, Point3* position);
int eRand__10GameObject(void* object);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b,
                                                                      int c, int d, int e, int f, void* description,
                                                                      bool g);
int iatan2__Fff(float y, float x);
float icos__Fi(int angle);
float isin__Fi(int angle);
float worldHitscan__FP12_worldHeaderG6Point3T1fi(void* world_header, Point3* start, Point3* direction, float length,
                                                 int mode);
int netCheckActivation__FG6Point3f(Point3* position, float range);
void* netGetLocalObject__FP8Creature(void* object);
int isBoss__FP10GameObject(void* object);
void fxRunCreatureFire__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStateffff(
    void* model, void* animation, Point3* position, short yaw, void* fx_state, float a, float b, float c, float d);
void fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi(
    void* model, void* animation, Point3* position, short yaw, void* fx_state, float a, float b, float c,
    unsigned color);
int strcasecmp(const char* a, const char* b);
char* strcpy(char* destination, const char* source);
char* findTagString__10GameObjectPci(void* object, const char* tag, int zero);
}

static void* player_object(int index) {
    return NORRATH_W8_POINTER(*reinterpret_cast<unsigned*>(player + index * 23984 + 21872));
}

static void resolve_attack(void* self, float a, float b, unsigned flags, void* description) {
    gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(self, self, a, b, 100000, 0, 0, static_cast<int>(flags),
                                                                      description, false);
}

extern "C" int everyFrame__8CreatureP12_modelHeader(void* self, void* model) {
    void* const animation = static_cast<char*>(self) + 288;
    if ((field<unsigned>(self, 12) & 0x40u) == 0u) {
        const float half = field<float>(self, 32) * 0.5f;
        Point3 offset = {0.0f, 0.0f, half};
        Point3 target;
        target.x = field<float>(self, 16) + offset.x;
        target.y = field<float>(self, 20);
        target.z = field<float>(self, 24) + half;
        unsigned char* const entry = static_cast<unsigned char*>(NORRATH_W8_POINTER(field<unsigned>(self, 72))) + 136;
        void (*const function)(void*, Point3*) = reinterpret_cast<void (*)(void*, Point3*)>(
            NORRATH_W8_FUNCTION(*reinterpret_cast<unsigned*>(entry + 4)));
        function(static_cast<char*>(self) + *reinterpret_cast<short*>(entry), &target);
    }

    short* const decaying[2] = {&field<short>(self, 148), &field<short>(self, 140)};
    for (int group = 0; group < 2; ++group) {
        for (int index = 0; index < 4; ++index) {
            short* const value = decaying[group] + index;
            if (*value > 0) {
                *value = static_cast<short>(*value - 2);
                if (*value < 0) *value = 0;
            } else if (*value < 0) {
                *value = static_cast<short>(*value + 2);
                if (*value > 0) *value = 0;
            }
        }
    }

    if (field<unsigned long long>(self, 160) & 0x08000000ull) {
        field<float>(self, 788) = 50.0f;
        char* const line_name = findTagString__10GameObjectPci(self, "PathName", 0);
        if (line_name != nullptr) {
            unsigned char state[48];
            gameLineInitPathState__FP17gameLinePathStatePcP6Point3(state, line_name, &field<Point3>(self, 16));
            *reinterpret_cast<void**>(state + 28) = gameLineGet__FPc(line_name);
            initPosition__FP17gameLinePathState(state);
            float* vertex = gameLineGetVert__Fii(*reinterpret_cast<void**>(state + 28), *reinterpret_cast<int*>(state + 32) + 1);
            if (vertex != nullptr) {
                const float dx = field<float>(self, 16) - vertex[0];
                const float dy = field<float>(self, 20) - vertex[1];
                const float distance = square_root(dx * dx + dy * dy);
                if (distance < field<float>(self, 788)) {
                    vertex = gameLineGetVert__Fii(*reinterpret_cast<void**>(state + 28), *reinterpret_cast<int*>(state + 32) + 2);
                    field<Point3>(self, 776) = *reinterpret_cast<Point3*>(vertex);
                    field<Point3>(self, 272) = field<Point3>(self, 776);
                    field<int>(self, 80) = field<int>(self, 808) + 600;
                    field<unsigned char>(self, 84) = reinterpret_cast<unsigned char*>(vertex)[60];
                } else {
                    field<Point3>(self, 776) = *reinterpret_cast<Point3*>(vertex);
                    field<Point3>(self, 272) = field<Point3>(self, 776);
                    unsigned char current = field<unsigned char>(self, 84);
                    unsigned char wanted = reinterpret_cast<unsigned char*>(vertex)[60];
                    if (current != wanted) {
                        field<int>(self, 80) = field<int>(self, 808) + 600;
                        current = reinterpret_cast<unsigned char*>(vertex)[60];
                        field<unsigned char>(self, 84) = current;
                        wanted = reinterpret_cast<unsigned char*>(vertex)[60];
                    }
                    if (wanted == current && field<int>(self, 80) < field<int>(self, 808)) {
                        field<Point3>(self, 16) = field<Point3>(self, 776);
                        objectUpdateInGrid__FP10GameObject(self);
                    }
                }
            }
        }
    }

    if (field<short>(self, 796) != 0) field<short>(self, 796) = static_cast<short>(field<unsigned short>(self, 796) - 1);
    if (field<short>(self, 794) != 0) field<short>(self, 794) = static_cast<short>(field<unsigned short>(self, 794) - 1);
    if (field<int>(self, 808) < field<int>(self, 804) && (frameNm & 3) == 0) {
        Point3 effect_position = {field<float>(self, 16), field<float>(self, 20), field<float>(self, 24) + field<float>(self, 32)};
        addEffect__FiG6Point3(16, &effect_position);
    }
    if (field<short>(self, 796) != 0 && (frameNm & 3) == 0) {
        Point3 effect_position = {field<float>(self, 16), field<float>(self, 20), field<float>(self, 24) + field<float>(self, 32)};
        addEffect__FiG6Point3(18, &effect_position);
    }

    short pursue = field<short>(self, 792);
    if (pursue != 0) {
        short remaining = pursue;
        if (pursue != -1) {
            remaining = static_cast<short>(pursue - 1);
            field<short>(self, 792) = remaining;
        }
        if (remaining == 0) field<unsigned long long>(self, 160) &= ~0x00c00000ull;
        if ((field<unsigned long long>(self, 160) & 0x00c00000ull) != 0ull) {
            const signed char tracked = field<signed char>(self, 85);
            void* target_object = nullptr;
            bool valid = false;
            if (tracked >= 0 && tracked < nmPlayers) {
                target_object = player_object(tracked);
                if (target_object != nullptr && (field<unsigned>(target_object, 12) & 8u) == 0u) valid = true;
            }
            if (!valid) {
                Point3 here = field<Point3>(self, 16);
                field<signed char>(self, 85) = static_cast<signed char>(getClosestLivingPlayer__FG6Point3f(&here, 500.0f));
            } else {
                field<Point3>(self, 776) = field<Point3>(target_object, 16);
                const float push_x = icos__Fi(field<short>(player_object(field<signed char>(self, 85)), 56)) * 30.0f;
                const float push_y = isin__Fi(field<short>(player_object(field<signed char>(self, 85)), 56)) * 30.0f;
                if (field<unsigned long long>(self, 160) & 0x00400000ull) {
                    field<float>(self, 776) = field<float>(self, 776) + push_x;
                    field<float>(self, 780) = field<float>(self, 780) + push_y;
                } else {
                    field<float>(self, 776) = field<float>(self, 776) - push_x;
                    field<float>(self, 780) = field<float>(self, 780) - push_y;
                }
            }
            field<float>(self, 788) = (field<unsigned long long>(self, 160) & 0x00400000ull) ? 500.0f : 100.0f;
            if (field<int>(self, 236) == 0 && strcasecmp(levelName + 81, "kelethin") != 0) {
                Point3 probe = field<Point3>(self, 776);
                const float offset_x = field<float>(self, 16) - field<float>(self, 776);
                const float offset_y = field<float>(self, 20) - field<float>(self, 780);
                probe.z = probe.z + 11.0f;
                const float offset_z = field<float>(self, 24) - field<float>(self, 784);
                float step_vector[3] = {offset_x, offset_y, offset_z};
                int angle = iatan2__Fff(offset_y, offset_x);
                for (int attempt = 0; attempt < 32; ++attempt) {
                    const int heading = static_cast<short>(angle);
                    angle += 2048;
                    const float cosine = icos__Fi(heading);
                    const float sine = isin__Fi(heading);
                    Point3 direction = {cosine, sine, 0.0f};
                    Point3 reach;
                    step_vector[0] = direction.x;
                    step_vector[1] = direction.y;
                    step_vector[2] = direction.z;
                    const float length = float_bits(0x44480000u);
                    reach.x = step_vector[0] * length;
                    reach.y = step_vector[1] * length;
                    reach.z = step_vector[2] * length;
                    const float fraction = worldHitscan__FP12_worldHeaderG6Point3T1fi(world, &probe, &reach, float_bits(0x3f999999u), 3);
                    if (0.0f < fraction && fraction < 1.0f) {
                        const float full_x = step_vector[0] * length;
                        const float full_y = step_vector[1] * length;
                        const float full_z = step_vector[2] * length;
                        Point3 candidate;
                        candidate.x = field<float>(self, 776) + full_x * fraction;
                        candidate.y = field<float>(self, 780) + full_y * fraction;
                        candidate.z = field<float>(self, 784) + full_z * fraction;
                        bool accept = false;
                        bool test_distance = true;
                        if (netNetworkMode != 0) {
                            if (netCheckActivation__FG6Point3f(&candidate, 400.0f) == 0) {
                                accept = true;
                                test_distance = false;
                            } else if (netNetworkMode != 0) {
                                test_distance = false;
                            }
                        }
                        if (test_distance) {
                            const float ddx = candidate.x - worldViewCenter.x;
                            const float ddy = candidate.y - worldViewCenter.y;
                            const float ddz = candidate.z - worldViewCenter.z;
                            if (float_bits(0x481c4000u) <= ddx * ddx + ddy * ddy + ddz * ddz) accept = true;
                        }
                        if (accept) {
                            field<Point3>(self, 16) = candidate;
                            objectUpdateInGrid__FP10GameObject(self);
                            field<Point3>(self, 272) = field<Point3>(self, 776);
                            resetRouting__8Creature(self);
                            break;
                        }
                    }
                }
            }
        }
    }

    if (field<signed char>(self, 800) != 60) field<int>(self, 808) = field<int>(self, 808) + 1;

    float glow = field<float>(self, 876);
    if (0.0f < glow) {
        const float capped = glow < 1.0f ? glow : 1.0f;
        const int level = static_cast<int>(capped * 128.0f);
        field<short>(self, 146) = 32;
        field<float>(self, 876) = glow - float_bits(0x3c888888u);
        field<short>(self, 140) = static_cast<short>(level);
        field<short>(self, 152) = static_cast<short>(level);
        field<short>(self, 148) = static_cast<short>(level);
        field<short>(self, 144) = static_cast<short>(level);
    }

    if (0.0f < field<float>(self, 856)) {
        if (netGetLocalObject__FP8Creature(self) == self && (field<unsigned>(self, 12) & 0x4000000u) == 0u) {
            Point3 here = field<Point3>(self, 16);
            fxRunCreatureFire__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStateffff(
                model, animation, &here, field<short>(self, 56), static_cast<char*>(self) + 824, field<float>(self, 856),
                field<float>(self, 224), field<float>(self, 228), static_cast<float>(field<unsigned char>(self, 40)));
        }
        field<float>(self, 856) = field<float>(self, 856) - float_bits(0x3c888888u);
        if ((frameNm & 0x1f) == 0) {
            unsigned char description[96];
            for (int slot = 0; slot < 4; ++slot) *reinterpret_cast<short*>(description + 36 + 8 * slot) = -1;
            description[33] = 0xff;
            description[32] = 0xff;
            for (int slot = 0; slot < 4; ++slot) {
                *reinterpret_cast<short*>(description + 16 + 2 * slot) = 0;
                *reinterpret_cast<short*>(description + 24 + 2 * slot) = 0;
            }
            for (int slot = 0; slot < 4; ++slot) {
                short& kind = *reinterpret_cast<short*>(description + 36 + 8 * slot);
                if (kind == -1 || kind == 21) {
                    kind = 21;
                    *reinterpret_cast<short*>(description + 42 + 8 * slot) = 0;
                    *reinterpret_cast<short*>(description + 38 + 8 * slot) = 1;
                    *reinterpret_cast<short*>(description + 40 + 8 * slot) = 1;
                    break;
                }
            }
            resolve_attack(self, 0.0f, 0.0f, 0x1180c040u, description);
        }
    }

    if (0.0f < field<float>(self, 864)) {
        if (netGetLocalObject__FP8Creature(self) == self && (field<unsigned>(self, 12) & 0x4000000u) == 0u) {
            Point3 here = field<Point3>(self, 16);
            fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi(
                model, animation, &here, field<short>(self, 56), static_cast<char*>(self) + 824, field<float>(self, 864),
                field<float>(self, 224), field<float>(self, 228), 0x5e808080u);
        }
        const float shock = field<float>(self, 864) - float_bits(0x3c888888u);
        field<float>(self, 864) = shock;
        if (shock <= 0.0f) {
            field<int>(self, 852) = 0;
            field<int>(self, 844) = 0;
            field<int>(self, 836) = 0;
        }
    }

    if (0.0f < field<float>(self, 860)) {
        const float slow = field<float>(self, 860) - float_bits(0x3d888888u);
        field<float>(self, 860) = slow;
        if (slow <= 0.0f) {
            field<float>(self, 860) = 0.0f;
            field<signed char>(self, 800) = 0;
        }
        const unsigned flags = field<unsigned>(self, 12);
        if (flags & 8u) {
            field<signed char>(self, 800) = 0;
        } else {
            const float remaining = field<float>(self, 860);
            int amount = static_cast<int>(remaining);
            if (!(remaining <= 60.0f)) amount = 59;
            field<signed char>(self, 800) = static_cast<signed char>(amount);
        }
        const float limit = 60.0f - field<float>(self, 92) * 60.0f;
        if (limit < static_cast<float>(field<signed char>(self, 800))) field<signed char>(self, 800) = static_cast<signed char>(static_cast<int>(limit));
        if (flags & 0x40u) {
            if (field<signed char>(self, 800) >= 31) field<signed char>(self, 800) = 30;
            field<float>(self, 860) = field<float>(self, 860) - float_bits(0x3d088888u);
        } else if (isBoss__FP10GameObject(self) != 0) {
            if (field<signed char>(self, 800) >= 16) field<signed char>(self, 800) = 15;
        } else {
            if (field<signed char>(self, 800) >= 31) field<signed char>(self, 800) = 30;
        }
    }

    const int stun = field<int>(self, 924);
    if (stun != 0) {
        if (stun <= 0) {
            field<int>(self, 924) = 0;
            field<signed char>(self, 800) = 0;
        } else if (field<unsigned>(self, 12) & 8u) {
            field<int>(self, 924) = 0;
            field<signed char>(self, 800) = 0;
        } else {
            const int elapsed = frameNm - field<int>(self, 252);
            if (elapsed < 31) {
                field<signed char>(self, 800) = static_cast<signed char>(elapsed << 1);
                if ((static_cast<int>(static_cast<unsigned>(elapsed) << 25) >> 24) >= 61) field<signed char>(self, 800) = 60;
            } else if (!(stun < 31)) {
                field<signed char>(self, 800) = 60;
            } else {
                field<signed char>(self, 800) = static_cast<signed char>(field<unsigned char>(self, 924));
            }
        }
        const float cap = 60.0f - field<float>(self, 104) * 60.0f;
        if (cap < static_cast<float>(field<signed char>(self, 800))) field<signed char>(self, 800) = static_cast<signed char>(static_cast<int>(cap));
    }

    if (0.0f < field<float>(self, 868)) {
        field<float>(self, 868) = field<float>(self, 868) - float_bits(0x3c888888u);
        if ((frameNm & 0x1f) == 0) {
            const int random = eRand__10GameObject(self);
            const int low = field<short>(self, 880);
            const int range = field<short>(self, 882) - low + 1;
            const float scale = float_bits(0x3dcccccc);
            resolve_attack(self, static_cast<float>(low) * scale, static_cast<float>(random % range) * scale, 0x0080c040u, nullptr);
        }
    }
    if (0.0f < field<float>(self, 872)) {
        field<float>(self, 872) = field<float>(self, 872) - float_bits(0x3c888888u);
        if ((frameNm & 0x1f) == 0) {
            const int random = eRand__10GameObject(self);
            const int low = field<short>(self, 884);
            const int range = field<short>(self, 886) - low + 1;
            const float scale = float_bits(0x3dcccccc);
            resolve_attack(self, static_cast<float>(low) * scale, static_cast<float>(random % range) * scale, 0x0080c040u, nullptr);
        }
    }

    const unsigned long long flags_now = field<unsigned long long>(self, 160);
    if (flags_now & 1ull) {
        bool aggro = false;
        if (field<int>(self, 236) != 0 && localNmPlayersAlive > 0) {
            const int count = localNmPlayersAlive;
            for (int index = 0; index < count; ++index) {
                void* const near_object = NORRATH_W8_POINTER(*reinterpret_cast<unsigned*>(player + localPlayersAlive[index] * 23984 + 21872));
                if (near_object != nullptr) {
                    const float dx = field<float>(near_object, 16) - field<float>(self, 16);
                    const float dy = field<float>(near_object, 20) - field<float>(self, 20);
                    const float dz = field<float>(near_object, 24) - field<float>(self, 24);
                    const float reach = field<float>(self, 788);
                    if (dx * dx + dy * dy + dz * dz < reach * reach) {
                        field<unsigned long long>(self, 160) = flags_now | 0x80000000ull;
                        unsigned char* const entry = static_cast<unsigned char*>(NORRATH_W8_POINTER(field<unsigned>(self, 72))) + 152;
                        const char* (*const function)(void*) = reinterpret_cast<const char* (*)(void*)>(
                            NORRATH_W8_FUNCTION(*reinterpret_cast<unsigned*>(entry + 4)));
                        strcpy(bossMeterName, function(static_cast<char*>(self) + *reinterpret_cast<short*>(entry)));
                        bossMeterMax = static_cast<int>(field<float>(self, 564));
                        bossMeterCurrent = static_cast<int>(field<float>(self, 560));
                        aggro = true;
                        break;
                    }
                }
            }
        }
        if (!aggro) {
            const unsigned long long flags_again = field<unsigned long long>(self, 160);
            if (flags_again & 0x80000000ull) {
                bossMeterCurrent = 0;
                bossMeterMax = 0;
                field<unsigned long long>(self, 160) = flags_again & ~0x80000000ull;
            }
        }
    }

    if ((levelFlags & 0x100) != 0 && field<signed char>(self, 800) < 20) field<signed char>(self, 800) = 20;

    const float height = field<float>(self, 60);
    const float spring = field<float>(self, 64) + (field<float>(self, 24) - height) * 3.0f;
    const float root = square_root(float_bits(0x3db851ebu));
    const float damped = spring - root * spring;
    field<float>(self, 64) = damped;
    field<float>(self, 60) = height + damped * float_bits(0x3c888888u);
    return 0;
}
