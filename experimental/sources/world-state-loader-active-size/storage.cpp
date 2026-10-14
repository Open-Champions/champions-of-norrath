#include "storage.hpp"
// Observed lumpLoadSize at 0x00484018: four file-backed zero bytes in .sdata.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned char norrath_loader_active_size_view[4] NORRATH_LOADED(".sdata") = {
    0x00, 0x00, 0x00, 0x00
};
}
