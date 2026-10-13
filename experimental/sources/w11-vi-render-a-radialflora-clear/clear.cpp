// Observed: Clear__19VIRadialFloraSystem at 0x010d5bf8 is 128 bytes. It stores zero
// in the words at offsets 0, 4 and 8. For each of the nine pointer words at
// offsets 32, 36, ... 64, in order, a nonzero pointer p has
// _$_t7VIArray1Z17VIRadialFloraInst called on p+44 with flags 2, then
// __builtin_delete(p), and the word is set to zero. Zero words are skipped. It then
// stores zero at 68 and returns 0.
// Inferred: the nine words are heap blocks, each holding an instance array at +44.
extern "C" {
void _$_t7VIArray1Z17VIRadialFloraInst(void* array, int flags);
void __builtin_delete(void* pointer);

int Clear__19VIRadialFloraSystem(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<int*>(bytes + 0) = 0;
    *reinterpret_cast<int*>(bytes + 4) = 0;
    *reinterpret_cast<int*>(bytes + 8) = 0;
    for (int i = 0; i < 9; ++i) {
        unsigned int& slot = *reinterpret_cast<unsigned int*>(bytes + 32 + 4 * i);
        unsigned char* block = reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(slot));
        if (block != 0) {
            _$_t7VIArray1Z17VIRadialFloraInst(block + 44, 2);
            __builtin_delete(block);
            slot = 0;
        }
    }
    *reinterpret_cast<int*>(bytes + 68) = 0;
    return 0;
}
}
