// Observed doSteering__7Vehicle at 0x00274dd0, 1220 bytes. Nothing is
// returned. Nothing here was executed on the EE. Field meanings are not
// established here, so offsets stay numeric. Named values: cur is the float at
// +800 (the value this routine moves); d the signed word at +956 (-1 = none);
// stick = the float at pad + 20 + 88*d (d used as is, even -1); L below.
//   Nothing happens when the word at +1024 is non-zero.
//   L = float(+860) + (float(+864) - float(+860)) * min.s(1.0, float(+840) *
//   K1), K1 the float 0x3d888888.
//   Auto mode (word +2148 non-zero): a target t is found. With the pointer
//   w at +2268 non-null, t = L when distanceToPlane(*(w+2320), *(w+2320)+24,
//   w+16) is above 0.0, else -L. With w null: a1 = atan2f(float(+1980),
//   -float(+1984)) * D and a2 = atan2f(float(+596), -float(+600)) * D (D the
//   float 0x42652de0, about 180/pi); t = a2 - a1; h = float(+852) * 0.5; when
//   the word +1020 is zero and h < |float(+828)| then t += ((|f828| - h) / h)
//   * 10.0, negated when float(+828) < 0; t is wrapped into (-180, 180]
//   by one subtraction or addition of 360.0 and clamped to [-L, L].
//   Then cur = cur + ((t - cur) * 0.25) * 0x3f7fffff-float; done.
//   Manual mode: the signed byte at +108 (c), when non-zero, drops by one and
//   the next block is skipped unless that leaves it zero. The block calls the
//   Vehicle virtual in table offset 216 (the eventSteeringChange slot:
//   halfword delta, pointer at +4) and sets c = 10 when float(+952) * stick <
//   0.0, or float(+952) == 0.0 and stick != 0.0. Then float(+952) = stick
//   (before clamping) and stick is clamped to [-1, 1]; stick = 0.0 when
//   gameControlPaused is non-zero or the record pointer at player + 23984*d +
//   21872 is non-null with bit 0x400000 set in its word at +12; and stick =
//   float(+752) when the signed byte cutMode is non-zero. With e = (M0, M1,
//   0) / sqrt(M0^2 + M1^2) (rsqrt.s) for the matrix floats at +1116, +1120
//   and V = the Point3 at +1188: when |V| > 2.0, y = 90.0 - acosf(Vn.e) * D,
//   Vn = V * (1.0/|V|) and dot as (x*x + y*y) + z*z, else y = 0.0. y *= 0.5; the
//   target t is y when d == -1 or |stick| <= 0.125, else y + L * -stick. cur
//   moves toward t by the step R = float(+856) * (1/60) (upward) or -R
//   (downward), not past t; finally cur is clamped to [-L, L].
// Point3 values passed by value in the original are passed as the address of a
// caller-owned copy.
struct __attribute__((may_alias)) Point3 { float x, y, z; };

extern "C" {
float distanceToPlane__FP6Point3N20(const Point3* a, const Point3* b, const Point3* c);
float atan2f(float y, float x);
float acosf(float x);
extern unsigned char pad[];
extern unsigned char player[];
extern int gameControlPaused;
extern signed char cutMode;
}

typedef float AliasedFloat __attribute__((may_alias));
typedef int AliasedInt __attribute__((may_alias));
typedef short AliasedShort __attribute__((may_alias));
typedef unsigned char* AliasedPointer __attribute__((may_alias));
typedef void (*VirtualFunction)(unsigned char*);
typedef VirtualFunction AliasedFunction __attribute__((may_alias));

namespace {
inline int& word(unsigned char* base, int offset) { return *reinterpret_cast<AliasedInt*>(base + offset); }
inline float& real(unsigned char* base, int offset) { return *reinterpret_cast<AliasedFloat*>(base + offset); }
inline float min_s(float a, float b) {
#if defined(__mips__)
    float out;
    __asm__("min.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b));
    return out;
#else
    return b < a ? b : a;
#endif
}
inline float rsqrt_s(float numerator, float value) {
#if defined(__mips__)
    float out;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(out) : "f"(numerator), "f"(value));
    return out;
#else
    return numerator / __builtin_sqrtf(value);
#endif
}
}

