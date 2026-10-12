#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_run__4Prop at 0x147e30, 780 bytes.
// Halfword object+320 bit 0 clear selects the motion half. The singles at
// object+252 and +256 are scaled by K0 into a motion {x, y, 0}. When x*x+y*y
// is above K1, worldClipMotion receives the world word (0x4844a0), the point
// at object+16, the motion, the single at object+28, 1 and a 12-byte output.
// The motion is scaled by the returned fraction and added to the point,
// objectUpdateInGrid runs, and a fraction below 1.0 with the output's x and y
// forming a dot product with object+252/+256: d = K1 - dot; for d above zero
// each of +252/+256 gains output * d * 1.5. worldFindStandHeight then gets the
// (possibly moved) point and the single at object+28. With that height plus
// the single at object+264 as a floor, the single at object+48 loses K2, the
// single at object+24 gains the new object+48 * K3, and both are stored. At or
// below the floor with object+48 not above zero, +252/+256 are scaled by K4,
// +24 is set to the floor, and +48 becomes (-object+48) * K6 when object+48 is
// below K5, else 0.0. Halfword +56 then gains halfword +268, and +268 loses
// 10 as a signed halfword, clamped at 0 when negative. Both halves continue.
// Halfword +270 loses 1. When the signed result is not above zero, bit 0 of
// +320 is clear, bit 0 is set and bit 1 is clear, objectRemoveFromRunList runs
// and the method returns. After a set bit 0 or bit 1, or a positive result, a
// signed +270 below -60 reduces the singles at +24 and +316 by K7, and below
// -300 calls vtable byte 8 with integer 3.
// Constants (observed .lit4 words, 0x47fd38..0x47fd54): K0 0x3c888888,
// K1 0x3a83126e, K2 0x41d7ffff, K3 0x3c888888, K4 0x3f7ae147, K5 0xbdcccccc,
// K6 0x3ecccccc, K7 0x3dcccccc. Unknown: what any field or the constants mean.
extern "C" unsigned char norrath_actors_b_view_4844a0[];
extern "C" float worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(
    void* world, const void* start, const void* motion, float extent, int flag, void* output);
extern "C" float worldFindStandHeight__FP12_worldHeaderG6Point3f(void* world, const void* point, float extent);
extern "C" void objectUpdateInGrid__FP10GameObject(void* object);
extern "C" void objectRemoveFromRunList__FP10GameObject(void* object);

extern "C" void msg_run__4Prop(void* self) {
    using namespace vslots_b;
    const Address base = address(self);
    const float k0 = 0x1.111110p-6f, k1 = 0x1.0624dcp-10f, k2 = 0x1.affffep+4f, k3 = 0x1.111110p-6f;
    const float k4 = 0x1.f5c28ep-1f, k5 = -0x1.999998p-4f, k6 = 0x1.999998p-2f, k7 = 0x1.999998p-4f;
    if ((read_u16(base + 320u) & 1u) == 0u) {
        float motion[3];
        float output[3] = {0.0f, 0.0f, 0.0f};
        unsigned char start[12];
        motion[1] = read_single(base + 256u) * k0;
        motion[0] = read_single(base + 252u) * k0;
        motion[2] = 0.0f;
        if (k1 < motion[0] * motion[0] + motion[1] * motion[1]) {
            for (unsigned i = 0; i < 12u; ++i) start[i] = static_cast<unsigned char>(read_u8(base + 16u + i));
            void* const world = pointer(read_word(address(norrath_actors_b_view_4844a0)));
            const float fraction = worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(
                world, start, motion, read_single(base + 28u), 1, output);
            motion[0] *= fraction;
            motion[1] *= fraction;
            motion[2] *= fraction;
            write_single(base + 16u, read_single(base + 16u) + motion[0]);
            write_single(base + 20u, read_single(base + 20u) + motion[1]);
            write_single(base + 24u, read_single(base + 24u) + motion[2]);
            objectUpdateInGrid__FP10GameObject(self);
            if (fraction < 1.0f) {
                const float vx = read_single(base + 252u);
                const float vy = read_single(base + 256u);
                const float depth = k1 - (output[0] * vx + output[1] * vy);
                if (0.0f < depth) {
                    write_single(base + 256u, vy + output[1] * depth * 1.5f);
                    write_single(base + 252u, vx + output[0] * depth * 1.5f);
                }
            }
        }
        for (unsigned i = 0; i < 12u; ++i) start[i] = static_cast<unsigned char>(read_u8(base + 16u + i));
        const float height = worldFindStandHeight__FP12_worldHeaderG6Point3f(
            pointer(read_word(address(norrath_actors_b_view_4844a0))), start, read_single(base + 28u));
        const float floor_z = read_single(base + 264u) + height;
        const float fall = read_single(base + 48u) - k2;
        const float z = read_single(base + 24u) + fall * k3;
        write_single(base + 48u, fall);
        write_single(base + 24u, z);
        if (z <= floor_z && fall <= 0.0f) {
            const float vx = read_single(base + 252u) * k4;
            const float vy = read_single(base + 256u) * k4;
            write_single(base + 24u, floor_z);
            write_single(base + 252u, vx);
            write_single(base + 256u, vy);
            write_single(base + 48u, fall < k5 ? -fall * k6 : 0.0f);
        }
        const unsigned step = read_u16(base + 268u);
        write_u16(base + 56u, read_u16(base + 56u) + step);
        const short reduced = static_cast<short>(step - 10u);
        write_u16(base + 268u, reduced >= 0 ? static_cast<unsigned>(reduced) : 0u);
    }
    const short counter = static_cast<short>(read_u16(base + 270u) - 1u);
    write_u16(base + 270u, static_cast<unsigned>(counter));
    if (counter <= 0) {
        const unsigned flags = read_u16(base + 320u);
        if ((flags & 1u) == 0u) {
            write_u16(base + 320u, flags | 1u);
            if ((flags & 2u) == 0u) {
                objectRemoveFromRunList__FP10GameObject(self);
                return;
            }
        }
    }
    const short current = read_i16(base + 270u);
    if (current < -60) {
        write_single(base + 24u, read_single(base + 24u) - k7);
        write_single(base + 316u, read_single(base + 316u) - k7);
        if (current < -300 && self != nullptr) {
            void* adjusted = nullptr;
            using SlotCall = void (*)(void*, int);
            SlotCall call = slot_function<SlotCall>(self, 8u, &adjusted);
            call(adjusted, 3);
        }
    }
}
