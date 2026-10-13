// Observed: PushFrustum__11VIClipStackR8VICamera at 0x01098568 is 452 bytes. Arguments:
// the stack object (a0) and a camera (a1). The stack object holds: count at +0, a pointer
// to 16-byte entries at +8, a running total at +12, a pointer to 12-byte records at +20,
// a running total at +24 and a pointer to 16-byte records at +32. With e = entries +
// count*16 and P = the total at +24, V = the total at +12 (both read before anything is
// added) it:
//   - stores P into e+4 and V into e+12;
//   - copies five 16-byte records from camera+416, +432, +448, +464, +480 into the
//     16-byte record array at index P, P+1, ..., P+4 (as raw float words);
//   - copies five 12-byte records from camera+28, +352, +364, +376, +388 into the
//     12-byte record array at index V, V+1, ..., V+4;
//   - sets the total at +24 to P+5 and the total at +12 to V+5;
//   - stores 5 into e+8, then e+0 = (new total at +24) - e[+4] and e+8 = (new total at
//     +12) - e[+12], which are both 5 again, and increments the count.
// It returns the count as it was before the increment. There is no capacity check.
// Inferred: the camera's embedded frustum object starts at +304 (Update initializes it
// there) and Plane(i) returns frustum+96+16*i, so the five copied 16-byte records are
// Plane(1) to Plane(5). The 12-byte records are the camera position at +28 and four
// vectors at frustum+48, +60, +72 and +84. Pop subtracts the per-entry counts at +0 and
// +8 from the totals at +24 and +12, and GetTop returns the entry's two regions.
// Unknown: the meaning of the four frustum vectors, and the member names.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

namespace {
inline unsigned& word(unsigned char* base, int offset) { return *reinterpret_cast<unsigned*>(base + offset); }
inline void copy_words(unsigned char* to, const unsigned char* from, int count) {
    for (int i = 0; i < count; ++i) reinterpret_cast<unsigned*>(to)[i] = reinterpret_cast<const unsigned*>(from)[i];
}
}  // namespace

extern "C" {
int PushFrustum__11VIClipStackR8VICamera(unsigned char* self, unsigned char* camera) {
    int count = static_cast<int>(word(self, 0));
    unsigned char* entry = NORRATH_WORD_TO_POINTER(word(self, 8)) + count * 16;
    int plane_start = static_cast<int>(word(self, 24));
    word(entry, 4) = static_cast<unsigned>(plane_start);
    word(entry, 12) = word(self, 12);
    unsigned char* planes = NORRATH_WORD_TO_POINTER(word(self, 32)) + plane_start * 16;
    for (int i = 0; i < 5; ++i) copy_words(planes + i * 16, camera + 416 + i * 16, 4);
    int vector_start = static_cast<int>(word(self, 12));
    unsigned char* vectors = NORRATH_WORD_TO_POINTER(word(self, 20));
    copy_words(vectors + vector_start * 12, camera + 28, 3);
    copy_words(vectors + (vector_start + 1) * 12, camera + 352, 3);
    copy_words(vectors + (vector_start + 2) * 12, camera + 364, 3);
    copy_words(vectors + (vector_start + 3) * 12, camera + 376, 3);
    copy_words(vectors + (vector_start + 4) * 12, camera + 388, 3);
    word(self, 24) = static_cast<unsigned>(plane_start + 5);
    word(self, 12) = static_cast<unsigned>(vector_start + 5);
    word(NORRATH_WORD_TO_POINTER(word(self, 8)) + count * 16, 8) = 5;
    unsigned plane_total = word(self, 24);
    unsigned entry_vectors = word(entry, 12);
    word(entry, 0) = plane_total - word(entry, 4);
    word(entry, 8) = word(self, 12) - entry_vectors;
    word(self, 0) = static_cast<unsigned>(count + 1);
    return count;
}
}
