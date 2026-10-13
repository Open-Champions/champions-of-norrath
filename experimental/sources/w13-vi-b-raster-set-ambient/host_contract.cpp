#include <initializer_list>
#define NORRATH_HOST_CONTRACT 1
#include <string.h>
static unsigned char scratch[1024];
extern "C" volatile unsigned char* norrath_host_scratch_byte(unsigned address) { return &scratch[address - 0x70000000u]; }
#include "set_ambient.cpp"

int main() {
    // Source at an odd address to exercise the unaligned copy.
    static unsigned char buffer[64];
    static unsigned char object[18000];
    for (int offset : {0, 1, 3}) {
        unsigned char* color = buffer + offset;
        for (int i = 0; i < 16; ++i) color[i] = static_cast<unsigned char>(0x30 + i * 7);
        memset(object, 0x6b, sizeof object);
        memset(scratch, 0x77, sizeof scratch);
        static unsigned char want[18000];
        memcpy(want, object, sizeof object);
        want[17888] = 1; want[17889] = 0; want[17890] = 0; want[17891] = 0;
        if (SetAmbient__8VIRasterRC10VIColor32F(object, color) != 0) return 1;
        if (memcmp(object, want, sizeof object) != 0) return 2;
        for (unsigned i = 0; i < sizeof scratch; ++i) {
            unsigned char expect = (i >= 0x1e0 && i < 0x1f0) ? color[i - 0x1e0] : 0x77;
            if (scratch[i] != expect) return 3;
        }
    }
    return 0;
}
