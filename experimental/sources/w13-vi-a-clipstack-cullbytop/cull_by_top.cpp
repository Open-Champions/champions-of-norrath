// Observed: CullByTop__11VIClipStackRC6VIBBox at 0x010993a0 is 268 bytes. Arguments: stack (a0)
// and a box (a1: six floats, min x/y/z at +0..+8, max x/y/z at +12..+20). Fields: count n at
// +0, an array of 16-byte records at the word +8, and a plane array at the word +32. The
// top record is record[n - 1] (the count is not checked for zero). Its first int is the
// number of planes m and its second int the index of the first plane; planes are 16-byte
// records of four floats (a, b, c, d). For each of the m planes in order it builds
// the point q with q.x = (0 <= a) ? box.min.x : box.max.x, and likewise y with b and z with c,
// computes ((q.x * a + q.y * b) + q.z * c) + d, and returns 1 as soon as that is > 0.0f.
// When no plane gives a positive value, or m <= 0, it returns 0.
// Inferred: the planes of the top clip volume, each pointing to the inside with a negative
// distance; the chosen corner is the one furthest along the plane normal's opposite side,
// so a positive value means the whole box is outside that plane and is culled.
// Unknown: which of min/max is the nearer corner for the engine's plane orientation, the
// names of the fields, and the record layout beyond the two ints used.
extern "C" {
// A host check defines NORRATH_HOST_ARENA because a 64-bit host cannot keep the two table
// pointers in 4-byte words; the words then hold arena offsets.
#ifdef NORRATH_HOST_ARENA
unsigned char* norrath_host_pointer(unsigned address);
#define WORD_POINTER(word) norrath_host_pointer(word)
#else
#define WORD_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

int CullByTop__11VIClipStackRC6VIBBox(unsigned char* stack, const float* box) {
    int n = *reinterpret_cast<int*>(stack);
    const int* top = reinterpret_cast<const int*>(WORD_POINTER(*reinterpret_cast<unsigned*>(stack + 8)) + (n - 1) * 16);
    int count = top[0];
    const float* plane = reinterpret_cast<const float*>(WORD_POINTER(*reinterpret_cast<unsigned*>(stack + 32)) + top[1] * 16);
    for (int i = 0; i < count; ++i, plane += 4) {
        float x = 0.0f <= plane[0] ? box[0] : box[3];
        float y = 0.0f <= plane[1] ? box[1] : box[4];
        float z = 0.0f <= plane[2] ? box[2] : box[5];
        if (0.0f < x * plane[0] + y * plane[1] + z * plane[2] + plane[3]) {
            return 1;
        }
    }
    return 0;
}
}
