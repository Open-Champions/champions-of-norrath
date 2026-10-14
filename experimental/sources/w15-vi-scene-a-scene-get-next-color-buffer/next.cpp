// Observed: GetNextColorBuffer__7VIScene at 0x01145158, 112 bytes. Arguments: the scene (S). Word S+1092 holds a current static-lighting index C;
// if C is -1 the function returns -1 and changes nothing. Otherwise it takes the static-lighting object at index C of the pool whose record
// array pointer is at S+1076 (12-byte records, first word the object), increments the counter word at S+1096 (stored before it is tested)
// to K, and looks at the array header at object+8: a 32-bit count at +0 and a pointer to 32-bit values at +4. When K is below the count it
// returns the value at index K, otherwise it returns -1. The counter keeps its incremented value in both cases.
// Inferred: S+1068 is the static-lighting pool (ReleaseStaticLighting__7VIScenei uses that base); S+1092/S+1096 are a cursor over the
// colour buffers of the current static lighting, so each call hands out the next one.
// Unknown: whether anything resets the counter to zero (no reset in this function), and the element type of the array.
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
int GetNextColorBuffer__7VIScene(unsigned char* self) {
    int current = *reinterpret_cast<int*>(self + 1092);
    if (current == -1) return -1;
    unsigned char* records = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 1076));
    int next = *reinterpret_cast<int*>(self + 1096) + 1;
    unsigned char* object = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(records + current * 12));
    *reinterpret_cast<int*>(self + 1096) = next;
    unsigned char* array = object + 8;
    if (next < *reinterpret_cast<int*>(array)) {
        unsigned char* values = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(array + 4));
        return *reinterpret_cast<int*>(values + next * 4);
    }
    return -1;
}
}
