#include "storage.hpp"
// Observed cutMode at 0x00484318: one file-backed zero byte in .sdata. The next byte is the separate cutPlayMode object.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned char norrath_game_cut_mode_byte_view[1] NORRATH_LOADED(".sdata") = {
    0x00
};
}
