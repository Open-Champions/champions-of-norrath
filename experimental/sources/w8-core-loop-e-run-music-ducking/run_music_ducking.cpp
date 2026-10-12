// Observed runMusicDucking__Fv at 0x0024b710, 232 bytes.
// It remembers musicDuckAmount, then reads soundGetStreamPlayStatus(2). When
// bit 0x20 of that status is set, desiredMusicDuckAmount is first set to 0.2.
// In either case desiredMusicDuckAmount then gains 0.016666668 and is limited
// to 1.0, and stored. musicDuckAmount steps toward the desired value by 0.01
// per call, without passing it. If musicDuckAmount changed and
// soundGetStreamPlayStatus(0) has bit 0x20 set, it calls
// soundSetStreamVolume(0, musicDuckAmount * soundMusicVolume * 0.46875).
// The three image constants are the bit patterns 3e4cccd0, 3c888888 and
// 3c23d70a (twice, used for both step directions).
// Inferred: the amount lowers music while the stream-2 status bit is set.
// Unknown: what stream 2 carries and what bit 0x20 of the status means.
extern "C" int soundGetStreamPlayStatus__Fi(int index);
extern "C" void soundSetStreamVolume__Fif(int index, float volume);
extern "C" float musicDuckAmount;
extern "C" float desiredMusicDuckAmount;
extern "C" float soundMusicVolume;

namespace {
__attribute__((always_inline)) inline float constant(unsigned bits) {
    union {
        unsigned word;
        float value;
    } u = {bits};
    return u.value;
}
}

extern "C" void runMusicDucking__Fv() {
    const int status = soundGetStreamPlayStatus__Fi(2);
    const float previous = musicDuckAmount;
    if ((status & 0x20) != 0) desiredMusicDuckAmount = constant(0x3e4cccd0u);
    float desired = desiredMusicDuckAmount + constant(0x3c888888u);
    if (1.0f < desired) desired = 1.0f;
    desiredMusicDuckAmount = desired;
    float amount = musicDuckAmount;
    if (desired < amount) {
        amount = amount - constant(0x3c23d70au);
        if (amount < desired) amount = desired;
        musicDuckAmount = amount;
    }
    if (amount < desired) {
        amount = amount + constant(0x3c23d70au);
        if (desired < amount) amount = desired;
        musicDuckAmount = amount;
    }
    if (amount == previous) return;
    if ((soundGetStreamPlayStatus__Fi(0) & 0x20) != 0)
        soundSetStreamVolume__Fif(0, musicDuckAmount * soundMusicVolume * constant(0x3ef00000u));
}
