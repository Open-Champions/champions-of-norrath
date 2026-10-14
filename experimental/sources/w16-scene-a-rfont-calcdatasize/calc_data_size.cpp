// Observed: CalcDataSize__7VIRFontPUc at 0x01135c80 is 320 bytes in the pinned SLUS-20565 image. With n = the word at offset 40:
//   +64 = 0; +68 = 12 * n; +76 = +68 + 56 * (word at +48); +80 = +76.
// Then, for each of the n entries (none if n <= 0), the word at +28 selects how a data byte b = data[i] becomes a step (an unsigned
// compare against 7 guards a jump table with seven entries): 0 -> (b + 1) >> 1, 1 -> b, 2, 3 and 6 -> b << 2, 4 -> 3 * b, 5 -> b << 1;
// any other value returns -1 at once. Each step adds step * (word at +20) to the word at +80. After the loop: +72 = +80; if the word at
// +52 is zero it returns 0; else it returns -1 unless the word at +32 equals 2, in which case it adds 4 * (+52) to +80 and returns 0.
// Inferred: +28 is a pixel-format selector (the steps look like bytes per row for 4-, 8-, 16-, 24- and 32-bit rows, with the byte read
// as a row width; the enum parameter of CreateResources is named VIBitDepth), +40 is the entry count (Init stores `size` there),
// +68/+76 are table offsets inside one block and +80 the block size that Allocate__7VIRFont requests.
// Unknown: what the entries are, the meaning of +20 (set from Init's `extra`), +32, +48, +52, and the exact format of each selector value.
// Nothing here ran on the EE.
extern "C" {
int CalcDataSize__7VIRFontPUc(unsigned char* self, unsigned char* data) {
    int* w = reinterpret_cast<int*>(self);
    int n = w[10];
    w[16] = 0;
    w[17] = n * 12;
    w[19] = w[17] + w[12] * 56;
    w[20] = w[19];
    for (int i = 0; i < n; ++i) {
        int b = data[i];
        int step;
        switch (static_cast<unsigned>(w[7])) {
        case 0: step = (b + 1) >> 1; break;
        case 1: step = b; break;
        case 2: case 3: case 6: step = b << 2; break;
        case 4: step = b * 3; break;
        case 5: step = b << 1; break;
        default: return -1;
        }
        w[20] += step * w[5];
    }
    w[18] = w[20];
    if (w[13] != 0) {
        if (w[8] != 2) return -1;
        w[20] = w[18] + w[13] * 4;
    }
    return 0;
}
}
