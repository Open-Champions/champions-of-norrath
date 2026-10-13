#include "enable_all_dir_lights.cpp"
#include <initializer_list>
#include <string.h>

int main() {
    for (int arg : {0, 1, 5, -3}) {
        static unsigned char object[18000];
        memset(object, 0x6b, sizeof object);
        static unsigned char want[18000];
        memcpy(want, object, sizeof object);
        want[17888] = 1; want[17889] = 0; want[17890] = 0; want[17891] = 0;
        if (EnableAllDirLights__8VIRasteri(object, arg) != 0) return 1;
        if (memcmp(object, want, sizeof object) != 0) return 2;
    }
    return 0;
}
