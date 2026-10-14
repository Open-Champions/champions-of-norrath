#include "storage.hpp"
// Observed objectIgnoreFlags at 0x00483ac0: four file-backed zero bytes in .sdata. The grid box scan reloads this mask, and objectPutSaveData passes the same word to the broadcast iterator.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned char norrath_grid_ignore_flags_view[4] NORRATH_LOADED(".sdata") = {
    0x00, 0x00, 0x00, 0x00
};
}
