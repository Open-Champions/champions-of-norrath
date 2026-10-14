// Observed: GetSpriteType__7VIScenei at 0x01144b88, 32 bytes. Arguments: the scene and a sprite index. It calls Sprite__7VIScenei(scene, index)
// and returns the 32-bit word at offset 52 of the sprite it gets back, with no null test.
// Inferred: the word at +52 of a sprite object is a type tag the scene reports to callers.
// Unknown: the tag values.
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

int GetSpriteType__7VIScenei(unsigned char* self, int index) {
    unsigned char* sprite = NORRATH_WORD_TO_POINTER(Sprite__7VIScenei(self, index));
    return *reinterpret_cast<int*>(sprite + 52);
}
}
