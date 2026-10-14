// Observed: AttenuationMode__C13VISoundSprite at 0x0117c478, 8 bytes. With the sound sprite as
// this, it returns the 32-bit word at this + 76 and touches nothing else.
// Inferred: the attenuation-mode word; the name suffix C shows the method is const.
// Unknown: the member name and the full set of values the word can hold.
extern "C" {
int AttenuationMode__C13VISoundSprite(const void* object) {
    return *reinterpret_cast<const int*>(static_cast<const unsigned char*>(object) + 76);
}
}
