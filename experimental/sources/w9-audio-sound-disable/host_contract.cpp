#include "sound_disable.cpp"
#include <cstdio>
extern "C" int disableSoundPlaying = 12345;
int main() {
    soundDisable__Ff(2.0f);
    if (disableSoundPlaying != 120) return 1;
    soundDisable__Ff(0.0f);
    if (disableSoundPlaying != 0) return 2;
    soundDisable__Ff(0.025f);   // 1.5 frames truncates toward zero
    if (disableSoundPlaying != 1) return 3;
    soundDisable__Ff(-0.5f);
    if (disableSoundPlaying != -30) return 4;
    std::puts("sound_disable passed");
    return 0;
}
