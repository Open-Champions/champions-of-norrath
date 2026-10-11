#include "storage.hpp"
// Observed lumpLoadLump at 0x00484014: four file-backed zero bytes in .sdata.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned char norrath_loader_active_buffer_view[4] NORRATH_LOADED(".sdata") = {
    0x00, 0x00, 0x00, 0x00
};
}
