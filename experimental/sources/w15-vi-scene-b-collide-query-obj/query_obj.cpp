// Observed: QueryObj__9VICollideRC6VIBBoxR14VIQueryTriListi at 0x01092ad0, 184 bytes, in the pinned SLUS-20565 image. Arguments: the collide record (C), a box (X, six floats), a triangle-list
// reference (T) and an index (I). The collision buffer B is the first word of record I of the 12-byte records addressed by the word at C+536 (no range test); it is read before the first call.
// A local six-float box L is zero-filled, then Transform__C6VIBBoxRC10VIMatrix44R6VIBBox(X, C+80, L) runs, and the result is
// Query__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer(C, X, L, T, B), whose result is returned unchanged.
// Inferred: C+80 is the inverse of the world matrix (SetWorld fills it), so L is X moved into the collide object's local space; the two boxes are passed so the query can use both. This is a name-based reading.
// Unknown: the box layout convention beyond six floats, the triangle-list contents and the meaning of the query's result.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" void* norrath_host_function(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#define NORRATH_WORD_TO_FUNCTION(word) norrath_host_function(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#define NORRATH_WORD_TO_FUNCTION(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void Transform__C6VIBBoxRC10VIMatrix44R6VIBBox(const unsigned char* self, const unsigned char* matrix, float* result);
int Query__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer(unsigned char* self, const unsigned char* box, const float* local, unsigned char* list, unsigned char* buffer);

int QueryObj__9VICollideRC6VIBBoxR14VIQueryTriListi(unsigned char* self, const unsigned char* box, unsigned char* list, int index) {
    unsigned char* buffer = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 536)) + index * 12));
    float local[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    Transform__C6VIBBoxRC10VIMatrix44R6VIBBox(box, self + 80, local);
    return Query__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer(self, box, local, list, buffer);
}
}
