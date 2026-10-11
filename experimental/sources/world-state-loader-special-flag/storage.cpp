#include "storage.hpp"
// Observed noNewVags at 0x00483f90: four file-backed zero bytes in .sdata. lumpLoad stores 1 and later 0 here.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned char norrath_loader_special_flag_view[4] NORRATH_LOADED(".sdata") = {
    0x00, 0x00, 0x00, 0x00
};
}
