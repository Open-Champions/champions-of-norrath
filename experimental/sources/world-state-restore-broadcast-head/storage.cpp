#include "storage.hpp"
// Observed deleteList at 0x00486880 in .sbss. objectPutSaveData passes its address to the safe iterator. The ELF size field is 0; alwaysDrawList begins eight bytes later, so the extent is inferred as eight bytes.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned char norrath_restore_broadcast_head_view[8] NORRATH_LOADED(".sbss") = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
}
