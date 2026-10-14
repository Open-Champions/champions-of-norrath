#include "storage.hpp"
// Observed rodata at 0x00463470, the first lumpLoad name comparison. The C string is 13 bytes; the three following zeros are padding before the next literal and are not included.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
const unsigned char norrath_loader_name_a_view[13] NORRATH_LOADED(".rodata") = {
    0x76, 0x61, 0x6d, 0x70, 0x6c, 0x6f, 0x72, 0x64, 0x2e, 0x6c, 0x6d, 0x70, 0x00
};
}
