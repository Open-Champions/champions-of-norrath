// Observed persue2__8CreatureP10GameObjectfff at 0x00110bc8, 1216 bytes, 240-byte frame. Arguments: a
// creature C, a target object T, and three floats of which the first is not read (here b is the
// second, c the third). Returns nothing. Fields used: C+16/+20/+24 position, C+28 a float "span",
// C+56 halfword heading, C+72 table address word, C+176 speed float, C+180/+184 two floats, C+260
// route point (three floats, x = -1.0 meaning none), C+569 signed byte, C+712 frame stamp, C+760
// steering vector (three floats), C+800 unsigned byte. T+16/+20/+24 is its position. A table
// entry is 8 bytes: a signed halfword adjustment, then a function address at +4. "move" is the
// entry at table + 176, called as move(C + adjustment, 0, point) and its result is cut to a short.
//  1. Nothing happens when T is null. d = T.pos - C.pos with z taken as (T.z + 13) - (C.z + 13), and
//     dist = |sqrt(dx^2 + dy^2 + dz^2)|.
//  2. When ((frameNm ^ C[569]) & 7) == 0 (one frame in eight): wallAndObjectSteering(&slot, C,
//     min(dist, span + 10), span + span, T) is called and its Point3 replaces the steering vector at C+760.
//  3. Moving: when both x and y of the steering vector are nonzero and b + 5 < dist, move(C, 0, steering
//     + C.pos) is called. Otherwise worldHitscan(world, (C.x, C.y, C.z + 13), d, the float 1.1, 3)
//     (the float is the word 0x3f8ccccc at gp-32156) gives h:
//     - h < 0 or h > 1 (a clear line): move(C, 0, T.pos).
//     - else up to 16 rounds, each: the route point x at C+260 is tested; a refresh happens when it is
//       -1.0, or when the xy distance from C to it is below span, or when C+712 < frameNm. A refresh
//       sets C+712 = frameNm + 30 and calls getNextRoutePoint(C, &C[260], T.pos, b, c, 0); when the
//       point's x is -1.0 afterwards the rounds stop. With no refresh the rounds also stop.
//       After the rounds: move(C, 0, T.pos) when C[260] is -1.0 (its result is dropped and r stays 0),
//       else move(C, 0, the point at C+260), whose result is r.
//  4. With r the short result of the last kept move (0 when none): speed = C[176]. When |r| > 16000, speed and C[180] and C[184] are
//     halved (only the last two are stored back). When 1 <= C[800] <= 60, speed is multiplied by
//     (60 - C[800]) * the float 0x3c888888 (the gp-32152 word, about 1/60). When b + 5 < dist,
//     move2(C, (icos(h) * speed, isin(h) * speed, 0)) is called with h the halfword at C+56.
// Inferred: a chase step: the creature steers toward T, using a route when a wall blocks the straight
// line, and slows when the move result is large. Unknown: the creature field names, what C+176/+180/
// +184 and C+800 are, what the first float is, and the entry-176 result.

struct Point3 { float x, y, z; };
struct _worldHeader;

extern "C" {
extern _worldHeader* world;
extern int frameNm;
float icos__Fi(int angle);
float isin__Fi(int angle);
Point3* wallAndObjectSteering__FP8CreatureffP10GameObject(Point3* out, void* creature, float reach, float unused, void* ignore);
float worldHitscan__FP12_worldHeaderG6Point3T1fi(_worldHeader* world, Point3 start, Point3 direction, float range, int mode);
void getNextRoutePoint__8CreatureP6Point3G6Point3ffi(void* creature, Point3* route, Point3 target, float a, float b, int c);
void move2__8CreatureG6Point3(void* creature, Point3 movement);
}

namespace {
inline float squareRoot(float value) {
#if defined(__mips__)
    float result;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(result) : "f"(value));
    return result;
#else
    return __builtin_sqrtf(value);
#endif
}

