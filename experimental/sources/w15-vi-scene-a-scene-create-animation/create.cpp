// Observed: CreateAnimation__7VIScenei at 0x01144d78, 132 bytes. Arguments: the scene (S) and a flag (K). It allocates 56 bytes with
// __builtin_new, runs the constructor __13VIHSpriteAnim on the block when the allocation is nonnull, and calls
// Add__t6VIPool1ZP13VIHSpriteAnimRCP13VIHSpriteAnim(S+1020, &block). A negative result returns -1. Otherwise, when K is nonzero it increments the
// word at +4 of the block (read back from the stack slot after the call), and it returns the pool's result.
// Inferred: S+1020 is the scene's animation pool, the result is the new animation's index, and the word at +4 is a reference count that
// K asks for (ReleaseAnimation__7VIScenei decrements it). A failed allocation still goes through Add with a null pointer.
// Unknown: what Add does with a null element and the constructor's argument list.
extern "C" {
unsigned __builtin_new(unsigned size);
void __13VIHSpriteAnim(unsigned char* animation);
int Add__t6VIPool1ZP13VIHSpriteAnimRCP13VIHSpriteAnim(unsigned char* pool, unsigned* element);
}
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
int CreateAnimation__7VIScenei(unsigned char* self, int addReference) {
    unsigned animation = __builtin_new(56);
    if (animation != 0) __13VIHSpriteAnim(NORRATH_WORD_TO_POINTER(animation));
    int index = Add__t6VIPool1ZP13VIHSpriteAnimRCP13VIHSpriteAnim(self + 1020, &animation);
    if (index < 0) return -1;
    if (addReference != 0) *reinterpret_cast<int*>(NORRATH_WORD_TO_POINTER(animation) + 4) += 1;
    return index;
}
}
