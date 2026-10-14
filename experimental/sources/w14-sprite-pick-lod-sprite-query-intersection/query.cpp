// Observed: QueryIntersection__11VILODSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene at 0x010769c0, 128 bytes. Arguments: the sprite (S), the box, the triangle list, the matrix and the
// scene (V). Sprite__7VIScenei(V, word at S+76) returns another sprite P. Its word at +60 is a table; the 8-byte entry at
// table+64 holds a signed halfword adjustment at +0 and a function word at +4. That function is called with
// (P + adjustment, box, list, matrix, V) and its result is returned. Nothing is stored.
// Inferred: the word at S+76 is the index of the first sprite this level-of-detail sprite shows, and the entry is the
// ninth slot of the table (64/8) whose name is unknown. Unknown: the layout of the table's other entries.
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
unsigned Sprite__7VIScenei(unsigned char* scene, int index);

int QueryIntersection__11VILODSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene(unsigned char* self, unsigned char* box, unsigned char* list, unsigned char* matrix, unsigned char* scene) {
    unsigned char* sprite = NORRATH_WORD_TO_POINTER(Sprite__7VIScenei(scene, *reinterpret_cast<int*>(self + 76)));
    unsigned char* entry = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(sprite + 60)) + 64;
    typedef int (*Slot)(unsigned char*, unsigned char*, unsigned char*, unsigned char*, unsigned char*);
    Slot function = reinterpret_cast<Slot>(NORRATH_WORD_TO_FUNCTION(*reinterpret_cast<unsigned*>(entry + 4)));
    return function(sprite + *reinterpret_cast<short*>(entry), box, list, matrix, scene);
}
}
