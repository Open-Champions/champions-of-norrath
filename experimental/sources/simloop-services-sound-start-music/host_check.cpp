#include <cstdio>
#include <cstdlib>
extern "C" float soundMusicVolume = 0.f;
extern "C" float musicDuckAmount = 1.f;
static int volumes = 0;
static int starts = 0;
static float seen_volume = 0.f;
static int seen_flags = 0;
static const char* seen_name = nullptr;
extern "C" void soundSetStreamVolume__Fif(int index, float volume) {
    if (index != 0) std::abort();
    volumes += 1;
    seen_volume = volume;
}
extern "C" void soundStartDialogStream__FiPci(int index, char* name, int flags) {
    if (index != 0) std::abort();
    starts += 1;
    seen_name = name;
    seen_flags = flags;
}
extern "C" void soundStartMusic__FPci(char*, int);
static float bits(unsigned value) {
    float out;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&out);
    bytes[0] = static_cast<unsigned char>(value);
    bytes[1] = static_cast<unsigned char>(value >> 8);
    bytes[2] = static_cast<unsigned char>(value >> 16);
    bytes[3] = static_cast<unsigned char>(value >> 24);
    return out;
}
int main() {
    soundMusicVolume = bits(0x3d4cccccu) * 0.5f;
    soundStartMusic__FPci(const_cast<char*>("theme"), 0);
    if (volumes != 0 || starts != 0) return 1;
    soundMusicVolume = 1.f;
    musicDuckAmount = 0.5f;
    soundStartMusic__FPci(const_cast<char*>("theme"), 0x1);
    if (volumes != 1 || starts != 1 || seen_flags != (0x1 | 0x2 | 0x20)) return 2;
    if (seen_name == nullptr || seen_volume != (0.5f * 1.f) * bits(0x3ef00000u)) return 3;
    std::puts("start music contract passed");
    return 0;
}
