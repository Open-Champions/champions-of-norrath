#include "storage.hpp"
// Observed poolOffset at 0x004868a4 in .sbss. objectPutSaveData stores the string base here. The ELF size field is 0; the next symbol begins four bytes later, so the extent is inferred as one word.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned char norrath_restore_string_base_word[4] NORRATH_LOADED(".sbss") = {
    0x00, 0x00, 0x00, 0x00
};
}
