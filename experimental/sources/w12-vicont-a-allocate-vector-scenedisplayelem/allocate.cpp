// Observed: Allocate__t8VIVector1Z18VISceneDisplayElem at 0x01213cc0, 284 bytes. Offsets are bytes from the object; elements are 12 bytes.
//  1. Capacity is the word at +4; zero returns 0 at once.
//  2. When the storage word at +8 is zero: __builtin_vec_new(capacity * 12) is stored at +8 (before the
//     result is tested) and the result is 0, or -1 when the block is null.
//  3. Otherwise a new block is requested (null returns -1, nothing stored). For each of the first word[0]
//     elements the 12 bytes are copied to the same slot of the new block unless that address is null.
//     Then DestroyPool__t8VIVector1Z18VISceneDisplayElem(object) is called; a negative result returns -1. The old block, if the
//     word at +8 is still nonzero, goes to __builtin_vec_delete, the new block is stored at +8 and the result is 0.
// Inferred: +0 is the element count and +8 the element block, as in the pool instances. No free chain is
// built here. The new block is leaked on the early -1 return after DestroyPool.
extern "C" {
void* __builtin_vec_new(unsigned size);
void __builtin_vec_delete(void* pointer);
int DestroyPool__t8VIVector1Z18VISceneDisplayElem(void* object);

int Allocate__t8VIVector1Z18VISceneDisplayElem(void* object) {
    struct Element { unsigned a, b, c; };
    unsigned* w = static_cast<unsigned*>(object);
    int capacity = static_cast<int>(w[1]);
    if (capacity == 0) return 0;
    if (w[2] == 0) {
        void* block = __builtin_vec_new(capacity * 12);
        w[2] = static_cast<unsigned>(reinterpret_cast<unsigned long>(block));
        return block == 0 ? -1 : 0;
    }
    Element* fresh = static_cast<Element*>(__builtin_vec_new(capacity * 12));
    if (fresh == 0) return -1;
    for (int i = 0; i < static_cast<int>(w[0]); ++i) {
        Element* to = fresh + i;
        if (to != 0) *to = static_cast<Element*>(reinterpret_cast<void*>(static_cast<unsigned long>(w[2])))[i];
    }
    if (DestroyPool__t8VIVector1Z18VISceneDisplayElem(object) < 0) return -1;
    if (w[2] != 0) __builtin_vec_delete(reinterpret_cast<void*>(static_cast<unsigned long>(w[2])));
    w[2] = static_cast<unsigned>(reinterpret_cast<unsigned long>(fresh));
    return 0;
}
}
