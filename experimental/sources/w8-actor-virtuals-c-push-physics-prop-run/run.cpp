#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Observed msg_run__15PushPhysicsProp at 0x1505d0, 2732 bytes.
// Fields: a body block starts at +80 (float at +80, position Point3 at +96,
// velocity at +124/+128/+132, angular words at +136..+144, a 3x3 matrix at +148,
// reference point at +232..+240), half extent at +256, height extent at +32, radius
// at +28, floor height at +76, counters at +244, +248 and +252. Unknown: the real
// names of these fields beyond what the body-routine callee names suggest.
//
// Nothing happens when the word at +252 is zero. Otherwise:
// 1. +252 is decremented; the z velocity at +132 loses 0.1 / (float at +80) per call
//    (K0); a nonzero +248 is decremented. Two zero Point3 accumulators A and B start.
// 2. Eight box corners (signs from the bits of the corner index, each +/- half
//    extent) are rotated by the matrix at +148 and moved to the body position. A
//    corner farther than 2*half extent (horizontally) from the reference point sets
//    +248 to 30, marks a "moved" flag and lowers the floor test height to 0. A
//    corner below the floor height (+76, or 0 after that mark) gets a drag force
//    (-0.1 times its velocity in x and y), an impulse along +z when it is moving down
//    (scaled by -0.2) and a spring push of (depth, at most 4.0) * 0.01 minus
//    (z velocity) * 0.01 along +z, added only when positive.
// 3. When "moved" was marked: a zero +244 becomes 60; the horizontal direction from
//    the reference point to the position is normalised (rsqrt.s) and, when its dot
//    with the xy velocity is below 2.0, the xy velocity gains that direction times
//    K4. Both cases then zero the xy velocity under K5 and the three angular words
//    under K6 in absolute sum.
// 4. The accumulated forces are applied once with scale 1.0; the same zeroing runs
//    again. The position is copied from the body position, z drops by the half
//    extent, and the xy velocity times the float at +80 is the intended motion; when
//    its squared length exceeds K7 the motion is clipped by worldClipMotion (radius
//    +28, flags 1), applied, the grid updated, and a clipped result below 1.0 bounces
//    the velocity off the returned normal when the incoming part is positive.
// 5. The position is copied to the reference point and every object found in a box
//    around the position (radius at +28, 16 results) with bit 0x1 clear that is not
//    this object and overlaps the circle test (distance + K9 < radii sum) is
//    handled: with bit 0x10 set the object acts as a support (its top is kept in the
//    support height and its position copied to the reference point) when its top is
//    within 6.0 below this object's height; otherwise the objects are pushed apart
//    by the overlap (clipped by worldClipMotion unless this or the other
//    PushPhysicsProp has a nonzero +248), the xy velocity is deflected away along the
//    separating direction (K9 minus dot, times 1.5) and the other object's virtual
//    slot at byte offset 48 is called with this object, then this object's with the
//    other.
// 6. The body position x/y are written from the position, the body z gains
//    (z velocity * float at +80), and worldFindStandHeight gives the floor, raised
//    to the support height (initially K8); it becomes +76 and, when this object's z
//    minus K10 is at most that floor and the z velocity is not positive, the z is
//    snapped to it. The grid is updated and a nonzero +244 is decremented.
// The constants K0..K10 are the eleven consecutive floats at 0x47fdc4 (0.18, 0.1, 0.01,
// -0.2, 0.05, 0.094, 0.149, 0.001, -100000.0, 0.001, 0.01), written here by bit pattern.
// Inferred: the type-descriptor comparison tells other PushPhysicsProp objects apart
// through the virtual table's first function pointer, as in the Trigger source.
extern "C" void matUntransform__FP6Point3P8Matrix33T0(const void* in, const void* matrix, void* out);
extern "C" void bodyGetVelocity__FP5_bodyP6Point3T1(void* body, const void* point, void* out);
extern "C" void bodyAddForce__FP5_bodyP6Point3N31(void* body, const void* point, const void* force, void* a, void* b);
extern "C" void bodyApplyImpulse__FP5_bodyG6Point3T1f(void* body, const void* point, const void* normal, float scale);
extern "C" void bodyApplyForce__FP5_bodyG6Point3T1f(void* body, const void* a, const void* b, float scale);
extern "C" float worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(void* world, const void* start, const void* move,
                                                                       float radius, int flags, void* normal_out);
