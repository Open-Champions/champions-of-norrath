#include <cstring>

extern "C" unsigned soundSfxVolume;
extern "C" unsigned soundMusicVolume;
extern "C" unsigned soundStereoEnable;

int main() {
    const unsigned char one_f[4] = {0x00, 0x00, 0x80, 0x3f};
    const unsigned char one[4] = {0x01, 0x00, 0x00, 0x00};
    if (std::memcmp(&soundSfxVolume, one_f, 4) || std::memcmp(&soundMusicVolume, one_f, 4)) return 1;
    if (std::memcmp(&soundStereoEnable, one, 4)) return 2;
    if (soundSfxVolume != 0x3f800000u || soundMusicVolume != 0x3f800000u) return 3;
    return 0;
}
