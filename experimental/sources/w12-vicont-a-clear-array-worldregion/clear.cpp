// Observed: Clear__t7VIArray1Z13VIWorldRegion at 0x0120fbd0, 100 bytes. Offsets are bytes from the object.
//  1. When the word at +4 is nonzero: call DestroyPool__t7VIArray1Z13VIWorldRegion with the object; a negative result
//     returns -1 at once with nothing stored. Otherwise the word at +4 is read again and, if
//     still nonzero, passed to __builtin_vec_delete.
//  2. Then the words at +4 and +0 are set to zero, and the result is 0.
// Inferred: the word at +4 is the element storage of this VIArray instance; the template argument
// types come only from the mangled name. The body is the same as the VIArray<int> instance.
extern "C" {
int DestroyPool__t7VIArray1Z13VIWorldRegion(void* array);
void __builtin_vec_delete(void* pointer);

int Clear__t7VIArray1Z13VIWorldRegion(void* array) {
    unsigned* words = static_cast<unsigned*>(array);
    if (words[1] != 0) {
        if (DestroyPool__t7VIArray1Z13VIWorldRegion(array) < 0) return -1;
        if (words[1] != 0) __builtin_vec_delete(reinterpret_cast<void*>(static_cast<unsigned long>(words[1])));
    }
    words[1] = 0;
    words[0] = 0;
    return 0;
}
}
