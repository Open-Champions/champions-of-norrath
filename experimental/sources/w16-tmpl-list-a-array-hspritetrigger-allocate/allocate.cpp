// Observed: Allocate__t7VIArray1Z16VIHSpriteTrigger at 0x01203a00 is 120 bytes. Offsets are bytes
// from the array.
//  1. The element count is the word at +0; zero returns 0 at once and leaves +4 alone.
//  2. Otherwise __builtin_vec_new(count * 16) is stored at +4 before the result is tested; a null
//     result returns -1.
//  3. Then for each index below the count (the count word is read again after every element) the
//     16-byte element at +4 plus 16 * index gets four zero words. The result is 0.
// Inferred: the elements are 16 bytes each and their construction is plain zero-fill.
// Unknown: the element layout beyond that, and the template parameters beyond the mangled name.
extern "C" {
void* __builtin_vec_new(unsigned size);

int Allocate__t7VIArray1Z16VIHSpriteTrigger(void* array) {
    unsigned* w = static_cast<unsigned*>(array);
    if (w[0] == 0) return 0;
    void* block = __builtin_vec_new(w[0] << 4);
    w[1] = static_cast<unsigned>(reinterpret_cast<unsigned long>(block));
    if (block == 0) return -1;
    unsigned* element = static_cast<unsigned*>(block);
    for (int index = 0; index < static_cast<int>(w[0]); ++index) {
        element[0] = 0;
        element[1] = 0;
        element[2] = 0;
        element[3] = 0;
        element += 4;
    }
    return 0;
}
}
