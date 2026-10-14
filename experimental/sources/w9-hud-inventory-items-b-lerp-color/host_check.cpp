extern "C" unsigned lerpColor__FUiUif(unsigned, unsigned, float);

int main() {
    if (lerpColor__FUiUif(0x00000000u, 0xffffffffu, 0.0f) != 0u) return 1;
    if (lerpColor__FUiUif(0x00000000u, 0xffffffffu, 1.0f) != 0xffffffffu) return 2;
    // channels interpolate independently, at their own byte positions
    if (lerpColor__FUiUif(0x00000000u, 0x80402010u, 0.5f) != 0x40201008u) return 3;
    if (lerpColor__FUiUif(0xff00ff00u, 0x00ff00ffu, 0.25f) != 0xbf3fbf3fu) return 4;
    // decreasing channel, truncation toward zero: 100 + (0-100)*0.333 = 66.7 -> 66
    if (lerpColor__FUiUif(100u, 0u, 0.333f) != 66u) return 5;
    // high byte (above 127) converts as unsigned
    if (lerpColor__FUiUif(0xf0000000u, 0xf0000000u, 0.7f) != 0xf0000000u) return 6;
    if (lerpColor__FUiUif(0x80000000u, 0xc0000000u, 0.5f) != 0xa0000000u) return 7;
    // no clamping: 200 + (250-200)*2 = 300 spills into the next byte
    if (lerpColor__FUiUif(200u, 250u, 2.0f) != 300u) return 8;
    return 0;
}
