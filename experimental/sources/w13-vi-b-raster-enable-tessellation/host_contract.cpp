#include "enable_tessellation.cpp"
#include <initializer_list>
#include <string.h>

int main() {
    for (int v : {0, 1, 2, -1}) {
        unsigned char object[128];
        memset(object, 0x5a, sizeof object);
        unsigned char want[128];
        memcpy(want, object, sizeof object);
        memcpy(want + 60, &v, 4);
        EnableTessellation__8VIRasteri(object, v);
        if (memcmp(object, want, sizeof object) != 0) return 1;
    }
    return 0;
}
