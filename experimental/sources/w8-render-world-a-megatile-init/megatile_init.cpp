// Observed megatileInit__FP12MegatileRootPv at 0x0025e0e0, 708 bytes. The first
// argument is a megatile root record and the second is a relocation base that is
// added, as an integer, to the stored offsets in the record. Offsets below are
// from the root address.
//  1. The global word mgrg (0x00485c54) is set to the root. The pointers at +32
//     and +40 get the base added.
//  2. For each of the `[+28]` records of 12 bytes at [+32], and then each of the
//     `[+36]` records of 204 bytes at [+40], megatileBankInit__FP16MegatileInstBankPv
//     and megatileItemInit__FP8MegatilePv are called with the record and the base.
//     The counts are read again after every call.
//  3. A nonzero word at +52 gets the base added. For a halfword at +48 equal to
//     1, 2 or 3, the words at +12 + 8 i and +276 + 8 i of the block at [+52] get
//     the base added, for i from 0 to 32.
//  4. The word at +56 chooses: 1 adds the base to the word at +84; 2 adds it to the
//     [+84] words from +88, then runs an empty doubly nested loop of [+84] by
//     [+84] steps with no effect; anything else does nothing.
//  5. A word of 1 at +760 adds the base to the [+764] words from +768.
//  6. Using mgrg again (read after the calls): while the byte at +784 of the first
//     of up to eight 16-byte entries from +768 is nonzero, an entry whose byte
//     at +16 is 1 gets the base added to its word at +20, and every entry gets
//     it added to the words at +24 and +28.
//  7. mtcInitialize__Fv is called.
// Inferred: this relocates a loaded megatile block in place; mgrg is the current
// root. The empty nested loop is left from a removed check, as in lumpCheck__Fv.
// Unknown: every field name, and the meaning of the halfword at +48.
#ifdef NORRATH_W8_HOST
extern unsigned norrath_w8_pointer_to_word(const void* pointer);
extern void* norrath_w8_word_to_pointer(unsigned word);
#define NORRATH_W8_WORD(pointer) norrath_w8_pointer_to_word(pointer)
#define NORRATH_W8_POINTER(word) norrath_w8_word_to_pointer(word)
extern unsigned norrath_w8_delay_steps;
#define NORRATH_W8_DELAY_STEP() (++norrath_w8_delay_steps)
#else
#define NORRATH_W8_WORD(pointer) static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer))
#define NORRATH_W8_POINTER(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#define NORRATH_W8_DELAY_STEP() __asm__ volatile("")
#endif

template <class T>
static inline T& field(void* object, unsigned offset) {
    return *reinterpret_cast<T*>(static_cast<char*>(object) + offset);
}

extern "C" {
extern void* mgrg;
void megatileBankInit__FP16MegatileInstBankPv(void* bank, void* base);
void megatileItemInit__FP8MegatilePv(void* item, void* base);
void mtcInitialize__Fv();

void megatileInit__FP12MegatileRootPv(void* root, void* base) {
    const unsigned relocation = NORRATH_W8_WORD(base);
    const unsigned banks = field<unsigned>(root, 32);
    const unsigned items = field<unsigned>(root, 40);
    mgrg = root;
    field<unsigned>(root, 32) = banks + relocation;
    field<unsigned>(root, 40) = items + relocation;
    for (int index = 0; index < field<int>(root, 28); ++index) {
        megatileBankInit__FP16MegatileInstBankPv(
            static_cast<char*>(NORRATH_W8_POINTER(field<unsigned>(root, 32))) + index * 12, base);
    }
    for (int index = 0; index < field<int>(root, 36); ++index) {
        megatileItemInit__FP8MegatilePv(
            static_cast<char*>(NORRATH_W8_POINTER(field<unsigned>(root, 40))) + index * 204, base);
    }
    if (field<unsigned>(root, 52) != 0u) field<unsigned>(root, 52) += relocation;
    const short mode = field<short>(root, 48);
    if (mode != 0 && mode >= 0 && mode < 4) {
        char* const block = static_cast<char*>(NORRATH_W8_POINTER(field<unsigned>(root, 52)));
        for (int index = 0; index < 33; ++index) {
            *reinterpret_cast<unsigned*>(block + 12 + 8 * index) += relocation;
            *reinterpret_cast<unsigned*>(block + 276 + 8 * index) += relocation;
        }
    }
    const int kind = field<int>(root, 56);
    if (kind == 1) {
        field<unsigned>(root, 84) += relocation;
    } else if (kind == 2) {
        int count = field<int>(root, 84);
        for (int index = 0; index < field<int>(root, 84); ++index) field<unsigned>(root, 88 + 4 * index) += relocation;
        if (count > 0) count = field<int>(root, 84);
        if (count > 0) {
            for (int outer = 0; outer < count; ++outer) {
                for (int inner = count; inner > 0; --inner) NORRATH_W8_DELAY_STEP();
            }
        }
    }
    if (field<int>(root, 760) == 1) {
        for (int index = 0; index < field<int>(root, 764); ++index) field<unsigned>(root, 768 + 4 * index) += relocation;
    }
    void* const current = mgrg;
    if (field<signed char>(current, 784) != 0) {
        int entry = 0;
        do {
            const unsigned offset = 768 + 16 * entry;
            if (field<signed char>(current, offset + 16) == 1) field<unsigned>(current, offset + 20) += relocation;
            field<unsigned>(current, offset + 24) += relocation;
            field<unsigned>(current, offset + 28) += relocation;
            ++entry;
        } while (entry < 8 && field<signed char>(current, 768 + 16 * entry + 16) != 0);
    }
    mtcInitialize__Fv();
}
}
