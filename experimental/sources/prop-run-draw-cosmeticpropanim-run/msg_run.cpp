#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_run__16CosmeticPropAnim at 0x1551b8, 68 bytes.
// objectActiveProcessing receives the object, the signed byte at object+140
// and 480. Its low byte replaces object+140. animAdvanceAnimation then gets
// the animation state at object+100, the point at object+16, and two nulls.
// Unknown: what the object+140 byte records.
extern "C" void animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void* state, void* point, void* second, short* last);
extern "C" int objectActiveProcessing__FP10GameObjectii(void* object, int value, int limit);

extern "C" void msg_run__16CosmeticPropAnim(void* self) {
    using namespace vslots_b;
    const Address base = address(self);
    const int old = static_cast<signed char>(read_u8(base + 140u));
    write_u8(base + 140u, static_cast<unsigned>(objectActiveProcessing__FP10GameObjectii(self, old, 480)));
    animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(pointer(base + 100u), pointer(base + 16u), nullptr, nullptr);
}
