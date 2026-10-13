#include "../w11-rend-a-common/rend.hpp"
// Observed getDynamicLightNumber__FP13PolysetHeader at 0x0017ca40, extent 212. The argument is
// a polyset header ph; its word at +4 holds a slot number in the low 31 bits (0 = none) and a
// flag in bit 31. DynamicLightArray is 16 records of 68 bytes. Each record's word at +0 is
// the draw frame at which it was last claimed and its word at +4 the owner (a polyset header
// address, or zero); the light itself starts at +8.
//  1. When (ph[4] & 0x7fffffff) - 1 is below 16 the polyset already has slot n = that value.
//  2. Otherwise the record with the smallest frame word below drawFrameNm is chosen (the first
//     one on ties, and record 0 when none is below). The old owner, when non-zero, has the low
//     31 bits of its +4 word cleared (its bit 31 stays). The record's owner becomes ph and
//     ph[4] becomes (ph[4] & 0x80000000) | (n + 1). The original reads ph[4] again after clearing
//     the old owner; since clearing keeps bit 31 this makes no observable difference, even when the
//     old owner is ph itself.
//  3. Either way the record's frame word is set to drawFrameNm and n is returned.
// Inferred: the slot allocator for the sixteen dynamic lights, reclaiming the least recently used
// one; drawWorldDynamicLight calls it and treats 16 or more as no light. Unknown: what bit 31 of
// ph[4] means. Nothing here was executed on the EE.
extern "C" {
extern unsigned char DynamicLightArray[16 * 68];
extern int drawFrameNm;
unsigned getDynamicLightNumber__FP13PolysetHeader(unsigned char* ph) {
    using namespace rend;
    unsigned n = (at<u32>(ph, 4) & 0x7fffffffu) - 1u;
    if (n >= 16u) {
        int best = drawFrameNm;
        n = 0;
        for (unsigned i = 0; i < 16u; ++i) {
            const int frame = at<i32>(DynamicLightArray + i * 68, 0);
            if (frame < best) {
                best = frame;
                n = i;
            }
        }
        unsigned char* record = DynamicLightArray + n * 68;
        unsigned char* owner = ptr(record, 4);
        if (owner != nullptr) at<u32>(owner, 4) &= 0x80000000u;
        const unsigned flag = at<u32>(ph, 4) & 0x80000000u;
        set_ptr(record, 4, ph);
        at<u32>(ph, 4) = flag | (n + 1u);
    }
    at<i32>(DynamicLightArray + n * 68, 0) = drawFrameNm;
    return n;
}
}
