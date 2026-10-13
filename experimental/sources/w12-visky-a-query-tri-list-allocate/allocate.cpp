// Observed: Allocate__14VIQueryTriList at 0x010d0f20 is 176 bytes. The word at byte offset 4 of
// the object (this) is a count n. When n is zero the routine returns 0 and stores nothing. Otherwise
// it requests 40 * n bytes with __builtin_vec_new and, if a block came back, zeroes nine floats in
// each 40-byte record, at record offsets 4, 8, 12, 16, 20, 24, 28, 32 and 36 (offset 0 is left as
// the allocator returned it). The block pointer, null or not, is stored at this+8. The result is -1
// when the block is null and 0 otherwise.
// Inferred: three 3-float vectors per record (a triangle) and a leading word of another kind; a
// per-record ordering of the stores is not observable. Unknown: the meaning of word 0 and the
// member names.
extern "C" {
void* __builtin_vec_new(unsigned size);

int Allocate__14VIQueryTriList(unsigned char* self) {
    const unsigned count = *reinterpret_cast<const unsigned*>(self + 4);
    if (count == 0) return 0;
    unsigned char* block = static_cast<unsigned char*>(__builtin_vec_new(count * 40));
    if (block != nullptr) {
        for (unsigned i = 0; i < count; ++i) {
            float* triangle = reinterpret_cast<float*>(block + 40 * i + 4);
            for (int k = 0; k < 9; ++k) triangle[k] = 0.0f;
        }
    }
    *reinterpret_cast<unsigned*>(self + 8) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(block));
    return block != nullptr ? 0 : -1;
}
}
