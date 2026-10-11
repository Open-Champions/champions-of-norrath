#include "storage.hpp"
// Observed coreLoading at 0x00486680: four file-backed zero bytes in .sdata.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned char norrath_game_core_loading_word_view[4] NORRATH_LOADED(".sdata") = {
    0x00, 0x00, 0x00, 0x00
};
}
