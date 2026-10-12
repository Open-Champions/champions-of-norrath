// Observed worldFindWaterPatches__FP12_worldHeaderG6Point3T1PP13PolysetHeaderi
// at 0x0017a610, 516 bytes.
// Arguments: header, two Point3 values passed by address (A, then B; three
// floats each), an output array of record addresses, and a maximum count.
// The output array is first set to a zero terminator.
//
// A stack of up to 16 stream addresses starts with the word at header+4.
// While the stack is nonempty an address is popped and its signed half-word h
// is read.
//  * h bit 0x8000 set, 0x4000 clear: the entry is ignored.
//  * h bits 0x8000 and 0x4000 set: a split node. axis = h & 0xfff indexes
//    the floats of A and B. The 32-bit values at +2 and +6 are built from two
//    sign-extended halves OR'd together (low half | high half << 16), shifted
//    left by 2 and converted to float: lo, hi. If A[axis] <= lo the address
//    node+20 is pushed. Then if hi <= B[axis] the address
//    node+18 + 2*half(node+18) is pushed.
//  * h bit 0x8000 clear: a leaf. h is the number of entries; each entry is an
//    unsigned half-word index of a 60-byte record at the address in
//    header+36. A record whose word at +36 has bit 0x80 and whose box,
//    floats at +8/+12/+16 (minimum) and +20/+24/+28 (maximum), overlaps
//    A..B (no component of B is below the minimum and no maximum is below A,
//    all tests strict) is stored at the next output slot. After each store
//    the maximum is decremented and the walk ends when it reaches 1.
// After the walk the output array is terminated with a zero.
// The record meaning is not established; the 12 bytes at node+6..+17 beyond
// the two keys are not read here.
#if defined(NORRATH_HOST_WORLD)
extern unsigned char* norrath_world_arena;
#endif

namespace {
inline unsigned word(const unsigned char* base, unsigned offset) {
    return *reinterpret_cast<const unsigned*>(base + offset);
}
inline unsigned char* address(unsigned bits) {
#if defined(NORRATH_HOST_WORLD)
    return norrath_world_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
inline int half(const unsigned char* base) { return *reinterpret_cast<const short*>(base); }
inline int combine(const unsigned char* base) {
    return half(base) | (half(base + 2) << 16);
}
}

extern "C" void worldFindWaterPatches__FP12_worldHeaderG6Point3T1PP13PolysetHeaderi(
    void* world, const float* low, const float* high, unsigned char** out, int limit) {
    const unsigned char* header = static_cast<const unsigned char*>(world);
    unsigned char* stack[16];
    int depth = 1;
    float box_low[3] = {low[0], low[1], low[2]};
    float box_high[3] = {high[0], high[1], high[2]};
    *out = nullptr;
    stack[0] = address(word(header, 4));

    while (depth > 0) {
        --depth;
        const unsigned char* node = stack[depth];
        const int h = half(node);
        if ((h & 0x8000) != 0) {
            if ((h & 0x4000) == 0) continue;
            const unsigned axis = static_cast<unsigned>(h) & 0xfffu;
            const float near_limit = static_cast<float>(combine(node + 2) << 2);
            const float far_limit = static_cast<float>(combine(node + 6) << 2);
            if (box_low[axis] <= near_limit) {
                stack[depth++] = const_cast<unsigned char*>(node + 20);
            }
            if (far_limit <= box_high[axis]) {
                stack[depth++] = const_cast<unsigned char*>(node + 18 + half(node + 18) * 2);
            }
        } else {
            int remaining = h;
            const unsigned char* entry = node + 2;
            while (remaining != 0) {
                const unsigned index = *reinterpret_cast<const unsigned short*>(entry);
                entry += 2;
                unsigned char* record = address(word(header, 36)) + index * 60u;
                if ((word(record, 36) & 0x80u) != 0) {
                    const float* f = reinterpret_cast<const float*>(record);
                    const bool outside = box_high[0] < f[2] || f[5] < box_low[0] ||
                                         box_high[1] < f[3] || f[6] < box_low[1] ||
                                         box_high[2] < f[4] || f[7] < box_low[2];
                    if (!outside) {
                        *out++ = record;
                        if (--limit == 1) {
                            *out = nullptr;
                            return;
                        }
                    }
                }
                --remaining;
            }
        }
    }
    *out = nullptr;
}
