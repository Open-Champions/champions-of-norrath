// Observed: Allocate__t7VIArray1Z20VICSpritePartEmitter at 0x01208328 is 212 bytes. Offsets are
// bytes from the array unless stated.
//  1. The element count is the word at +0; zero returns 0 at once and leaves +4 alone.
//  2. Otherwise __builtin_vec_new(count * 1056) is stored at +4 before the result is tested; a
//     null result returns -1.
//  3. Then for each index below the count (the count word is read again after every element) the
//     1056-byte element is built: __17VIParticleEmitter is called with the address element+16,
//     and afterwards these element offsets are stored: +1040 as a doubleword zero, +0 = -1,
//     +1012 = 0, +1016 = -1, +1028 = 0, +1032 = 0, +1008 = 0, +1020 = 0, +1024 = 0.
//     The result is 0.
// Inferred: each element embeds a VIParticleEmitter at +16 and the stores are the rest of the
// element's constructor (no reads of the stored fields happen here).
// Unknown: the meaning of the individual fields, and the template parameters beyond the name.
extern "C" {
void* __builtin_vec_new(unsigned size);
void __17VIParticleEmitter(void* self);

int Allocate__t7VIArray1Z20VICSpritePartEmitter(void* array) {
    unsigned* w = static_cast<unsigned*>(array);
    if (w[0] == 0) return 0;
    void* block = __builtin_vec_new(w[0] * 1056);
    w[1] = static_cast<unsigned>(reinterpret_cast<unsigned long>(block));
    if (block == 0) return -1;
    for (int index = 0; index < static_cast<int>(w[0]); ++index) {
        unsigned char* element = reinterpret_cast<unsigned char*>(
            static_cast<__UINTPTR_TYPE__>(w[1])) + index * 1056;
        __17VIParticleEmitter(element + 16);
        *reinterpret_cast<unsigned long long*>(element + 1040) = 0;
        *reinterpret_cast<unsigned*>(element + 0) = 0xffffffffu;
        *reinterpret_cast<unsigned*>(element + 1012) = 0;
        *reinterpret_cast<unsigned*>(element + 1016) = 0xffffffffu;
        *reinterpret_cast<unsigned*>(element + 1028) = 0;
        *reinterpret_cast<unsigned*>(element + 1032) = 0;
        *reinterpret_cast<unsigned*>(element + 1008) = 0;
        *reinterpret_cast<unsigned*>(element + 1020) = 0;
        *reinterpret_cast<unsigned*>(element + 1024) = 0;
    }
    return 0;
}
}