extern "C" void doSteering__7Vehicle(unsigned char* vehicle) {
    if (word(vehicle, 1024) != 0) return;
    const float low = real(vehicle, 860);
    const float limit = low + (real(vehicle, 864) - low) * min_s(1.0f, real(vehicle, 840) * 0x1.111110p-4f);
    float target;
    if (word(vehicle, 2148) != 0) {
        unsigned char* const blocker = *reinterpret_cast<AliasedPointer*>(vehicle + 2268);
        if (blocker != 0) {
            unsigned char* const node = *reinterpret_cast<AliasedPointer*>(blocker + 2320);
            const float side = distanceToPlane__FP6Point3N20(reinterpret_cast<Point3*>(node), reinterpret_cast<Point3*>(node + 24),
                                                             reinterpret_cast<Point3*>(blocker + 16));
            target = 0.0f < side ? limit : -limit;
        } else {
            const float first = atan2f(real(vehicle, 1980), -real(vehicle, 1984)) * 0x1.ca5bc0p+5f;
            const float second = atan2f(real(vehicle, 596), -real(vehicle, 600)) * 0x1.ca5bc0p+5f;
            const float half = real(vehicle, 852) * 0.5f;
            target = second - first;
            if (word(vehicle, 1020) == 0) {
                const float error = real(vehicle, 828);
                const float magnitude = error < 0.0f ? -error : error;
                if (half < magnitude) {
                    float extra = (magnitude - half) / half * 10.0f;
                    if (error < 0.0f) extra = -extra;
                    target = target + extra;
                }
            }
            if (180.0f < target) {
                target = target - 360.0f;
            } else if (target < -180.0f) {
                target = target + 360.0f;
            }
            if (target < -limit) target = -limit;
            if (limit < target) target = limit;
        }
        const float current = real(vehicle, 800);
        real(vehicle, 800) = current + (target - current) * 0.25f * 0x1.fffffep-1f;
        return;
    }

    const int driver = word(vehicle, 956);
    float stick = *reinterpret_cast<AliasedFloat*>(pad + 20 + 88 * driver);
    signed char* const counter = reinterpret_cast<signed char*>(vehicle + 108);
    bool run_block = true;
    if (*counter != 0) {
        *counter = static_cast<signed char>(*counter - 1);
        if (*counter != 0) run_block = false;
    }
    if (run_block) {
        const float previous = real(vehicle, 952);
        if (previous * stick < 0.0f || (previous == 0.0f && stick != 0.0f)) {
            unsigned char* const entry = *reinterpret_cast<AliasedPointer*>(vehicle + 72) + 216;
            (*reinterpret_cast<AliasedFunction*>(entry + 4))(vehicle + *reinterpret_cast<AliasedShort*>(entry));
            *counter = 10;
        }
    }
    real(vehicle, 952) = stick;
    if (1.0f < stick) stick = 1.0f;
    if (stick < -1.0f) stick = -1.0f;
    if (gameControlPaused != 0) {
        stick = 0.0f;
    } else {
        unsigned char* const record = *reinterpret_cast<AliasedPointer*>(player + 23984 * driver + 21872);
        if (record != 0 && (word(record, 12) & 0x400000) != 0) stick = 0.0f;
    }
    if (cutMode != 0) stick = real(vehicle, 752);

    const float m0 = real(vehicle, 1116);
    const float m1 = real(vehicle, 1120);
    Point3 velocity = *reinterpret_cast<Point3*>(vehicle + 1188);
    const float inverse = rsqrt_s(1.0f, m0 * m0 + m1 * m1);
    const float ex = m0 * inverse;
    const float ey = m1 * inverse;
    const float ez = 0.0f * inverse;
    const float speed = __builtin_sqrtf(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
    const float scale = 1.0f / speed;
    velocity.z = velocity.z * scale;
    velocity.x = velocity.x * scale;
    velocity.y = velocity.y * scale;
    float angle = 0.0f;
    if (2.0f < speed) angle = 90.0f - acosf(velocity.x * ex + velocity.y * ey + velocity.z * ez) * 0x1.ca5bc0p+5f;
    angle = angle * 0.5f;
    float goal;
    if (driver == -1) {
        goal = angle;
    } else {
        const float absolute = stick < 0.0f ? -stick : stick;
        if (0.125f < absolute) goal = angle + limit * -stick;
        else goal = angle;
    }

    const float current = real(vehicle, 800);
    float step;
    if (current < goal) step = real(vehicle, 856) * 0x1.111110p-6f;
    else if (goal < current) step = -real(vehicle, 856) * 0x1.111110p-6f;
    else step = 0.0f;
    if (0.0f < step) {
        if (goal < current + step) step = goal - current;
    }
    if (step < 0.0f) {
        if (current + step < goal) step = goal - current;
    }
    real(vehicle, 800) = current + step;
    if (real(vehicle, 800) < -limit) real(vehicle, 800) = -limit;
    if (limit < real(vehicle, 800)) real(vehicle, 800) = limit;
}
