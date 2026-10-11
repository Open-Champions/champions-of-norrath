#include "storage.hpp"
// Observed freeroot at 0x00486564: four file-backed zero bytes in .sdata. routeAddNode treats -1 as the reclaim sentinel.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned char norrath_route_free_root_bytes[4] NORRATH_LOADED(".sdata") = {
    0x00, 0x00, 0x00, 0x00
};
}
