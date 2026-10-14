// Host check only; not a build input.
#include "begin_adpcm.cpp"
#include <cstring>
int main() {
    unsigned char dev[4096];
    std::memset(dev, 0x5a, sizeof dev);
    unsigned slot = 0x00123450;
    int out = 0;
    int r = BeginAdpcmTransfer__13VISoundDevicePPvUiiPi(dev, reinterpret_cast<void**>(&slot), 0x2000, 77, &out);
    unsigned* w = reinterpret_cast<unsigned*>(dev);
    if (r != 0 || slot != 0) return 1;
    if (w[3380 / 4] != 0x00123450 || w[3384 / 4] != 77 || w[3388 / 4] != 0x2000 || w[3392 / 4] != 0xffffffffu) return 2;
    if (w[3412 / 4] != 0 || w[3420 / 4] != 0x00123450) return 3;
    if (w[3416 / 4] != static_cast<unsigned>(reinterpret_cast<unsigned long>(&out))) return 4;
    for (int i = 0; i < 4096; ++i) {
        if (i >= 3380 && i < 3424) continue;
        if (dev[i] != 0x5a) return 5;
    }
    if (w[3396 / 4] != 0x5a5a5a5au || w[3400 / 4] != 0x5a5a5a5au || w[3404 / 4] != 0x5a5a5a5au || w[3408 / 4] != 0x5a5a5a5au) return 6;
    return 0;
}
