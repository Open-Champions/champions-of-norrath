// Observed: Clear__t8VIVector1Z18VISceneDisplayElem at 0x01224ed8, 104 bytes. Offsets are bytes from the object.
//  1. When the word at +8 is nonzero: call DestroyPool__t8VIVector1Z18VISceneDisplayElem with the object; a negative result
//     returns -1 at once with nothing stored. Otherwise the word at +8 is read again and, if
//     still nonzero, passed to __builtin_vec_delete.
//  2. Then zero is stored at +8, +0 and +4, and the result is 0.
// Inferred: the word at +8 is the element storage of VIVector<VISceneDisplayElem>; the template argument
// types come only from the mangled name.
extern "C" {
int DestroyPool__t8VIVector1Z18VISceneDisplayElem(void* array);
void __builtin_vec_delete(void* pointer);

int Clear__t8VIVector1Z18VISceneDisplayElem(void* array) {
    unsigned* words = static_cast<unsigned*>(array);
    if (words[2] != 0) {
        if (DestroyPool__t8VIVector1Z18VISceneDisplayElem(array) < 0) return -1;
        if (words[2] != 0) __builtin_vec_delete(reinterpret_cast<void*>(static_cast<unsigned long>(words[2])));
    }
    words[2] = 0;
    words[0] = 0;
    words[1] = 0;
    return 0;
}
}