inline float minimum(float a, float b) {
#if defined(__mips__)
    float out;
    __asm__("min.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b));
    return out;
#else
    return a < b ? a : b;
#endif
}

inline float absolute(float value) {
#if defined(__mips__)
    float out;
    __asm__("abs.s %0, %1" : "=f"(out) : "f"(value));
    return out;
#else
    return __builtin_fabsf(value);
#endif
}

template <typename T>
inline T fromWord(unsigned word) {
    return reinterpret_cast<T>(static_cast<__UINTPTR_TYPE__>(word));
}

inline float& real(unsigned char* base, int offset) { return *reinterpret_cast<float*>(base + offset); }

inline Point3 pointAt(unsigned char* base, int offset) {
    Point3 p;
    p.x = real(base, offset);
    p.y = real(base, offset + 4);
    p.z = real(base, offset + 8);
    return p;
}

// The table entry at +176 of the creature's table: a short adjustment, then the function address.
inline short move(unsigned char* creature, Point3 destination) {
    unsigned char* entry = fromWord<unsigned char*>(*reinterpret_cast<unsigned*>(creature + 72)) + 176;
    typedef int (*Move)(void*, int, Point3);
    // The image keeps only the low 16 bits of the result (shift left, arithmetic shift right).
    return static_cast<short>(fromWord<Move>(*reinterpret_cast<unsigned*>(entry + 4))(creature + *reinterpret_cast<short*>(entry), 0, destination));
}
}  // namespace

extern "C" void persue2__8CreatureP10GameObjectfff(void* self, void* target, float first, float b, float c) {
    (void)first;
    unsigned char* const creature = static_cast<unsigned char*>(self);
    unsigned char* const object = static_cast<unsigned char*>(target);
    if (object == nullptr) return;
    short result = 0;
    const float thirteen = 13.0f;
    Point3 direction;
    direction.x = real(object, 16) - real(creature, 16);
    direction.y = real(object, 20) - real(creature, 20);
    direction.z = (real(object, 24) + thirteen) - (real(creature, 24) + thirteen);
    const float distance = absolute(squareRoot(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z));
    if (((frameNm ^ *reinterpret_cast<signed char*>(creature + 569)) & 7) == 0) {
        Point3 steering;
        wallAndObjectSteering__FP8CreatureffP10GameObject(&steering, creature, minimum(distance, real(creature, 28) + 10.0f),
                                                          real(creature, 28) + real(creature, 28), object);
        real(creature, 760) = steering.x;
        real(creature, 764) = steering.y;
        real(creature, 768) = steering.z;
    }
    if (real(creature, 760) != 0.0f && real(creature, 764) != 0.0f && b + 5.0f < distance) {
        Point3 destination;
        destination.x = real(creature, 760) + real(creature, 16);
        destination.y = real(creature, 764) + real(creature, 20);
        destination.z = real(creature, 768) + real(creature, 24);
        result = move(creature, destination);
    } else {
        Point3 start;
        start.x = real(creature, 16);
        start.y = real(creature, 20);
        start.z = real(creature, 24) + thirteen;
        const float hit = worldHitscan__FP12_worldHeaderG6Point3T1fi(world, start, direction,
                                                                     __builtin_bit_cast(float, 0x3f8ccccc), 3);
        if (hit < 0.0f || 1.0f < hit) {
            result = move(creature, pointAt(object, 16));
        } else {
            for (int round = 0; round < 16; ++round) {
                bool refresh;
                const float routeX = real(creature, 260);
                const int frame = frameNm;
                if (routeX == -1.0f) {
                    refresh = true;
                } else {
                    const float dx = routeX - real(creature, 16);
                    const float dy = real(creature, 264) - real(creature, 20);
                    if (squareRoot(dx * dx + dy * dy) < real(creature, 28)) refresh = true;
                    else refresh = *reinterpret_cast<int*>(creature + 712) < frame;
                }
                if (!refresh) break;
                *reinterpret_cast<int*>(creature + 712) = frame + 30;
                getNextRoutePoint__8CreatureP6Point3G6Point3ffi(creature, reinterpret_cast<Point3*>(creature + 260),
                                                                pointAt(object, 16), b, c, 0);
                if (real(creature, 260) == -1.0f) break;
            }
            if (real(creature, 260) == -1.0f) (void)move(creature, pointAt(object, 16));  // this path keeps no result
            else result = move(creature, pointAt(creature, 260));
        }
    }
    float speed = real(creature, 176);
    const int magnitude = result < 0 ? -result : result;
    if (magnitude >= 16001) {
        speed = speed * 0.5f;
        real(creature, 180) = real(creature, 180) * 0.5f;
        real(creature, 184) = real(creature, 184) * 0.5f;
    }
    const unsigned char level = creature[800];
    if (static_cast<unsigned>(level) - 1u < 60u) {
        speed = speed * (static_cast<float>(60 - static_cast<signed char>(level)) * __builtin_bit_cast(float, 0x3c888888u));
    }
    if (b + 5.0f < distance) {
        const int heading = *reinterpret_cast<short*>(creature + 56);
        Point3 movement;
        movement.x = icos__Fi(heading) * speed;
        movement.y = isin__Fi(heading) * speed;
        movement.z = 0.0f;
        move2__8CreatureG6Point3(creature, movement);
    }
}
