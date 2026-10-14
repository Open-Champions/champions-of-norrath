#include "flora_type.cpp"
#include <initializer_list>
#include <string.h>

int main() {
    for (int t : {0, 1, 127, -1, -128}) {
        unsigned char o[96], want[96];
        memset(o, 0x77, sizeof o);
        memcpy(want, o, sizeof o);
        want[81] = static_cast<unsigned char>(t);
        if (FloraType__12VICollBufferSc(o, static_cast<signed char>(t)) != 0) return 1;
        if (memcmp(o, want, sizeof o) != 0) return 2;
    }
    return 0;
}
