// Observed: ChannelCount__C6VIWave at 0x01138168, 8 bytes. It returns the word at +20 of the object; nothing else is read or written.
// Inferred: the word is the channel count of the wave (it is the same slot Format and SampleSize neighbours sit beside, at 0x01138160
// and 0x01138170, each also an 8-byte reader).
// Unknown: the layout of the other fields.
extern "C" {
int ChannelCount__C6VIWave(const void* wave) {
    return static_cast<const int*>(wave)[5];
}
}
