#include <initializer_list>
#define NORRATH_HOST_CONTRACT 1
#include <string.h>
static unsigned char scratch[1024];
extern "C" volatile unsigned char* norrath_host_scratch_byte(unsigned address) { return &scratch[address - 0x70000000u]; }

static int order;
static const void* seenMatrix; static const void* seenSource; static void* seenDestination;
static unsigned char atMul[1024];
static void* seenNormalize; static int mulOrder, normOrder;
extern "C" void SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3(const void* matrix, const void* source, void* destination) {
    mulOrder = ++order; seenMatrix = matrix; seenSource = source; seenDestination = destination;
    memcpy(atMul, scratch, sizeof scratch);
    // Stand-in product: negate the destination's first float so the result is observable.
    float* d = static_cast<float*>(destination);
    d[0] = -d[0];
}
extern "C" void Normalize__7VIVect3(void* vector) {
    normOrder = ++order; seenNormalize = vector;
    static_cast<float*>(vector)[1] = 123.0f;
}
#include "set_sun_light.cpp"

int main() {
    static unsigned char buffer[96], object[18000], want[18000];
    unsigned char matrix[64];
    for (int offset : {0, 1}) {
        unsigned char* light = buffer + offset;
        float dir[3] = {1.5f, -2.5f, 3.5f};
        memcpy(light, dir, 12);
        for (int i = 12; i < 28; ++i) light[i] = static_cast<unsigned char>(0x40 + i);
        memset(object, 0x6b, sizeof object);
        memcpy(want, object, sizeof object);
        want[17888] = 1; want[17889] = 0; want[17890] = 0; want[17891] = 0;
        memset(scratch, 0x77, sizeof scratch);
        order = 0;
        if (SetSunLight__8VIRasterRC10VIMatrix44RC16VIRasterDirLight(object, matrix, light) != 0) return 1;
        if (memcmp(object, want, sizeof object) != 0) return 2;
        if (seenMatrix != matrix || seenSource != light || seenDestination != scratch + 0x1a0 || seenNormalize != scratch + 0x1a0) return 3;
        if (mulOrder != 1 || normOrder != 2) return 4;
        // Copies were complete before the product ran.
        if (memcmp(atMul + 0x1a0, dir, 12) != 0 || memcmp(atMul + 0x1ac, light + 12, 16) != 0) return 5;
        float out[3];
        memcpy(out, scratch + 0x1a0, 12);
        if (out[0] != -1.5f || out[1] != 123.0f || out[2] != 3.5f) return 6;
        if (memcmp(scratch + 0x1ac, light + 12, 16) != 0) return 7;
        for (unsigned i = 0; i < sizeof scratch; ++i)
            if ((i < 0x1a0 || i >= 0x1bc) && scratch[i] != 0x77) return 8;
    }
    return 0;
}
