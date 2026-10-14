// Host check only; not a build input.
int norrath_sound_bgm_version __asm__("_13VISoundDevice$sBgmVersion");
#include "bgm_buffer_size.cpp"
int main() {
    norrath_sound_bgm_version = 0;
    if (BgmBufferSize__13VISoundDevice(0) != 0x8000) return 1;
    norrath_sound_bgm_version = 1;
    if (BgmBufferSize__13VISoundDevice(0) != 0x20000) return 2;
    norrath_sound_bgm_version = 2;
    if (BgmBufferSize__13VISoundDevice(0) != 0x8000) return 3;
    norrath_sound_bgm_version = -1;
    if (BgmBufferSize__13VISoundDevice(0) != 0x8000) return 4;
    return 0;
}
