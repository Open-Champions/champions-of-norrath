#include <initializer_list>
#define NORRATH_HOST_CONTRACT 1
#include <string.h>
#include <stdint.h>
alignas(16) static unsigned char arena[1024];
extern "C" unsigned char* norrath_host_load_pointer(const void* field) { uint32_t v; memcpy(&v, field, 4); return arena + v; }
extern "C" void norrath_host_store_pointer(void* field, unsigned char* value) { uint32_t v = static_cast<uint32_t>(value - arena); memcpy(field, &v, 4); }
static int flushes; static void* flushArg;
extern "C" int FlushDMABufferIfFull__8VIRaster(void* raster) { ++flushes; flushArg = raster; return 0; }
#include "end_tri_fan.cpp"
unsigned int VIRasterFanOffset __asm__("_8VIRaster$RFanOffset") = 0x00123456u;

static unsigned word(const unsigned char* base, int index) { unsigned v; memcpy(&v, base + index * 4, 4); return v; }

int main() {
    alignas(16) static unsigned char object[20000];
    // Packet start at arena+64, cursor after three vertices: start + 32 + 3 * 16.
    for (int vertices : {0, 1, 3, 7}) {
        memset(arena, 0x5a, sizeof arena);
        memset(object, 0x6b, sizeof object);
        unsigned char* start = arena + 64;
        unsigned char* cursor = start + 32 + vertices * 16;
        unsigned char* tag = arena + 16;
        norrath_host_store_pointer(object + 52, cursor);
        norrath_host_store_pointer(object + 56, start);
        norrath_host_store_pointer(object + 19492, tag);
        unsigned counter = 100 + vertices; memcpy(object + 48, &counter, 4);
        int mode = 2; memcpy(object + 17884, &mode, 4);
        unsigned char before[1024]; memcpy(before, arena, sizeof arena);
        flushes = 0;
        if (EndTriFan__8VIRaster(object) != 0 || flushes != 1 || flushArg != object) return 1;
        int n = (32 + vertices * 16 + 16) / 16;
        // Closing entry.
        if (word(cursor, 0) || word(cursor, 1) || word(cursor, 2) || word(cursor, 3) != (0x14000000u | 0x00123456u)) return 2;
        // Header.
        if (word(start, 0) || word(start, 1) || word(start, 2) || word(start, 3) != ((unsigned(n - 2) << 16) | 0x6c008000u)) return 3;
        // Counter entry.
        if (word(start + 16, 0) != counter || word(start + 16, 1) || word(start + 16, 2) || word(start + 16, 3)) return 4;
        // Cursor ends at start + 32.
        if (norrath_host_load_pointer(object + 52) != start + 32) return 5;
        // DMA tag.
        unsigned short qwc; memcpy(&qwc, tag, 2);
        if (qwc != n || tag[3] != 0x10 || word(tag, 1) != 0) return 6;
        if (tag[2] != 0x5a) return 7;
        if (norrath_host_load_pointer(object + 19492) != tag + n * 16 + 16) return 8;
        int cleared; memcpy(&cleared, object + 17884, 4);
        if (cleared != 0) return 9;
        // Nothing else in the arena changed.
        for (int i = 0; i < (int)sizeof arena; ++i) {
            bool touched = (i >= cursor - arena && i < cursor - arena + 16) || (i >= start - arena && i < start - arena + 32) ||
                           (i >= tag - arena && i < tag - arena + 8);
            if (!touched && arena[i] != before[i]) return 10;
        }
    }
    return 0;
}
