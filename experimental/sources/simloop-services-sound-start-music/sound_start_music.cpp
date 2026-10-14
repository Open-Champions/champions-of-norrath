// Observed soundStartMusic__FPci at 0x0024eed0, 108 bytes.
// soundMusicVolume is compared with the word 0x3d4ccccc at 0x00482030. A
// smaller volume returns. Otherwise the incoming flags gain bit 1,
// soundSetStreamVolume receives stream 0 and musicDuckAmount times the music
// volume times the word 0x3ef00000, and soundStartDialogStream receives
// stream 0, the name, and those flags with bit 0x20 also set.
extern "C" {
extern float soundMusicVolume;
extern float musicDuckAmount;
void soundSetStreamVolume__Fif(int index, float volume);
void soundStartDialogStream__FiPci(int index, char* name, int flags);

static float bits(unsigned value) {
    float out;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&out);
    bytes[0] = static_cast<unsigned char>(value);
    bytes[1] = static_cast<unsigned char>(value >> 8);
    bytes[2] = static_cast<unsigned char>(value >> 16);
    bytes[3] = static_cast<unsigned char>(value >> 24);
    return out;
}

void soundStartMusic__FPci(char* name, int flags) {
    const float volume = soundMusicVolume;
    if (volume < bits(0x3d4cccccu)) return;
    flags |= 2;
    soundSetStreamVolume__Fif(0, (musicDuckAmount * volume) * bits(0x3ef00000u));
    soundStartDialogStream__FiPci(0, name, flags | 0x20);
}
}
