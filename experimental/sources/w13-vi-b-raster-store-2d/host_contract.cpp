#define NORRATH_HOST_CONTRACT 1
#include <string.h>
static unsigned char scratch[1024];
extern "C" volatile unsigned char* norrath_host_scratch_byte(unsigned address) { return &scratch[address - 0x70000000u]; }
static int copies;
extern "C" void sceVu0CopyMatrix(void* destination, const void* source) { ++copies; memmove(destination, source, 64); }
#include "store_2d.cpp"

int main() {
    static unsigned char object[18000], want[18000];
    memset(object, 0x6b, sizeof object);
    memset(scratch, 0x77, sizeof scratch);
    for (int i = 0; i < 128; ++i) scratch[i] = static_cast<unsigned char>(i * 5 + 2);
    scratch[0x220] = 0x31; scratch[0x221] = 0x42;
    unsigned char scratchBefore[1024];
    memcpy(scratchBefore, scratch, sizeof scratch);
    memcpy(want, object, sizeof object);
    memcpy(want + 17584, scratchBefore, 128);
    want[17712] = 0x31; want[17713] = 0x42;
    want[17888] = 1; want[17889] = 0; want[17890] = 0; want[17891] = 0;
    if (Store2D__8VIRaster(object) != 0 || copies != 2) return 1;
    if (memcmp(object, want, sizeof object) != 0) return 2;
    for (unsigned i = 0; i < sizeof scratch; ++i) {
        unsigned char expect = scratchBefore[i];
        if (i == 0x220 || i == 0x221) expect = 0;
        if (scratch[i] != expect) return 3;
    }
    return 0;
}
