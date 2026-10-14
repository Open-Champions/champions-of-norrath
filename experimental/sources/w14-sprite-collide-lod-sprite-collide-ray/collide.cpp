// Observed: Collide__11VILODSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide at 0x01076940, 128 bytes. Arguments: the sprite (S), the ray, the scene filter,
// the scene (V) and the collide record. Sprite__7VIScenei(V, word at S+76) returns another sprite P. Its word at +60 is
// a table; the 8-byte entry at table+40 holds a signed halfword adjustment at +0 and a function word at +4. That function
// is called with (P + adjustment, ray, filter, scene, collide) and its result is returned. Nothing is stored.
// Inferred: the word at S+76 is the index of the sprite shown by this level-of-detail sprite, and the entry is one
// virtual slot (sixth of the table, from 40/8) whose name is unknown. Unknown: the layout of the table's other entries.
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

int Collide__11VILODSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide(unsigned char* self, unsigned char* shape, unsigned char* filter, unsigned char* scene, unsigned char* collide) {
    unsigned char* sprite = NORRATH_WORD_TO_POINTER(Sprite__7VIScenei(scene, *reinterpret_cast<int*>(self + 76)));
    unsigned char* entry = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(sprite + 60)) + 40;
    typedef int (*Slot)(unsigned char*, unsigned char*, unsigned char*, unsigned char*, unsigned char*);
    Slot function = reinterpret_cast<Slot>(NORRATH_WORD_TO_FUNCTION(*reinterpret_cast<unsigned*>(entry + 4)));
    return function(sprite + *reinterpret_cast<short*>(entry), shape, filter, scene, collide);
}
}
