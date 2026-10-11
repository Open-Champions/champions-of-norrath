#include "storage.hpp"
// Observed userFilterFunction at 0x00483b00: four file-backed zero bytes in .sdata. objectSetFilterFunction stores A0 here.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned char norrath_restore_filter_word[4] NORRATH_LOADED(".sdata") = {
    0x00, 0x00, 0x00, 0x00
};
}
