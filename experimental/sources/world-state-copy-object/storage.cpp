#include "storage.hpp"
// Observed copyObject at 0x0048451c: four file-backed zero bytes in .sdata. listSetCopyObject stores A0 in this word.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned char norrath_list_copy_object_word[4] NORRATH_LOADED(".sdata") = {
    0x00, 0x00, 0x00, 0x00
};
}
