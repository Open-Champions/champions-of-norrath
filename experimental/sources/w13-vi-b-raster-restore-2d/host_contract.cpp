#define NORRATH_HOST_CONTRACT 1
#include <string.h>
static unsigned char scratch[1024];
extern "C" volatile unsigned char* norrath_host_scratch_byte(unsigned address) { return &scratch[address - 0x70000000u]; }
static int copies;
extern "C" void sceVu0CopyMatrix(void* destination, const void* source) { ++copies; memmove(destination, source, 64); }
#include "restore_2d.cpp"

int main() {
    static unsigned char object[18000], want[18000];
    memset(object, 0x6b, sizeof object);
    for (int i = 0; i < 130; ++i) object[17584 + i] = static_cast<unsigned char>(i * 3 + 1);
    object[17712] = 0x5a; object[17713] = 0xa5;
    memcpy(want, object, sizeof object);
    want[17888] = 1; want[17889] = 0; want[17890] = 0; want[17891] = 0;
    memset(scratch, 0x77, sizeof scratch);
    if (Restore2D__8VIRaster(object) != 0 || copies != 2) return 1;
    if (memcmp(object, want, sizeof object) != 0) return 2;
    for (unsigned i = 0; i < sizeof scratch; ++i) {
        unsigned char expect = 0x77;
        if (i < 64) expect = want[17584 + i];
        else if (i >= 64 && i < 128) expect = want[17648 + i - 64];
        else if (i == 0x220) expect = 0x5a;
        else if (i == 0x221) expect = 0xa5;
        if (scratch[i] != expect) return 3;
    }
    return 0;
}
