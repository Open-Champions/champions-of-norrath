// Observed: IsLooping__C19VIStreamAudioSprite at 0x01019338, 8 bytes. With the stream audio sprite as
// this, it returns the 32-bit word at this + 64 and touches nothing else.
// Inferred: the looping word; the name suffix C shows the method is const.
// Unknown: the member name and the full set of values the word can hold.
extern "C" {
int IsLooping__C19VIStreamAudioSprite(const void* object) {
    return *reinterpret_cast<const int*>(static_cast<const unsigned char*>(object) + 64);
}
}
