// Observed: CullByTop__11VIClipStackRC8VISphere at 0x01099588 is 152 bytes in the pinned SLUS-20565 image. The stack holds an entry
// count at +0, a pointer to 16-byte entries at +8 and a pointer to 16-byte planes at +32 (layout seen in w12-vimath-a). The routine
// takes the last entry (index count - 1): its word at +0 is a plane count n and its word at +4 the index of its first plane. For each
// of the n planes (a, b, c, d) it computes ((sx*a + sy*b) + sz*c) + d for the sphere's centre (three floats at +0, +4, +8) and
// returns 1 as soon as the sphere's radius (float at +12) is strictly less than that value. It returns 0 if no plane does, or
// immediately when n <= 0. The entry count is not checked, so an empty stack reads before the entry array.
// Inferred: the sphere is wholly outside the top clip region when it lies farther than its radius on the positive side of any plane
// (planes face outward); the result is a cull flag.
// Unknown: the sign convention of the planes beyond that comparison, and the stack's remaining fields. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int CullByTop__11VIClipStackRC8VISphere(const unsigned char* self, const float* sphere) {
    int count = *reinterpret_cast<const int*>(self);
    const unsigned char* entries = NORRATH_WORD_TO_POINTER(*reinterpret_cast<const unsigned*>(self + 8));
    const unsigned char* planes = NORRATH_WORD_TO_POINTER(*reinterpret_cast<const unsigned*>(self + 32));
    const int* top = reinterpret_cast<const int*>(entries + (count - 1) * 16);
    int planeCount = top[0];
    const float* plane = reinterpret_cast<const float*>(planes + top[1] * 16);
    for (int i = 0; i < planeCount; ++i, plane += 4) {
        float distance = sphere[0] * plane[0] + sphere[1] * plane[1] + sphere[2] * plane[2] + plane[3];
        if (sphere[3] < distance) return 1;
    }
    return 0;
}
}
