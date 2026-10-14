#include <initializer_list>
#include "vertex_group.cpp"
#include <string.h>

int main() {
    for (int g : {0, 1, 127, -1, -128, 5}) {
        unsigned char o[96];
        memset(o, 0x44, sizeof o);
        unsigned char want[96];
        memcpy(want, o, sizeof o);
        want[80] = static_cast<unsigned char>(g);
        if (VertexGroup__12VICollBufferSc(o, static_cast<signed char>(g)) != 0) return 1;
        if (memcmp(o, want, sizeof o) != 0) return 2;
    }
    return 0;
}
