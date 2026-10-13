// Observed: soundStartMusic__Fii at 0x0024ee60 is 108 bytes; arguments are (int id, int flags).
// soundMusicVolume (0x00485934) is compared with the float 0x3d4ccccc, which the image holds
// at 0x0048202c. A smaller volume returns without any call. Otherwise it calls
// soundSetStreamVolume__Fif(0, musicDuckAmount * soundMusicVolume * 0x3ef00000-as-float) and then
// soundStartDialogStream__Fiii(0, id, flags | 0x22). This is the integer-id sibling of
// soundStartMusic__FPci, which does the same with a name.
// Inferred: nothing.
// Unknown: meaning of flag bits 0x2 and 0x20, and what the 0x3ef00000 scale represents.

extern "C" {
extern float soundMusicVolume;
extern float musicDuckAmount;
void soundSetStreamVolume__Fif(int index, float volume);
void soundStartDialogStream__Fiii(int index, int id, int flags);

static float bits(unsigned value) {
    float out;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&out);
    bytes[0] = static_cast<unsigned char>(value);
    bytes[1] = static_cast<unsigned char>(value >> 8);
    bytes[2] = static_cast<unsigned char>(value >> 16);
    bytes[3] = static_cast<unsigned char>(value >> 24);
    return out;
}

void soundStartMusic__Fii(int id, int flags) {
    const float volume = soundMusicVolume;
    if (volume < bits(0x3d4cccccu)) return;
    flags |= 2;
    soundSetStreamVolume__Fif(0, (musicDuckAmount * volume) * bits(0x3ef00000u));
    soundStartDialogStream__Fiii(0, id, flags | 0x20);
}
}
