// Observed: Init__t8VIVector1Z10VIMatrix44RCt8VIVector1Z10VIMatrix44 at 0x01201fd8 (the symbol size says 168 bytes; the code runs to
// 0x12020b7, 224 bytes). Arguments (vector, source). The vector object is a count at +0, a capacity at +4 and an element pointer at +8.
// When the element pointer is non-zero it calls DestroyPool__t8VIVector1Z10VIMatrix44(vector) (a negative result returns -1) and passes
// the pointer to __builtin_vec_delete when it is still non-zero; it then stores zero at +0, +4 and +8. It copies the source capacity to
// vector+4 and calls Allocate__t8VIVector1Z10VIMatrix44(vector) (a negative result returns -1). Then for i from 0 while i < the source
// count it takes the 64-byte slot at the vector's element pointer (read afresh every pass) + i*64 and, when that address is non-zero,
// calls __10VIMatrix44RC10VIMatrix44(slot, source element pointer (read afresh) + i*64) and re-reads the source count. After the loop
// it stores the source count at vector+0 and returns zero. A source count of zero or less skips the loop but the count is still
// stored. A zero slot address skips the call and the count re-read.
// Inferred: the copy-initialiser of a counted vector of VIMatrix44 elements, each copy-constructed in place; the null test is a
// placement-construction check.
// Unknown: the matrix class's members beyond its copy constructor, and the pool helpers' behaviour beyond the calls seen here.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int DestroyPool__t8VIVector1Z10VIMatrix44(void* vector);
int Allocate__t8VIVector1Z10VIMatrix44(void* vector);
void __builtin_vec_delete(void* pointer);
void* __10VIMatrix44RC10VIMatrix44(void* destination, const void* source);

int Init__t8VIVector1Z10VIMatrix44RCt8VIVector1Z10VIMatrix44(void* vector, const void* source) {
    unsigned* words = static_cast<unsigned*>(vector);
    const unsigned* from = static_cast<const unsigned*>(source);
    if (words[2] != 0) {
        if (DestroyPool__t8VIVector1Z10VIMatrix44(vector) < 0) return -1;
        if (words[2] != 0) __builtin_vec_delete(NORRATH_WORD_TO_POINTER(words[2]));
    }
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
    words[1] = from[1];
    if (Allocate__t8VIVector1Z10VIMatrix44(vector) < 0) return -1;
    int count = static_cast<int>(from[0]);
    for (int i = 0; i < count; ++i) {
        unsigned char* slot = NORRATH_WORD_TO_POINTER(words[2]) + i * 64;
        if (slot) {
            __10VIMatrix44RC10VIMatrix44(slot, NORRATH_WORD_TO_POINTER(from[2]) + i * 64);
            count = static_cast<int>(from[0]);
        }
    }
    words[0] = count;
    return 0;
}
}
