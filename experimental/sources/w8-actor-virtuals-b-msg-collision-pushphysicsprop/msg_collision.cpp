// Observed msg_collision__15PushPhysicsPropP10GameObject at 0x001503e8, 488 bytes.
// Part 1 runs only when bit 6 (0x40) of the other object's flag word at +12 is set.
//   The float pair (ox, oy) at other+1196 and other+1200 is scaled to unit length with
//   the reciprocal square root of ox*ox + oy*oy (rsqrt.s on the EE). When the dot
//   product of the unit pair with this object's float pair at +124 and +128 is below
//   1.0, that pair gains the other pair times 0.01 (the image reads 0.01 from a shared
//   float constant at 0x0047fdb8). A unit-vector stack local that is written and never
//   read in the image is not reproduced.
//   Then the objects inside a box are searched with objectFindInBox: the box is this
//   object's floats at +16 and +20 plus and minus the float at +28 in x and y, and
//   -10000.0 to 10000.0 in z (constants at 0x0047fdbc and 0x0047fdc0); up to 16 objects
//   come back in a zero-terminated list. Every listed object whose run-time type equals
//   the PushPhysicsProp type gets the value 190 stored in its word at +252.
// Part 2 always runs: when the other object's run-time type equals the PushPhysicsProp
//   type, the signed word at +252 of this object becomes the larger of the two objects'
//   words at +252.
// A run-time type is read the way the image does it: the other object's word at +72 is a
// table address, the function address at table+4 is called with no arguments and returns
// the type-info pointer, compared with __tf15PushPhysicsProp() through __eq__C9type_info
// RC9type_info. A null object calls __throw_bad_typeid.
// Unknown: what the float pairs and the +252 word are, and whether a full list of 16
// carries a terminator.

extern "C" {
void objectFindInBox__FG6Point3T0PP10GameObjecti(const void* low, const void* high, void** found, int capacity);
void* __tf15PushPhysicsProp();
int __eq__C9type_infoRC9type_info(const void* left, const void* right);
void __throw_bad_typeid();
}

#include "../w8-actor-virtuals-b-common/virtual_call.hpp"

namespace {
typedef float FloatWord __attribute__((may_alias));
typedef int SignedWord __attribute__((may_alias));

inline float reciprocal_sqrt(float squared) {
#if defined(__mips__)
    float result;
    float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(squared));
    return result;
#else
    return 1.0f / __builtin_sqrtf(squared);
#endif
}

void* run_time_type(void* object) {
    if (object == nullptr) __throw_bad_typeid();
    typedef void* (*TypeFunction)();
    const unsigned char* entry = w8avb::table_entry(object, 0);
    TypeFunction function = reinterpret_cast<TypeFunction>(
        w8avb::function_of(*reinterpret_cast<const w8avb::UWord*>(entry + 4)));
    return function();
}

bool is_push_physics_prop(void* object) {
    void* type = run_time_type(object);
    return __eq__C9type_infoRC9type_info(type, __tf15PushPhysicsProp()) != 0;
}
}

extern "C" void msg_collision__15PushPhysicsPropP10GameObject(void* self, void* other_storage) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char* other = static_cast<unsigned char*>(other_storage);
    if ((*reinterpret_cast<const w8avb::UWord*>(other + 12) & 0x40) != 0) {
        const float ox = *reinterpret_cast<const FloatWord*>(other + 1196);
        const float oy = *reinterpret_cast<const FloatWord*>(other + 1200);
        const float scale = reciprocal_sqrt(ox * ox + oy * oy);
        const float unit_x = ox * scale;
        const float unit_y = oy * scale;
        FloatWord* px = reinterpret_cast<FloatWord*>(object + 124);
        FloatWord* py = reinterpret_cast<FloatWord*>(object + 128);
        const float x = *px;
        const float y = *py;
        if (x * unit_x + y * unit_y < 1.0f) {
            *px = x + ox * 0.01f;
            *py = y + *reinterpret_cast<const FloatWord*>(other + 1200) * 0.01f;
        }
        const float reach = *reinterpret_cast<const FloatWord*>(object + 28);
        const float center_x = *reinterpret_cast<const FloatWord*>(object + 16);
        const float center_y = *reinterpret_cast<const FloatWord*>(object + 20);
        float low[3] = {center_x - reach, center_y - reach, -10000.0f};
        float high[3] = {center_x + reach, center_y + reach, 10000.0f};
        void* found[16];
        objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, found, 16);
        for (void** entry = found; *entry != nullptr;) {
            void* listed = *entry++;
            if (is_push_physics_prop(listed)) *reinterpret_cast<SignedWord*>(static_cast<unsigned char*>(listed) + 252) = 190;
        }
    }
    if (is_push_physics_prop(other)) {
        const int theirs = *reinterpret_cast<const SignedWord*>(other + 252);
        const int ours = *reinterpret_cast<const SignedWord*>(object + 252);
        *reinterpret_cast<SignedWord*>(object + 252) = theirs < ours ? ours : theirs;
    }
}
