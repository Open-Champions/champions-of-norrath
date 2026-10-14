#include <initializer_list>
#define NORRATH_HOST_CONTRACT 1
#include <string.h>
#include <stdint.h>
alignas(16) static unsigned char arena[512];
extern "C" unsigned char* norrath_host_load_pointer(const void* field) { uint32_t v; memcpy(&v, field, 4); return arena + v; }
extern "C" void norrath_host_store_pointer(void* field, unsigned char* value) { uint32_t v = static_cast<uint32_t>(value - arena); memcpy(field, &v, 4); }
extern "C" { unsigned char VIDMARasterMicrocode[16]; }
#include "upload_raster_micro.cpp"
unsigned int VIRasterPrimBuffer1 __asm__("_8VIRaster$PrimBuffer1") = 0x00100010u;
unsigned int VIRasterPrimBuffer2 __asm__("_8VIRaster$PrimBuffer2") = 0x00104050u;

static void put8(unsigned char* b, int off, unsigned v) { b[off] = static_cast<unsigned char>(v); }
static void put16(unsigned char* b, int off, unsigned v) { b[off] = static_cast<unsigned char>(v); b[off + 1] = static_cast<unsigned char>(v >> 8); }
static void put32(unsigned char* b, int off, unsigned v) { memcpy(b + off, &v, 4); }

int main() {
    alignas(16) static unsigned char object[20000], want[20000];
    for (int start : {0, 48, 160}) {
        memset(arena, 0x5a, sizeof arena);
        memset(object, 0x6b, sizeof object);
        unsigned char* P = arena + 64 + start;
        unsigned char* slot = object + 19492;
        norrath_host_store_pointer(slot, P);
        memcpy(want, object, sizeof object);
        unsigned char expect[512]; memcpy(expect, arena, sizeof arena);
        unsigned char* E = expect + 64 + start;
        put16(E, 0, 1); put8(E, 3, 0x10); put32(E, 4, 0);
        put32(E, 16, 0); put32(E, 20, 0); put32(E, 24, 0); put32(E, 28, 0x13000000);
        put8(E, 35, 0x50); put32(E, 36, static_cast<unsigned int>(reinterpret_cast<unsigned long>(VIDMARasterMicrocode)));
        put16(E, 32, 0);
        put8(E, 51, 0x10); put32(E, 52, 0);
        put16(E, 48, 1);
        put32(E, 64, 0); put32(E, 68, 0); put32(E, 72, 0x00100010u | 0x03000000u); put32(E, 76, (0x00104050u - 0x00100010u) | 0x02000000u);
        norrath_host_store_pointer(want + 19492, P + 80);
        int one = 1;
        memcpy(want + 17896, &one, 4); memcpy(want + 19532, &one, 4); memcpy(want + 17888, &one, 4);
        if (UploadRasterMicro__8VIRasterPP10_sceDmaTag(object, slot) != 0) return 1;
        if (memcmp(object, want, sizeof object) != 0) return 2;
        if (memcmp(arena, expect, sizeof arena) != 0) return 3;
    }
    return 0;
}
