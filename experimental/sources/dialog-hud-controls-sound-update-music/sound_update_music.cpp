// Observed soundUpdateMusicVolume__Fv at 0x0024ee18, 68 bytes.
// It asks soundGetStreamPlayStatus for stream 0. Bit 0x20 clear returns.
// Otherwise it multiplies musicDuckAmount by soundMusicVolume, then by the
// float bits 0x3ef00000, and passes stream 0 to soundSetStreamVolume.
// Those four symbols are not defined here.
extern "C" int soundGetStreamPlayStatus__Fi(int stream);
extern "C" void soundSetStreamVolume__Fif(int stream, float volume);
extern "C" float musicDuckAmount;
extern "C" float soundMusicVolume;

extern "C" void soundUpdateMusicVolume__Fv() {
    if ((soundGetStreamPlayStatus__Fi(0) & 0x20) == 0) return;
    const unsigned scale_bits = 0x3ef00000u;
    float scale;
    __builtin_memcpy(&scale, &scale_bits, sizeof scale);
    const float volume = (musicDuckAmount * soundMusicVolume) * scale;
    soundSetStreamVolume__Fif(0, volume);
}
