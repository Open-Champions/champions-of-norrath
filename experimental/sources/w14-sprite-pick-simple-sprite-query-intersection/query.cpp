// Observed: QueryIntersection__14VISimpleSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene at 0x01167138, 96 bytes. Arguments: the sprite (S), the box (B), the triangle list (L), the matrix (M)
// and the scene (V). Reads the collide object word at V+19364, calls SetWorld__9VICollideRC10VIMatrix44(C, M) and then
// tail-calls QueryObj__9VICollideRC6VIBBoxR14VIQueryTriListi(C, B, L, word at S+68), returning its result.
// Inferred: the word at S+68 identifies the object queried, the same word the collide virtuals of this sprite use.
// Unknown: its name and the meaning of the word at V+19364.
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
void SetWorld__9VICollideRC10VIMatrix44(unsigned char* collide, unsigned char* matrix);
int QueryObj__9VICollideRC6VIBBoxR14VIQueryTriListi(unsigned char* collide, unsigned char* box, unsigned char* list, int object);

int QueryIntersection__14VISimpleSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene(unsigned char* self, unsigned char* box, unsigned char* list, unsigned char* matrix, unsigned char* scene) {
    unsigned char* collide = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(scene + 19364));
    SetWorld__9VICollideRC10VIMatrix44(collide, matrix);
    return QueryObj__9VICollideRC6VIBBoxR14VIQueryTriListi(collide, box, list, *reinterpret_cast<int*>(self + 68));
}
}
