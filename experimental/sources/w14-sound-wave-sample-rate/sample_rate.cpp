// Observed: SampleRate__C6VIWave at 0x011381f0, 8 bytes. It returns the word at +32 of the object; nothing else is read or written.
// Inferred: the word is the sample rate; the neighbouring SetSampleRate__6VIWavei at 0x011381f8 is also an 8-byte accessor.
// Unknown: the unit and the layout of other fields.
extern "C" {
int SampleRate__C6VIWave(const void* wave) {
    return static_cast<const int*>(wave)[8];
}
}
