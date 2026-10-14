// Observed: Clear__t8VIVector1Z13VIHSpriteNode at 0x012034c0, 104 bytes. Offsets are bytes from the object.
//  1. When the word at +8 is nonzero: call DestroyPool__t8VIVector1Z13VIHSpriteNode with the object; a negative result
//     returns -1 at once with nothing stored. Otherwise the word at +8 is read again and, if
//     still nonzero, passed to __builtin_vec_delete.
//  2. Then the words at +8, +0 and +4 are set to zero (also when the first step was skipped), and the result is 0.
// Inferred: the word at +8 is the element storage of this container instance; the template argument
// types come only from the mangled name.
// Unknown: the meaning of the word at +4 beyond its reset to zero.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline void* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline void* at(unsigned address) { return reinterpret_cast<void*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int DestroyPool__t8VIVector1Z13VIHSpriteNode(void* object);
void __builtin_vec_delete(void* pointer);

int Clear__t8VIVector1Z13VIHSpriteNode(void* object) {
    unsigned* words = static_cast<unsigned*>(object);
    if (words[2] != 0) {
        if (DestroyPool__t8VIVector1Z13VIHSpriteNode(object) < 0) return -1;
        if (words[2] != 0) __builtin_vec_delete(at(words[2]));
    }
    words[2] = 0;
    words[0] = 0;
    words[1] = 0;
    return 0;
}
}
