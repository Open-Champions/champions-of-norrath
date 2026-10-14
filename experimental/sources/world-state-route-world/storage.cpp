#include "storage.hpp"
// Observed world at 0x004844a0: four file-backed zero bytes in .sdata. routeAddNode passes this pointer to the height query.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned char norrath_route_world_word_bytes[4] NORRATH_LOADED(".sdata") = {
    0x00, 0x00, 0x00, 0x00
};
}
