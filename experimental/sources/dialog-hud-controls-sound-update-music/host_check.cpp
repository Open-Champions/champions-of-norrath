#include <cstring>
#include <cstdint>

extern "C" {
float musicDuckAmount;
float soundMusicVolume;
void soundUpdateMusicVolume__Fv();
}

static int status_word;
static int status_stream;
static int sets;
static int set_stream;
static float set_volume;

extern "C" int soundGetStreamPlayStatus__Fi(int stream) {
    status_stream = stream;
    return status_word;
}

extern "C" void soundSetStreamVolume__Fif(int stream, float volume) {
    sets += 1;
    set_stream = stream;
    set_volume = volume;
}

static int bits_of(float value) {
    std::uint32_t bits = 0;
    std::memcpy(&bits, &value, 4);
    return static_cast<int>(bits);
}

int main() {
    musicDuckAmount = 3.f;
    soundMusicVolume = 5.f;
    status_word = 0x1f;
    soundUpdateMusicVolume__Fv();
    if (sets != 0 || status_stream != 0) return 1;

    status_word = 0x20;
    soundUpdateMusicVolume__Fv();
    if (sets != 1 || set_stream != 0) return 2;
    const float want = (3.f * 5.f) * 0.46875f;
    if (bits_of(set_volume) != bits_of(want)) return 3;
    if (bits_of(0.46875f) != 0x3ef00000) return 4;

    status_word = 0x21;
    soundUpdateMusicVolume__Fv();
    if (sets != 2) return 5;
    return 0;
}
