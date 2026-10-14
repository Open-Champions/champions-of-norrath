// Observed mceGetInfoApdx at 0x002e1668, 88 bytes (a local routine; it is the
// completion function sceMcGetInfo hands to sceSifCallRpc, with the 192-byte
// parameter block as its argument). The argument is read through the uncached
// alias (address OR 0x20000000). Three copies follow, each only when its
// destination word is non-null:
//   - word 0 of the block to the int whose address is in typeAddr (0x008e6b68);
//   - word 1 to the int at freeAddr (0x008e6b6c);
//   - word 36 (byte offset 144) to the int at formAddr (0x008e6b70).
// The pointer word is re-read for each test. Finally it tail-calls iSignalSema
// with the word at 0x00446078 (semaid_timerfunc), so it returns whatever that
// call returns; the caller does not use it.
// Unknown: why this completion signals that word, and what the block words are.
#ifndef W9_UNCACHED_MASK
#define W9_UNCACHED_MASK 0x20000000u
#endif
extern "C" {
extern int* norrath_libmc_info_type_pointer_word_view;
extern int* norrath_libmc_info_free_pointer_word_view;
extern int* norrath_libmc_info_format_pointer_word_view;
extern int semaid_timerfunc;
int iSignalSema(int semaphore);

void mceGetInfoApdx(void* argument) {
    const volatile int* const block = reinterpret_cast<const volatile int*>(
        reinterpret_cast<__UINTPTR_TYPE__>(argument) | W9_UNCACHED_MASK);
    if (norrath_libmc_info_type_pointer_word_view != 0)
        *norrath_libmc_info_type_pointer_word_view = block[0];
    if (norrath_libmc_info_free_pointer_word_view != 0)
        *norrath_libmc_info_free_pointer_word_view = block[1];
    if (norrath_libmc_info_format_pointer_word_view != 0)
        *norrath_libmc_info_format_pointer_word_view = block[36];
    iSignalSema(semaid_timerfunc);
}
}
