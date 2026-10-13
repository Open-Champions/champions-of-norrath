// Observed: SetNumLayers__10VIMaterialiR8VIRasterR12VIDictionary at 0x010ce7b8 is 356 bytes.
// Arguments: material (a0), new count n (a1), raster (a2), dictionary (a3). Layers are 144-byte
// records starting at material+48; the current count is the int at +8.
//   - If n >= 4 it returns -1 and changes nothing (negative n is not rejected).
//   - Growing (n > count): for each new layer index i it stores in the layer: -1 at +0, 1 at
//     +4, the byte -1 at +8, +9, +10 and +11, zero at +12, calls Identity__10VIMatrix33(layer
//     + 16), and stores 0.0f at +64 and +68.
//   - Shrinking (n < count): for each removed layer index i from n up to the count (re-read from
//     material+8 after every release) it calls ReleaseSurface__8VIRasteriR12VIDictionary(raster,
//     layer[0], dictionary) when the word at layer+0 is not -1.
//   - In both cases and when n equals the count, it then stores n at +8, 1 at +32 and returns 0.
// Inferred: a material holds up to four texture layers; +0 is a surface id (-1 = none), the four
// bytes at +8 a colour (all 255 = white), +16 a 3x3 matrix (reset to identity), +64/+68 an offset
// pair, and +32 on the material is a "changed" flag, as in SetLayerTexture. Released layers keep
// their old contents.
// Unknown: the meaning of the word at layer+4 (set to 1) and layer+12, and why n is limited to 4.
extern "C" {
void Identity__10VIMatrix33(void* matrix);
void ReleaseSurface__8VIRasteriR12VIDictionary(void* raster, int surface, void* dictionary);

int SetNumLayers__10VIMaterialiR8VIRasterR12VIDictionary(unsigned char* material, int count, void* raster, void* dictionary) {
    if (count >= 4) {
        return -1;
    }
    int current = *reinterpret_cast<int*>(material + 8);
    if (current < count) {
        for (int i = current; i < count; ++i) {
            unsigned char* layer = material + 48 + 144 * i;
            *reinterpret_cast<int*>(layer) = -1;
            *reinterpret_cast<int*>(layer + 4) = 1;
            layer[8] = 0xff;
            layer[9] = 0xff;
            layer[10] = 0xff;
            layer[11] = 0xff;
            *reinterpret_cast<int*>(layer + 12) = 0;
            Identity__10VIMatrix33(layer + 16);
            *reinterpret_cast<float*>(layer + 64) = 0.0f;
            *reinterpret_cast<float*>(layer + 68) = 0.0f;
        }
    } else if (count < current) {
        int i = count;
        do {
            int surface = *reinterpret_cast<int*>(material + 48 + 144 * i);
            if (surface != -1) {
                ReleaseSurface__8VIRasteriR12VIDictionary(raster, surface, dictionary);
                current = *reinterpret_cast<int*>(material + 8);
            }
            ++i;
        } while (i < current);
    }
    *reinterpret_cast<int*>(material + 8) = count;
    *reinterpret_cast<int*>(material + 32) = 1;
    return 0;
}
}