extern "C" float worldFindStandHeight__FP12_worldHeaderG6Point3f(void* world, const void* point, float radius);
extern "C" void objectUpdateInGrid__FP10GameObject(void* object);
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(const void* low, const void* high, void** out, int capacity);
extern "C" void* __tf15PushPhysicsProp();
extern "C" int __eq__C9type_infoRC9type_info(void* left, void* right);
extern "C" void __throw_bad_typeid();
extern "C" unsigned char norrath_actors_b_view_4844a0[];

namespace {
using namespace w8c;

typedef void* (*TypeFunction)();
typedef void (*ContactFunction)(void*, void*);

__attribute__((always_inline)) inline float constant(unsigned bits) { return single_from_bits(bits); }

__attribute__((always_inline)) inline void* world() {
    return pointer(read_word(address(norrath_actors_b_view_4844a0)));
}

__attribute__((always_inline)) inline float absolute(float value) {
#if defined(__mips__)
    float result;
    __asm__("abs.s %0, %1" : "=f"(result) : "f"(value));
    return result;
#else
    return __builtin_fabsf(value);
#endif
}

__attribute__((always_inline)) inline float square_root(float value) {
#if defined(__mips__)
    float result;
    __asm__("sqrt.s %0, %1" : "=f"(result) : "f"(value));
    return result;
#else
    return __builtin_sqrtf(value);
#endif
}

__attribute__((always_inline)) inline float reciprocal_root(float value) {
    const float one = 1.0f;
#if defined(__mips__)
    float result;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(value));
    return result;
#else
    return one / __builtin_sqrtf(value);
#endif
}

__attribute__((always_inline)) inline void copy_point(Address from, Address to) {
    write_word(to, read_word(from));
    write_word(to + 4u, read_word(from + 4u));
    write_word(to + 8u, read_word(from + 8u));
}

// Zeroes the planar velocity and the three angular words when their absolute
// sums fall under the two limits.
__attribute__((always_inline)) inline void settle(Address base) {
    if (absolute(read_single(base + 124u)) + absolute(read_single(base + 128u)) < constant(0x3dc08312u)) {
        write_word(base + 124u, 0u);
        write_word(base + 128u, 0u);
    }
    if (absolute(read_single(base + 136u)) + absolute(read_single(base + 140u)) + absolute(read_single(base + 144u)) <
        constant(0x3e189374u)) {
        write_word(base + 136u, 0u);
        write_word(base + 140u, 0u);
        write_word(base + 144u, 0u);
    }
}
}

