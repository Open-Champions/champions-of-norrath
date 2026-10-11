#include "storage.hpp"
// Observed rodata at 0x00463490, the second current-name comparison. The C string is 9 bytes; following padding before the next symbol is not included.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
const unsigned char norrath_loader_context_b_view[9] NORRATH_LOADED(".rodata") = {
    0x72, 0x75, 0x69, 0x6e, 0x69, 0x6e, 0x74, 0x31, 0x00
};
}
