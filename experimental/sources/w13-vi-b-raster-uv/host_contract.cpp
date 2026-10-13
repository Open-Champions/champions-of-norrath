#include "uv.cpp"
#include <string.h>

int main() {
    unsigned char object[64];
    memset(object, 0x31, sizeof object);
    unsigned char want[64];
    memcpy(want, object, sizeof object);
    float uv[3] = {0.25f, -7.5f, 99.0f};
    memcpy(want + 12, uv, 8);
    if (UV__8VIRasterRC7VIVect2(object, uv) != 0) return 1;
    if (memcmp(object, want, sizeof object) != 0) return 2;
    return 0;
}
