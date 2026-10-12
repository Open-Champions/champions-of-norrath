// Host contract for colorScale__FUif. Not an EE plan input.
#include "color_scale.cpp"
int main() {
    if (colorScale__FUif(0x80403020u, 1.0f) != 0x80403020u) return 1;
    if (colorScale__FUif(0x80403020u, 0.5f) != 0x80201810u) return 2;
    if (colorScale__FUif(0xff102030u, 2.0f) != 0xff204060u) return 3;
    if (colorScale__FUif(0x01f0f0f0u, 2.0f) != 0x01ffffffu) return 4;
    if (colorScale__FUif(0x7f010203u, -1.0f) != 0x7f000000u) return 5;
    if (colorScale__FUif(0x12345678u, 0.0f) != 0x12000000u) return 6;
    if (colorScale__FUif(0x00ffffffu, 1.0f) != 0x00ffffffu) return 7;
    if (colorScale__FUif(0x00030303u, 0.5f) != 0x00010101u) return 8;  // 1.5 truncates
    if (colorScale__FUif(0x00ff00ffu, 1.00001f) != 0x00ff00ffu) return 9;
    return 0;
}
