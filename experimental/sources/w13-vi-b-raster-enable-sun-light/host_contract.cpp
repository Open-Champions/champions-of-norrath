#define NORRATH_HOST_CONTRACT 1
#include <initializer_list>
#include <string.h>
static unsigned char scratch[1024];
extern "C" volatile unsigned char* norrath_host_scratch_byte(unsigned address) { return &scratch[address - 0x70000000u]; }
#include "enable_sun_light.cpp"
#include <initializer_list>

int main() {
    for (int arg : {0, 1, 2, 0x1ff, -1}) {
        static unsigned char object[18000];
        memset(object, 0x6b, sizeof object);
        memset(scratch, 0x77, sizeof scratch);
        static unsigned char want[18000];
        memcpy(want, object, sizeof object);
        want[17888] = 1; want[17889] = 0; want[17890] = 0; want[17891] = 0;
        if (EnableSunLight__8VIRasteri(object, arg) != 0) return 1;
        if (memcmp(object, want, sizeof object) != 0) return 2;
        for (unsigned i = 0; i < sizeof scratch; ++i) {
            unsigned char expect = i == 545 ? static_cast<unsigned char>(arg) : 0x77;
            if (scratch[i] != expect) return 3;
        }
    }
    return 0;
}
