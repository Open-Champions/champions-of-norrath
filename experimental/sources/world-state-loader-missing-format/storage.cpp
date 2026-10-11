#include "storage.hpp"
// Observed rodata at 0x00463458, the format passed to the missing-lump print. The C string is 22 bytes; two following zero bytes are padding and are not included.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
const unsigned char norrath_loader_missing_format_view[22] NORRATH_LOADED(".rodata") = {
    0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x6c, 0x75, 0x6d, 0x70, 0x20, 0x6e, 0x6f, 0x74, 0x20, 0x66, 0x6f, 0x75, 0x6e, 0x64, 0x0a, 0x00
};
}
