#include "storage.hpp"
// Observed drawFrameNm at 0x004844a8: four file-backed bytes 00 04 00 00 in .sdata, the integer 1024. routeAddNode subtracts each cache stamp from this word.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned char norrath_route_draw_frame_bytes[4] NORRATH_LOADED(".sdata") = {
    0x00, 0x04, 0x00, 0x00
};
}
