// Observed amx_GetString at 0x138900, 128 bytes. The source is a run of
// 32-bit cells. A first cell that is 256 or more (signed compare) starts a
// packed string: four characters per cell, high byte first, copied until a
// zero character, which is also written. Any other first cell starts an
// unpacked string: the low byte of each cell is copied while the next cell is
// nonzero, then a zero is written. A first cell of zero or below 256 and zero
// gives an empty string; a negative first cell is unpacked and is copied.
// The return is 0.
extern "C" int amx_GetString(char* dest, int* source) {
    if (source[0] >= 256) {
        int word = 0;
        int lane = 3;
        for (;;) {
            if (lane == 3) {
                word = *source;
                source += 1;
            }
            const char c = static_cast<char>(word >> (lane * 8));
            lane = (lane + 3) & 3;
            *dest = c;
            if (c == 0) break;
            dest += 1;
        }
        return 0;
    }
    if (source[0] == 0) {
        *dest = 0;
        return 0;
    }
    do {
        *dest = static_cast<char>(*source);
        source += 1;
        dest += 1;
    } while (*source != 0);
    *dest = 0;
    return 0;
}
