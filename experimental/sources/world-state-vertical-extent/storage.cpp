#include "storage.hpp"
// Observed four-byte load at 0x0047fcc8 inside .lit4, bits 0x47c35000. objectFindEmptySpot uses it as the vertical extent. It has no dedicated symbol; neighboring literal-pool entries are not included.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
const unsigned char norrath_grid_vertical_extent_view[4] NORRATH_LOADED(".lit4") = {
    0x00, 0x50, 0xc3, 0x47
};
}
