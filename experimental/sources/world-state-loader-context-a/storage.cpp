#include "storage.hpp"
// Observed eight bytes at 0x00484030 in .sdata, compared with the current level name. The next symbol begins at the following byte.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
const unsigned char norrath_loader_context_a_view[8] NORRATH_LOADED(".sdata") = {
    0x67, 0x6f, 0x74, 0x68, 0x69, 0x63, 0x32, 0x00
};
}
