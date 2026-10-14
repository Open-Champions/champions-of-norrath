// Observed: CreateRefMap__7VIScenei at 0x01144f00 is 164 bytes in the pinned SLUS-20565 image. It allocates 28 bytes with __builtin_new.
// If the allocation succeeds it zeroes the words at +0 and +4 and runs the constructor __t5VIMap2ZiZi on the address +8 of the block;
// otherwise the block pointer stays null. A null block returns -1. Otherwise it keeps the block pointer in a stack word and calls
// Add__t6VIPool1ZP8VIRefMapRCP8VIRefMap(this + 1044, &that word); a negative result returns -1 (the block is not freed). On success, if the int
// argument is non-zero, the word at +4 of the block (re-read from the stack word) is incremented. It returns Add's result (not
// necessarily zero).
// Inferred: the block is a VIRefMap (a reference count at +4 and a VIMap<int,int> at +8, from the mangled names), and the scene's pool of
// them is the member at +1044; the int argument asks for the new map to start with one reference. +0 is not named by anything here.
// Unknown: what +0 holds, the pool's capacity and the result of Add (an import) beyond "non-negative on success". Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
unsigned __builtin_new(unsigned size);
void __t5VIMap2ZiZi(unsigned char* map);
int Add__t6VIPool1ZP8VIRefMapRCP8VIRefMap(unsigned char* pool, const unsigned* entry);

int CreateRefMap__7VIScenei(unsigned char* self, int addReference) {
    unsigned block = __builtin_new(28);
    if (block != 0) {
        unsigned char* map = NORRATH_WORD_TO_POINTER(block);
        *reinterpret_cast<unsigned*>(map + 4) = 0;
        *reinterpret_cast<unsigned*>(map) = 0;
        __t5VIMap2ZiZi(map + 8);
    }
    unsigned ref = block;
    if (ref == 0) return -1;
    int index = Add__t6VIPool1ZP8VIRefMapRCP8VIRefMap(self + 1044, &ref);
    if (index < 0) return -1;
    if (addReference != 0) {
        unsigned char* map = NORRATH_WORD_TO_POINTER(ref);
        *reinterpret_cast<int*>(map + 4) += 1;
    }
    return index;
}
}
