#include "storage.hpp"
// Observed rodata at 0x00463480, the second lumpLoad name comparison. The C string is 13 bytes; following padding is not included.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
const unsigned char norrath_loader_name_b_view[13] NORRATH_LOADED(".rodata") = {
    0x73, 0x75, 0x70, 0x65, 0x72, 0x6f, 0x72, 0x63, 0x2e, 0x6c, 0x6d, 0x70, 0x00
};
}
