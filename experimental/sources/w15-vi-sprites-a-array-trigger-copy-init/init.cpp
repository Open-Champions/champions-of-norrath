// Observed: Init__t7VIArray1Z16VIHSpriteTriggerRCt7VIArray1Z16VIHSpriteTrigger at 0x012020b8, 212 bytes. Arguments (array, source). The
// array object is a count at +0 and an element pointer at +4. When the element pointer is non-zero it calls
// DestroyPool__t7VIArray1Z16VIHSpriteTrigger(array) (a negative result returns -1) and passes the pointer to __builtin_vec_delete when it
// is still non-zero. It then stores zero at +0 and +4. It copies the source count to array+0 and calls
// Allocate__t7VIArray1Z16VIHSpriteTrigger(array) (a negative result returns -1). Then for i from 0 while i < the source count (re-read
// each pass) it takes the 16-byte slot array.data + i*16 and, when that address is non-zero, copies the 16 bytes from source.data + i*16
// as two 64-bit words. It returns zero.
// Inferred: this is the copy-initialiser of a counted array of 16-byte plain records; the zero-address test is the null check of a
// placement construction. DestroyPool and Allocate are the container's own helpers.
// Unknown: the layout of the 16-byte trigger record, and whether the source count is ever negative (a count of zero or less copies nothing).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int DestroyPool__t7VIArray1Z16VIHSpriteTrigger(void* array);
int Allocate__t7VIArray1Z16VIHSpriteTrigger(void* array);
void __builtin_vec_delete(void* pointer);

int Init__t7VIArray1Z16VIHSpriteTriggerRCt7VIArray1Z16VIHSpriteTrigger(void* array, const void* source) {
    unsigned* words = static_cast<unsigned*>(array);
    const unsigned* from = static_cast<const unsigned*>(source);
    if (words[1] != 0) {
        if (DestroyPool__t7VIArray1Z16VIHSpriteTrigger(array) < 0) return -1;
        if (words[1] != 0) __builtin_vec_delete(NORRATH_WORD_TO_POINTER(words[1]));
    }
    words[0] = 0;
    words[1] = 0;
    words[0] = from[0];
    if (Allocate__t7VIArray1Z16VIHSpriteTrigger(array) < 0) return -1;
    for (int i = 0; i < static_cast<int>(from[0]); ++i) {
        unsigned char* destination = NORRATH_WORD_TO_POINTER(words[1]) + i * 16;
        if (destination) {
            const unsigned long long* sourceRecord = reinterpret_cast<const unsigned long long*>(NORRATH_WORD_TO_POINTER(from[1]) + i * 16);
            reinterpret_cast<unsigned long long*>(destination)[0] = sourceRecord[0];
            reinterpret_cast<unsigned long long*>(destination)[1] = sourceRecord[1];
        }
    }
    return 0;
}
}
