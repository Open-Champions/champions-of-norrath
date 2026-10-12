#include "change_volume.cpp"
#include <cstdio>
#include <cstring>
extern "C" float soundSfxVolume = 1.0f;
extern "C" int soundStereoEnable = 1;
extern "C" int nmBatchCommands = 0;
extern "C" unsigned char batchBuffer[512 * 8 + 16] = {};
extern "C" unsigned char norrath_sound_channel_voice_map_view[86] = {};
static unsigned short h(int e, int o) { unsigned short v; std::memcpy(&v, batchBuffer + e * 8 + o, 2); return v; }
static int w(int e) { int v; std::memcpy(&v, batchBuffer + e * 8 + 4, 4); return v; }
int main() {
    norrath_sound_channel_voice_map_view[8] = 1; norrath_sound_channel_voice_map_view[9] = 3;
    soundChangeVolume__Fiff(-1, 1, 1);
    if (nmBatchCommands != 0) return 1;
    soundSfxVolume = 0.5f;
    soundChangeVolume__Fiff(4, 1.0f, 0.5f);
    if (nmBatchCommands != 2) return 2;
    if (h(0, 0) != 1 || h(0, 2) != (1 | (3 << 1)) || w(0) != int(0.5f * 16383.0f)) return 3;
    if (h(1, 0) != 1 || h(1, 2) != ((1 | (3 << 1)) | 0x100) || w(1) != int(0.25f * 16383.0f)) return 4;
    // mono: both sides become half the sum of the scaled values
    soundStereoEnable = 0;
    soundChangeVolume__Fiff(4, 1.0f, 0.5f);
    if (w(2) != int(((0.5f + 0.25f) * 0.5f) * 16383.0f) || w(3) != w(2)) return 5;
    soundStereoEnable = 1; soundSfxVolume = 1.0f;
    // clamping: at 16384 and above to 16383, negative to zero
    soundChangeVolume__Fiff(4, 2.0f, -1.0f);
    if (w(4) != 16383 || w(5) != 0) return 6;
    soundChangeVolume__Fiff(4, 1.00006f, 1.0f);   // 16384.0 after scaling reaches the cap
    if (w(6) != 16383) return 7;
    soundChangeVolume__Fiff(4, 0.99993f, 0.0f);   // truncation, not rounding
    if (w(8) != int(0.99993f * 16383.0f) || w(8) == 16383 || w(9) != 0) return 8;
    // a signed map byte stays signed
    norrath_sound_channel_voice_map_view[2] = 0x80; norrath_sound_channel_voice_map_view[3] = 0;
    soundChangeVolume__Fiff(1, 0.0f, 0.0f);
    if (h(10, 2) != static_cast<unsigned short>(-128)) return 9;
    // room for only one more entry: the left entry is written, the right is not
    nmBatchCommands = 511;
    soundChangeVolume__Fiff(4, 1.0f, 1.0f);
    if (nmBatchCommands != 512 || h(511, 0) != 1 || w(511) != 16383) return 10;
    soundChangeVolume__Fiff(4, 0.5f, 0.5f);
    if (nmBatchCommands != 512 || w(511) != 16383) return 11;   // full: nothing is written
    for (int i = 512 * 8; i < 512 * 8 + 16; ++i) if (batchBuffer[i] != 0) return 12;  // nothing past the end
    std::puts("change_volume passed");
    return 0;
}
