#define NORRATH_HOST_CONTRACT 1
#include <initializer_list>
#include <string.h>
#include <stdint.h>
alignas(16) static unsigned char arena[0x200000];
extern "C" unsigned char* norrath_host_load_pointer(const void* field) { uint32_t v; memcpy(&v, field, 4); return arena + v; }
extern "C" void norrath_host_store_pointer(void* field, unsigned char* value) { uint32_t v = static_cast<uint32_t>(value - arena); memcpy(field, &v, 4); }

static int calls[8], order, sendArg, flipCount;
static void* sendData;
static unsigned char* flipRaster;
extern "C" int VIEEDMASync__Fi(int channel) { if (channel != 1) return 9; calls[order++] = 1; return 0; }
extern "C" int Flip__8VIRaster(void* raster) { calls[order++] = 2; flipRaster = static_cast<unsigned char*>(raster); ++flipCount; return 0; }
extern "C" int FlushCache(int mode) { if (mode != 0) return 9; calls[order++] = 3; return 0; }
extern "C" int VIEEDMASend__FiPv(int channel, void* data) { calls[order++] = 4; sendArg = channel; sendData = data; return 0; }
#include "flush_dma_buffer_if_full.cpp"

static int rd(unsigned char* o, int off) { int v; memcpy(&v, o + off, 4); return v; }
static void wr(unsigned char* o, int off, int v) { memcpy(o + off, &v, 4); }

int main() {
    alignas(16) static unsigned char object[20000];
    unsigned char* bufA = arena + 0x1000;
    unsigned char* bufB = arena + 0x100000;
    struct Case { int qwords; int counter; int flag; bool second; } cases[] = {
        {0, 900, 0, false},      // empty: nothing happens
        {100, 820, 1, false},    // small and counter below threshold: nothing
        {100, 821, 1, false},    // counter at threshold, flag set: send, switch to B
        {100, 821, 0, false},    // flag clear: sync, flip, then send
        {0x9200, 0, 1, false},   // exactly at the size limit: counter decides, nothing
        {0x9201, 0, 1, true},    // above the size limit: send from buffer B back to A
        {0x9201, 0, 0, false},
    };
    for (const Case& c : cases) {
        memset(object, 0x6b, sizeof object);
        memset(arena, 0x5a, sizeof arena);
        unsigned char* start = c.second ? bufB : bufA;
        norrath_host_store_pointer(object + 19484, bufA);
        norrath_host_store_pointer(object + 19488, bufB);
        norrath_host_store_pointer(object + 19496, start);
        norrath_host_store_pointer(object + 19492, start + c.qwords * 16);
        wr(object, 19480, c.counter); wr(object, 19504, c.flag); wr(object, 19500, 40); wr(object, 17880, 10);
        unsigned char before[20000]; memcpy(before, object, sizeof object);
        order = 0; flipCount = 0; sendData = 0;
        if (FlushDMABufferIfFull__8VIRaster(object) != 0) return 1;
        bool acts = c.qwords != 0 && (c.qwords > 0x9200 || c.counter >= 821);
        if (!acts) {
            if (order != 0 || memcmp(before, object, sizeof object) != 0) return 2;
            continue;
        }
        int expectOrder[8], n = 0;
        if (!c.flag) { expectOrder[n++] = 1; expectOrder[n++] = 2; }
        expectOrder[n++] = 3; expectOrder[n++] = 1; expectOrder[n++] = 4;
        if (order != n) return 3;
        for (int i = 0; i < n; ++i) if (calls[i] != expectOrder[i]) return 4;
        if (sendArg != 1 || sendData != start) return 5;
        if (!c.flag && (flipCount != 1 || flipRaster != object || rd(object, 17880) != 9)) return 6;
        if (c.flag && rd(object, 17880) != 10) return 7;
        unsigned char* next = c.second ? bufA : bufB;
        next = c.second ? bufA : bufB;
        // Buffer switch: the start was bufA -> use bufB; start was bufB -> use bufA.
        if (norrath_host_load_pointer(object + 19492) != next || norrath_host_load_pointer(object + 19496) != next) return 8;
        if (rd(object, 19480) != 0 || rd(object, 19500) != 41 || rd(object, 19504) != 1) return 9;
        unsigned char* tag = start + c.qwords * 16;
        if (tag[3] != 0x70 || rd(tag, 4) != 0 || tag[0] || tag[1] || tag[2] != 0x5a) return 10;
    }
    return 0;
}
