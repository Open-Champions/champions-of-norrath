// Observed: Allocate__t7VIArray1Zi at 0x011f5168, 120 bytes. Offsets are bytes from the array.
//  1. The element count is the word at +0; zero returns 0 at once.
//  2. Otherwise __builtin_vec_new(count * 4) is stored at +4 (before the result is tested); a null
//     result returns -1. Then the count is read again and a counting loop of that many empty iterations
//     runs (nothing is stored or called); the result is 0.
// Inferred: for int there is no per-element construction, so the loop is a leftover of the generic
// element-construction loop and has no visible effect; it is not reproduced here.
extern "C" {
void* __builtin_vec_new(unsigned size);

int Allocate__t7VIArray1Zi(void* array) {
    unsigned* w = static_cast<unsigned*>(array);
    if (w[0] == 0) return 0;
    void* block = __builtin_vec_new(w[0] * 4);
    w[1] = static_cast<unsigned>(reinterpret_cast<unsigned long>(block));
    return block == 0 ? -1 : 0;
}
}
