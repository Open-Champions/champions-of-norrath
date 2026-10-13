#include <initializer_list>
#define NORRATH_HOST_CONTRACT 1
#include <string.h>
#include <stdint.h>
alignas(16) static unsigned char arena[512];
extern "C" unsigned char* norrath_host_load_pointer(const void* field) { uint32_t v; memcpy(&v, field, 4); return arena + v; }
extern "C" void norrath_host_store_pointer(void* field, unsigned char* value) { uint32_t v = static_cast<uint32_t>(value - arena); memcpy(field, &v, 4); }
#include "set_fog_registers.cpp"

int main() {
    alignas(16) static unsigned char object[20000], want[20000];
    struct Case { float c[4]; unsigned char b[3]; } cases[] = {
        {{0.0f, 0.0f, 0.0f, 1.0f}, {0, 0, 0}},
        {{1.0f, 0.5f, 0.25f, 0.0f}, {255, 128, 64}},
        {{0.2f, 0.4f, 0.6f, 0.9f}, {51, 102, 153}},
        {{0.001f, 0.998f, 0.999f, 7.0f}, {0, 254, 255}},
    };
    for (const Case& c : cases) {
        memset(arena, 0x5a, sizeof arena);
        memset(object, 0x6b, sizeof object);
        unsigned char* P = arena + 64;
        norrath_host_store_pointer(object + 19492, P);
        memcpy(object + 17560, c.c, 16);
        memcpy(want, object, sizeof object);
        unsigned char expect[512]; memcpy(expect, arena, sizeof arena);
        unsigned char* E = expect + 64;
        E[3] = 0x10; memset(E + 4, 0, 4); E[0] = 3; E[1] = 0;
        unsigned w[4] = {0, 0, 0x10000000u, 0x50000002u}; memcpy(E + 16, w, 16);
        unsigned long long q[4] = {0x1000000000008001ull, 14ull, (unsigned long long)c.b[0] | ((unsigned long long)c.b[1] << 8) | ((unsigned long long)c.b[2] << 16), 61ull};
        memcpy(E + 32, q, 32);
        norrath_host_store_pointer(want + 19492, P + 64);
        if (SetFogRegisters__8VIRaster(object) != 0) return 1;
        if (memcmp(object, want, sizeof object) != 0) return 2;
        if (memcmp(arena, expect, sizeof arena) != 0) return 3;
    }
    return 0;
}
