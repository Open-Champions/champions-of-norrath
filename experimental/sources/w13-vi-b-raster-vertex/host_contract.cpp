#include <initializer_list>
#define NORRATH_HOST_CONTRACT 1
#include <string.h>
#include <stdint.h>
alignas(16) static unsigned char arena[1024];
extern "C" unsigned char* norrath_host_load_pointer(const void* field) { uint32_t v; memcpy(&v, field, 4); return arena + v; }
extern "C" void norrath_host_store_pointer(void* field, unsigned char* value) { uint32_t v = static_cast<uint32_t>(value - arena); memcpy(field, &v, 4); }
#include "vertex.cpp"

int main() {
    alignas(16) static unsigned char object[256], want[256];
    float position[3] = {1.25f, -2.5f, 3.75f};
    for (int count : {0, 5}) {
        memset(arena, 0x5a, sizeof arena);
        memset(object, 0, sizeof object);
        float attr[12];
        for (int i = 0; i < 12; ++i) attr[i] = 100.0f + i;
        memcpy(object + 12, attr + 3, 36);   // floats 3..11 as in the raster
        unsigned char* cursor = arena + 128;
        norrath_host_store_pointer(object + 52, cursor);
        memcpy(object + 48, &count, 4);
        memcpy(want, object, sizeof object);
        unsigned char expectArena[1024]; memcpy(expectArena, arena, sizeof arena);
        float e[16] = {1.25f, -2.5f, 3.75f, 1.0f, 103.0f, 104.0f, 1.0f, 0.0f, 105.0f, 106.0f, 107.0f, 0.0f, 108.0f, 109.0f, 110.0f, 111.0f};
        memcpy(expectArena + 128, e, sizeof e);
        norrath_host_store_pointer(want + 52, cursor + 64);
        int next = count + 1; memcpy(want + 48, &next, 4);
        if (Vertex__8VIRasterRC7VIVect3(object, position) != 0) return 1;
        if (memcmp(object, want, sizeof object) != 0) return 2;
        if (memcmp(arena, expectArena, sizeof arena) != 0) return 3;
    }
    return 0;
}