extern "C" void msg_run__15PushPhysicsProp(void* self) {
    const Address base = address(self);
    unsigned char* const bytes_self = static_cast<unsigned char*>(self);
    void* const body = bytes_self + 80;
    if (read_word(base + 252u) == 0u) return;
    write_single(base + 132u, read_single(base + 132u) - constant(0x3e3851e9u) / read_single(base + 80u));
    write_word(base + 252u, read_word(base + 252u) - 1u);
    float force_a[3] = {0.0f, 0.0f, 0.0f};
    float force_b[3] = {0.0f, 0.0f, 0.0f};
    if (read_word(base + 248u) != 0u) write_word(base + 248u, read_word(base + 248u) - 1u);
    int moved = 0;
    for (int index = 0; index < 8; ++index) {
        float corner[3];
        corner[0] = read_single(base + 256u);
        corner[2] = read_single(base + 256u);
        corner[1] = read_single(base + 256u);
        if ((index & 1) != 0) corner[0] = -corner[0];
        if ((index & 2) != 0) corner[1] = -corner[1];
        if ((index & 4) != 0) corner[2] = -corner[2];
        matUntransform__FP6Point3P8Matrix33T0(corner, bytes_self + 148, corner);
        corner[0] = corner[0] + read_single(base + 96u);
        const float limit_distance = read_single(base + 256u) + read_single(base + 256u);
        corner[1] = corner[1] + read_single(base + 100u);
        corner[2] = corner[2] + read_single(base + 104u);
        float floor = read_single(base + 76u);
        const float dx = corner[0] - read_single(base + 232u);
        const float dy = corner[1] - read_single(base + 236u);
        if (limit_distance * limit_distance < dx * dx + dy * dy) {
            write_word(base + 248u, 30u);
            moved = 1;
            floor = 0.0f;
        }
        if (!(corner[2] < floor)) continue;
        float normal[3] = {0.0f, 0.0f, 1.0f};
        float velocity[3];
        bodyGetVelocity__FP5_bodyP6Point3T1(body, corner, velocity);
        float depth = floor - corner[2];
        float drag[3];
        float vz = velocity[2];
        drag[0] = -(velocity[0] * constant(0x3dccccccu));
        drag[2] = 0.0f;
        drag[1] = -(velocity[1] * constant(0x3dccccccu));
        bodyAddForce__FP5_bodyP6Point3N31(body, corner, drag, force_a, force_b);
        if (vz < 0.0f) {
            bodyApplyImpulse__FP5_bodyG6Point3T1f(body, corner, normal, constant(0xbe4cccccu));
            bodyGetVelocity__FP5_bodyP6Point3T1(body, corner, velocity);
            vz = velocity[2];
        }
        if (constant(0x40800000u) < depth) depth = constant(0x40800000u);
        const float push = depth * constant(0x3c23d70au) - vz * constant(0x3c23d70au);
        if (!(0.0f < push)) continue;
        float spring[3];
        float axis[3] = {0.0f, 0.0f, 1.0f};
        spring[0] = 0.0f * push;
        spring[1] = axis[1] * push;
        spring[2] = axis[2] * push;
        bodyAddForce__FP5_bodyP6Point3N31(body, corner, spring, force_a, force_b);
    }

    if (moved != 0) {
        if (read_word(base + 244u) == 0u) write_word(base + 244u, 60u);
        float direction[3];
        direction[0] = read_single(base + 16u) - read_single(base + 232u);
        direction[1] = read_single(base + 20u) - read_single(base + 236u);
        direction[2] = 0.0f;
        const float inverse = reciprocal_root(direction[0] * direction[0] + direction[1] * direction[1]);
        direction[0] = direction[0] * inverse;
        direction[1] = direction[1] * inverse;
        direction[2] = direction[2] * inverse;
        const float vx = read_single(base + 124u);
        const float vy = read_single(base + 128u);
        if (vx * direction[0] + vy * direction[1] < 2.0f) {
            write_single(base + 128u, vy + direction[1] * constant(0x3d4cccccu));
            write_single(base + 124u, vx + direction[0] * constant(0x3d4cccccu));
        }
    }
    settle(base);
    bodyApplyForce__FP5_bodyG6Point3T1f(body, force_a, force_b, 1.0f);
    settle(base);

    copy_point(base + 96u, base + 16u);
    write_single(base + 24u, read_single(base + 24u) - read_single(base + 256u));
    float motion[3];
    motion[0] = read_single(base + 124u) * read_single(base + 80u);
    motion[1] = read_single(base + 128u) * read_single(base + 80u);
    motion[2] = 0.0f;
    if (constant(0x3a83126eu) < motion[0] * motion[0] + motion[1] * motion[1]) {
        float start[3] = {read_single(base + 16u), read_single(base + 20u), read_single(base + 24u)};
        float normal[3];
        const float fraction = worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(
            world(), start, motion, read_single(base + 28u), 1, normal);
        motion[0] = motion[0] * fraction;
        motion[1] = motion[1] * fraction;
        motion[2] = motion[2] * fraction;
        write_single(base + 16u, read_single(base + 16u) + motion[0]);
        write_single(base + 20u, read_single(base + 20u) + motion[1]);
        write_single(base + 24u, read_single(base + 24u) + motion[2]);
        objectUpdateInGrid__FP10GameObject(self);
        if (fraction < 1.0f) {
            const float nx = normal[0];
            const float ny = normal[1];
            const float vx = read_single(base + 124u);
            const float vy = read_single(base + 128u);
            const float incoming = constant(0x3a83126eu) - (nx * vx + ny * vy);
            if (0.0f < incoming) {
                const float bounce_y = ny * incoming * constant(0x3fc00000u);
                const float bounce_x = nx * incoming * constant(0x3fc00000u);
                write_single(base + 128u, vy + bounce_y);
                write_single(base + 124u, vx + bounce_x);
            }
        }
    }

    copy_point(base + 16u, base + 232u);
    float support = constant(0xc7c35000u);
    {
        float low[3];
        float high[3];
        const float height = read_single(base + 24u);
        const float radius = read_single(base + 28u);
        low[0] = read_single(base + 16u) - radius;
        low[1] = read_single(base + 20u) - radius;
        low[2] = height;
        high[0] = read_single(base + 16u) + radius;
        high[1] = read_single(base + 20u) + radius;
        high[2] = (height + read_single(base + 32u)) - constant(0x40c00000u);
        void* list[16];
        objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 16);
        const float top = read_single(base + 24u) + read_single(base + 32u);
        for (int i = 0; list[i] != nullptr; ++i) {
            void* const other = list[i];
            const Address other_base = address(other);
            if ((read_word(other_base + 12u) & 1u) != 0u) continue;
            if (other == self) continue;
            const float dx = read_single(base + 16u) - read_single(other_base + 16u);
            const float dy = read_single(base + 20u) - read_single(other_base + 20u);
            const float distance = square_root(dx * dx + dy * dy);
            const float reach = read_single(base + 28u) + read_single(other_base + 28u);
            if (!(distance + constant(0x3a83126eu) < reach)) continue;
            if ((read_word(other_base + 12u) & 0x10u) != 0u) {
                const float other_z = read_single(other_base + 24u);
                if (top < other_z) continue;
                const float other_top = other_z + read_single(other_base + 32u);
                if (other_top < read_single(base + 24u)) continue;
                if (other_top - read_single(base + 24u) < constant(0x40c00000u)) {
                    if (support < other_top) support = other_top;
                    copy_point(other_base + 16u, base + 232u);
                    continue;
                }
            }
            float push[3];
            const float ratio = (read_single(base + 28u) + read_single(other_base + 28u) - distance) / distance;
            push[1] = dy * ratio;
            push[0] = dx * ratio;
            push[2] = 0.0f;
            unsigned lock = read_word(base + 248u);
            if (other == nullptr) __throw_bad_typeid();
            void* const type = reinterpret_cast<TypeFunction>(
                pointer(read_word(address(pointer(read_word(other_base + 72u))) + 4u)))();
            if (__eq__C9type_infoRC9type_info(type, __tf15PushPhysicsProp()) != 0) {
                if (read_word(other_base + 248u) != 0u) lock = 1u;
            }
            if (lock == 0u) {
                float start[3] = {read_single(base + 16u), read_single(base + 20u), read_single(base + 24u)};
                const float fraction = worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(
                    world(), start, push, read_single(base + 28u), 1, nullptr);
                push[0] = push[0] * fraction;
                push[1] = push[1] * fraction;
                push[2] = push[2] * fraction;
                write_single(base + 16u, read_single(base + 16u) + push[0]);
                write_single(base + 20u, read_single(base + 20u) + push[1]);
                write_single(base + 24u, read_single(base + 24u) + push[2]);
                objectUpdateInGrid__FP10GameObject(self);
            }
            const float ux = dx / distance;
            const float uy = dy / distance;
            const float vx = read_single(base + 124u);
            const float vy = read_single(base + 128u);
            const float closing = constant(0x3a83126eu) - (ux * vx + uy * vy);
            if (0.0f < closing) {
                const float shove_y = uy * closing * constant(0x3fc00000u);
                const float shove_x = ux * closing * constant(0x3fc00000u);
                write_single(base + 128u, vy + shove_y);
                write_single(base + 124u, vx + shove_x);
            }
            objectUpdateInGrid__FP10GameObject(self);
            {
                void* adjusted;
                ContactFunction function = slot_function<ContactFunction>(other, 48u, &adjusted);
                function(adjusted, self);
            }
            {
                void* adjusted;
                ContactFunction function = slot_function<ContactFunction>(self, 48u, &adjusted);
                function(adjusted, other);
            }
        }
    }

    write_single(base + 96u, read_single(base + 16u));
    write_single(base + 100u, read_single(base + 20u));
    write_single(base + 104u, read_single(base + 104u) + read_single(base + 132u) * read_single(base + 80u));
    float stand_at[3] = {read_single(base + 16u), read_single(base + 20u), read_single(base + 24u)};
    float floor = worldFindStandHeight__FP12_worldHeaderG6Point3f(world(), stand_at, read_single(base + 28u));
    if (floor < support) floor = support;
    write_single(base + 76u, floor);
    if (read_single(base + 24u) - constant(0x3c23d70au) <= floor) {
        if (read_single(base + 132u) <= 0.0f) write_single(base + 24u, floor);
    }
    objectUpdateInGrid__FP10GameObject(self);
    if (read_word(base + 244u) != 0u) write_word(base + 244u, read_word(base + 244u) - 1u);
}
