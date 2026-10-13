// Observed: Clear__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry at 0x0120a530, 116 bytes. Offsets are bytes from the object.
//  1. When the word at +8 is nonzero: call DestroyPool__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry with the object; a negative result
//     returns -1 at once with nothing stored. Otherwise the word at +8 is read again and, if
//     still nonzero, passed to __builtin_vec_delete.
//  2. Then zero is stored at +0, +4 and +8, -1 at +16 and zero at +12, and the result is 0.
// Inferred: the word at +8 is the element storage of VIMap<unsigned, VIDictionary::VIDictEntry>; the template argument
// types come only from the mangled name.
extern "C" {
int DestroyPool__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* array);
void __builtin_vec_delete(void* pointer);

int Clear__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* array) {
    unsigned* words = static_cast<unsigned*>(array);
    if (words[2] != 0) {
        if (DestroyPool__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(array) < 0) return -1;
        if (words[2] != 0) __builtin_vec_delete(reinterpret_cast<void*>(static_cast<unsigned long>(words[2])));
    }
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
    words[4] = static_cast<unsigned>(-1);
    words[3] = 0;
    return 0;
}
}
