// Host check only; not a build input. Compiles the storage with the existing BGM buffer-size reader.
#include "bgm_version.cpp"
#include "../w15-vi-io-sound-a-sound-device-bgm-buffer-size/bgm_buffer_size.cpp"
int main() {
    if (norrath_sound_bgm_version != 0) return 1;
    if (BgmBufferSize__13VISoundDevice(0) != 0x8000) return 2;
    norrath_sound_bgm_version = 1;
    if (BgmBufferSize__13VISoundDevice(0) != 0x20000) return 3;
    norrath_sound_bgm_version = 0;
    return 0;
}
