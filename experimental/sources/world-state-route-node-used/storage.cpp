#include "storage.hpp"
// Observed nodeused at 0x00486560: four file-backed zero bytes in .sdata. routeAddNode increments it on allocation and decrements it when retiring a node.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned char norrath_route_node_used_bytes[4] NORRATH_LOADED(".sdata") = {
    0x00, 0x00, 0x00, 0x00
};
}
