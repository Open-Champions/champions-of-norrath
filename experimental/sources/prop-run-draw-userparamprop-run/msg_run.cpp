#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_run__13UserParamProp at 0x154bb0, 72 bytes.
// animAdvanceAnimation receives the animation state at object+84, the point
// at object+16, and two null arguments. The word at object+348 is then
// replaced by objectActiveProcessing of the object, that word and 480.
// Unknown: what the object+348 word records.
extern "C" void animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void* state, void* point, void* second, short* last);
extern "C" int objectActiveProcessing__FP10GameObjectii(void* object, int value, int limit);

extern "C" void msg_run__13UserParamProp(void* self) {
    using namespace vslots_b;
    const Address base = address(self);
    animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(pointer(base + 84u), pointer(base + 16u), nullptr, nullptr);
    write_word(base + 348u, static_cast<unsigned>(
        objectActiveProcessing__FP10GameObjectii(self, static_cast<int>(read_word(base + 348u)), 480)));
}
