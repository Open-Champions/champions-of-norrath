// Observed: Init__t8VIVector1Z13VIHSpriteNodeRCt8VIVector1Z13VIHSpriteNode at 0x01201da8 (the symbol size says 504 bytes; the code runs to
// 0x1201fd7). Arguments (vector, source). The vector object is a count at +0, a capacity at +4 and an element pointer at +8. When the
// element pointer is non-zero it calls DestroyPool__t8VIVector1Z13VIHSpriteNode(vector) (a negative result returns -1) and passes the
// pointer to __builtin_vec_delete when it is still non-zero; it then stores zero at +0, +4 and +8. It copies the source capacity to
// vector+4 and calls Allocate__t8VIVector1Z13VIHSpriteNode(vector) (a negative result returns -1). Then for i from 0 while i < the source
// count (re-read each pass) it takes the 256-byte slot data + i*256 and, when that address is non-zero, copies from source.data + i*256:
// four words at +0..+12; floats at +16..+32 (five), +36..+44 (three), +112..+120 (three), +124..+140 (five), +144..+152 (three),
// +224..+240 (five) and +244..+252 (three); and, through __10VIMatrix44RC10VIMatrix44 (copy construction), the matrices at +48 and +160.
// Bytes +156..+159 are not copied. After the loop it stores the source count at vector+0 and returns zero. A source count of zero or less
// skips the loop but the count is still stored.
// Inferred: this is the copy-initialiser of a counted vector of 256-byte node records; the null test is a placement-construction check, and
// the field-by-field copy is an inlined member-wise copy of a node (the two matrices at +48 and +160 are 64-byte VIMatrix44 members).
// Unknown: the node's field names, what lives at +156, and the behaviour of the pool helpers beyond the calls seen here.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int DestroyPool__t8VIVector1Z13VIHSpriteNode(void* vector);
int Allocate__t8VIVector1Z13VIHSpriteNode(void* vector);
void __builtin_vec_delete(void* pointer);
void* __10VIMatrix44RC10VIMatrix44(void* destination, const void* source);

static inline void copyFloats(unsigned char* to, const unsigned char* from, int offset, int count) {
    for (int i = 0; i < count; ++i) *reinterpret_cast<float*>(to + offset + i * 4) = *reinterpret_cast<const float*>(from + offset + i * 4);
}

int Init__t8VIVector1Z13VIHSpriteNodeRCt8VIVector1Z13VIHSpriteNode(void* vector, const void* source) {
    unsigned* words = static_cast<unsigned*>(vector);
    const unsigned* from = static_cast<const unsigned*>(source);
    if (words[2] != 0) {
        if (DestroyPool__t8VIVector1Z13VIHSpriteNode(vector) < 0) return -1;
        if (words[2] != 0) __builtin_vec_delete(NORRATH_WORD_TO_POINTER(words[2]));
    }
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
    words[1] = from[1];
    if (Allocate__t8VIVector1Z13VIHSpriteNode(vector) < 0) return -1;
    int count = static_cast<int>(from[0]);
    for (int i = 0; i < count; ++i) {
        unsigned char* node = NORRATH_WORD_TO_POINTER(words[2]) + i * 256;
        if (node) {
            const unsigned char* sourceNode = NORRATH_WORD_TO_POINTER(from[2]) + i * 256;
            for (int k = 0; k < 4; ++k) *reinterpret_cast<unsigned*>(node + k * 4) = *reinterpret_cast<const unsigned*>(sourceNode + k * 4);
            copyFloats(node, sourceNode, 16, 5);
            copyFloats(node, sourceNode, 36, 3);
            __10VIMatrix44RC10VIMatrix44(node + 48, sourceNode + 48);
            copyFloats(node, sourceNode, 112, 3);
            copyFloats(node, sourceNode, 124, 5);
            copyFloats(node, sourceNode, 144, 3);
            __10VIMatrix44RC10VIMatrix44(node + 160, sourceNode + 160);
            copyFloats(node, sourceNode, 224, 5);
            copyFloats(node, sourceNode, 244, 3);
            count = static_cast<int>(from[0]);
        }
    }
    words[0] = count;
    return 0;
}
}
